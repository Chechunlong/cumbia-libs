#include "qmlwriterbackend.h"
#include "cucontrolswriter_abs.h"
#include <cumacros.h>
#include <cumbiapool.h>
#include <cumbiapool_o.h>
#include <cudata.h>
#include <QStringList>
#include <vector>

#include <cucontrolsfactories_i.h>
#include <cucontrolsfactorypool.h>
#include <culinkstats.h>
#include <cucontextmenu.h>
#include <cucontext.h>
#include <culog.h>
#include <cuserviceprovider.h>
#include <cuservices.h>
#include <qulogimpl.h>

/** @private */
class QmlWriterBackendPrivate
{
public:
    bool auto_configure;
    bool ok;
    CuContext *context;
    QString target, unit, description;
    QString label;
    QVariant min, max, value;
};

QmlWriterBackend::QmlWriterBackend(QObject *parent) :
    QObject(parent), CuDataListener()
{
    m_init();
}

void QmlWriterBackend::m_init()
{
    d = new QmlWriterBackendPrivate;
    d->context = NULL;
    d->auto_configure = true;
    d->ok = true;
    d->min = 0.0;
    d->max = 1.0;
    d->value = 0.0;
}

void QmlWriterBackend::init(CumbiaPool_O *poo_o)
{
    d->context = new CuContext(poo_o->getPool(), poo_o->getFactory());
    if(!d->target.isEmpty())
        setTarget(d->target);
}

QmlWriterBackend::~QmlWriterBackend()
{
    pdelete("~QmlWriterBackend %p", this);
    if(d->context)
        delete d->context;
    delete d;
}

QString QmlWriterBackend::target() const {
    if(d->context) {
        CuControlsWriterA *w = d->context->getWriter();
        if(w != NULL)
            return w->target();
    }
    return "";
}

/** \brief returns the pointer to the CuContext
 *
 * CuContext sets up the connection and is used as a mediator to send and get data
 * to and from the reader.
 *
 * @see CuContext
 */
CuContext *QmlWriterBackend::getContext() const
{
    return d->context;
}

QVariant QmlWriterBackend::min() const
{
    return d->min;
}

QVariant QmlWriterBackend::max() const
{
    return d->max;
}

QVariant QmlWriterBackend::value() const
{
    return d->value;
}

QString QmlWriterBackend::label() const
{
    return d->label;
}

QString QmlWriterBackend::unit() const
{
    return d->unit;
}

QString QmlWriterBackend::description() const
{
    return d->description;
}

bool QmlWriterBackend::ok() const
{
    return d->ok;
}

/** \brief Connect the reader to the specified source.
 *
 * If a reader with a different source is configured, it is deleted.
 * If options have been set with QuContext::setOptions, they are used to set up the reader as desired.
 *
 * @see QuContext::setOptions
 * @see source
 */
void QmlWriterBackend::setTarget(const QString &target)
{
    if(d->context) {
        CuControlsWriterA* w = d->context->replace_writer(target.toStdString(), this);
        if(w && target != w->target()) {
            w->setTarget(target);
        }
    }
    if(d->target != target) {
        d->target = target;
        emit targetChanged();
    }
}

void QmlWriterBackend::m_configure(const CuData& da)
{
    QString description, unit, label;
    QVariant v;

    // unit, label, description
    unit = QString::fromStdString(da["display_unit"].toString());
    label = QString::fromStdString(da["label"].toString());
    description = QString::fromStdString(da["description"].toString());
    if(unit != d->unit) {
        d->unit = unit;
        emit unitChanged();
    }
    if(label != d->label) {
        d->label = label;
        emit labelChanged();
    }
    if(description != d->description) {
        d->description = description;
        emit descriptionChanged();
    }

    // min, max
    try {
        if(da.containsKey("min"))
            v = QVariant(std::stod(da["min"].toString()));
        if(d->min != v) {
            d->min = v;
            emit minChanged();
        }
        if(da.containsKey("max"))
            v = QVariant(std::stod(da["max"].toString()));
        if(d->max != v) {
            d->max = v;
            emit maxChanged();
        }
    }

    catch(const std::invalid_argument& ia) {
        perr("QmlReaderBackend.m_configure: number coversion error: %s", ia.what());
    }
    // initialise the object with the "write" value (also called "set point"), if available:
    //
    if(d->ok && da.containsKey("w_value")) {
        CuVariant var = da["w_value"];
        if(var.getFormat() == CuVariant::Scalar) {
            if(var.isFloatingPoint() || var.isInteger()) {
                double dval;
                var.to<double>(dval);
                if(dval != d->value) {
                    d->value = dval;
                    emit valueChanged();
                }
            }
            else if(var.getType() == CuVariant::String) {
                QVariant str = QString::fromStdString(var.toString());
                if(d->value != str) {
                    d->value = str;
                    emit valueChanged();
                }
            }
        }
    }
}

void QmlWriterBackend::onUpdate(const CuData &da)
{
    bool ok = !da["err"].toBool();
    if(d->ok != ok) {
        d->ok = ok;
        emit okChanged();
    }

    if(!d->ok) {
        perr("QmlWriterBackend [%s]: error %s target: \"%s\" format %s (writable: %d)", qstoc(objectName()),
             da["src"].toString().c_str(), da["msg"].toString().c_str(),
                da["data_format_str"].toString().c_str(), da["writable"].toInt());

        Cumbia* cumbia = d->context->cumbia();
        if(!cumbia) /* pick from the CumbiaPool */
            cumbia = d->context->cumbiaPool()->getBySrc(da["src"].toString());
        CuLog *log;
        if(cumbia && (log = static_cast<CuLog *>(cumbia->getServiceProvider()->get(CuServices::Log))))
        {
            static_cast<QuLogImpl *>(log->getImpl("QuLogImpl"))->showPopupOnMessage(CuLog::Write, true);
            log->write(QString("QuApplyNumeric [" + objectName() + "]").toStdString(), da["msg"].toString(), CuLog::Error, CuLog::Write);
        }
    }
    else if(d->auto_configure && da["type"].toString() == "property") {
        //
        // --------------------------------------------------------------------------------------------
        // You may want to check data format and write type and issue a warning or avoid configuration
        // at all if they are not as expected
        // if(da["data_format_str"] == "scalar" && da["writable"].toInt() > 0)
        // --------------------------------------------------------------------------------------------
        m_configure(da);
    }
    emit newData(da);
}

/** \brief write an integer to the target
 *
 * @param i the value to be written on the target
 */
void QmlWriterBackend::write(QVariant ival) {
    printf("\e[1;33mQmlWriteBackend.write: to double only!\e[0m\n");
    m_write(CuVariant(ival.toDouble()));
}

// perform the write operation on the target
//
void QmlWriterBackend::m_write(const CuVariant& v)
{
    if(!d->context)
        return;
    CuControlsWriterA *w = d->context->getWriter();
    if(w) {
        w->setArgs(v);
        w->execute();
    }
}

