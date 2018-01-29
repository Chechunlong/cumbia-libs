#include "quapplynumeric.h"
#include <cumacros.h>
#include <cudata.h>
#include <cumbia.h>
#include <cuserviceprovider.h>

#include "cucontrolswriter_abs.h"
#include "cucontrolsfactories_i.h"
#include "cucontrolsutils.h"
#include "cumbiapool.h"
#include "cucontext.h"
#include "qulogimpl.h"

class QuApplyNumericPrivate
{
public:
    CuContext *context;
    bool auto_configure;
    bool write_ok;
    CuLog *log;
};

/** \brief Constructor with the parent widget, an *engine specific* Cumbia implementation and a CuControlsWriterFactoryI interface.
 *
 *  Please refer to \ref md_src_cumbia_qtcontrols_widget_constructors documentation.
 */
QuApplyNumeric::QuApplyNumeric(QWidget *parent, Cumbia *cumbia, const CuControlsWriterFactoryI &w_fac)
    : EApplyNumeric(parent)
{
    m_init();
    d->context = new CuContext(cumbia, w_fac);
}

/** \brief Constructor with the parent widget, *CumbiaPool*  and *CuControlsFactoryPool*
 *
 *   Please refer to \ref md_src_cumbia_qtcontrols_widget_constructors documentation.
 */
QuApplyNumeric::QuApplyNumeric(QWidget *parent, CumbiaPool *cumbia_pool, const CuControlsFactoryPool &fpool)
    : EApplyNumeric(parent)
{
    m_init();
    d->context = new CuContext(cumbia_pool, fpool);
}

QuApplyNumeric::~QuApplyNumeric()
{
    delete d->context;
    delete d;
}

QString QuApplyNumeric::targets() const
{
    CuControlsWriterA *w = d->context->getWriter();
    if(w != NULL)
        return w->targets();
    return "";
}

void QuApplyNumeric::execute()
{
    cuprintf("QuApplyNumeric.execute\n");
    CuControlsUtils cu;
    CuVariant args = cu.getArgs(targets(), this);
    printf("QuApplyNumeric.execute: got args %s type %d format %d\n", args.toString().c_str(), args.getType(),
           args.getFormat());
    CuControlsWriterA *w = d->context->getWriter();
    if(w)
    {
        w->setArgs(args);
        w->execute();
    }
}

void QuApplyNumeric::setTargets(const QString &targets)
{
    printf("\e[1;32mQuApplyNumeric.setTargets!!!!! %s\e[0m\n", qstoc(targets));
    CuControlsWriterA* w = d->context->replace_writer(targets.toStdString(), this);
    if(w) w->setTargets(targets);
}

void QuApplyNumeric::execute(double val)
{
    cuprintf("QuApplyNumeric.execute\n");
    CuVariant args(val);
    printf("QuApplyNumeric.execute: got args %s type %d format %d\n", args.toString().c_str(), args.getType(),
           args.getFormat());
    CuControlsWriterA *w = d->context->getWriter();
    if(w)
    {
        w->setArgs(args);
        w->execute();
    }
}

void QuApplyNumeric::m_init()
{
    printf("\e[1;32mQuApplyNumeric> initializing\e[0m\n");
    d = new QuApplyNumericPrivate;
    connect(this, SIGNAL(clicked(double)), this, SLOT(execute(double)));
    d->auto_configure = true;
    d->write_ok = false;
}

void QuApplyNumeric::onUpdate(const CuData &da)
{
    if(da["err"].toBool())
    {
        perr("QuApplyNumeric [%s]: error %s target: \"%s\" format %s (writable: %d)", qstoc(objectName()),
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
    else if(d->auto_configure && da["type"].toString() == "property")
    {
        QString desc = "";
        if(da["data_format_str"] == "scalar" && da["writable"].toInt() > 0)
        {
            /* first apply format, if - correctly - specified */
            CuVariant m, M;
            m = da["min"];
            M = da["max"];
            std::string print_format = da["format"].toString();
            double min, max;
            bool ok;
            ok = m.to<double>(min);
            if(ok)
                ok = M.to<double>(max);
            if(ok)
            {
                configureNumber(min, min, QString::fromStdString(print_format));
                /* integerDigits() and decimalDigits() from NumberDelegate */
                setIntDigits(integerDigits());
                setDecDigits(decimalDigits());
                setMaximum(max);
                setMinimum(min);
                desc = "\n(min: "+ QString("%1").arg(min) + " max: "+ QString("%1").arg(max) + ")";
            }
            else
                pinfo("QuApplyNumeric: maximum and minimum values not set on the tango attribute \"%s\", object \"%s\": "
                      "not setting format nor maximum/minimum", qstoc(targets()), qstoc(objectName()));
            /* can set current values instead */
            double val;
            bool can_be_double = da["w_value"].to<double>(val);
            if (can_be_double)
            {
                setValue(val);
                clearModified();
            }
            if(!da["description"].isNull()) {
                desc.prepend(QString::fromStdString(da["description"].toString()));
            }
            setWhatsThis(desc);
        }
        else
            perr("QuApplyNumeric [%s]: invalid data format \"%s\" or read only source (writable: %d)", qstoc(objectName()),
                 da["data_format_str"].toString().c_str(), da["writable"].toInt());

    }
}

CuContext *QuApplyNumeric::getContext() const
{
    return d->context;
}
