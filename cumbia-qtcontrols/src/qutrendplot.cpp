#include "qutrendplot.h"
#include "quplotcommon.h"
#include <cumacros.h>
#include <cudata.h>
#include <QtDebug>
#include <QDateTime>

#include "qupalette.h"
#include "cucontrolsfactories_i.h"
#include "cucontrolsreader_abs.h"
#include "cucontext.h"
#include "quwidgetupdatestrategy_i.h"
#include "quplotbasecontextmenustrategy.h"
#include "cucontext.h"
#include "culinkstats.h"
#include "qutimescaledraw.h"
#include <quplotcurve.h>
#include <qwt_date_scale_engine.h>
#include <qwt_plot_directpainter.h>
#include <qwt_painter.h>

/** @private */
class QuTrendPlotPrivate
{
public:
    bool auto_configure, timeScaleDrawEnabled;
    bool read_ok;
    QuPlotCommon *plot_common;
    QuTimeScaleDraw *timeScaleDraw;
    QwtPlotDirectPainter *directPainter;
};

/** \brief Constructor with the parent widget, an *engine specific* Cumbia implementation and a CuControlsReaderFactoryI interface.
 *
 *  Please refer to \ref md_src_cumbia_qtcontrols_widget_constructors documentation.
 */
QuTrendPlot::QuTrendPlot(QWidget *w, Cumbia *cumbia, const CuControlsReaderFactoryI &r_fac)
    : QuPlotBase(w)
{
    d = new QuTrendPlotPrivate;
    d->plot_common = new QuPlotCommon(cumbia, r_fac);
    m_init();
}

/** \brief Constructor with the parent widget, *CumbiaPool*  and *CuControlsFactoryPool*
 *
 *   Please refer to \ref md_src_cumbia_qtcontrols_widget_constructors documentation.
 */
QuTrendPlot::QuTrendPlot(QWidget *w, CumbiaPool *cumbia_pool, const CuControlsFactoryPool &fpool)
    : QuPlotBase(w)
{
    d = new QuTrendPlotPrivate;
    d->plot_common = new QuPlotCommon(cumbia_pool, fpool);
    m_init();
}

QuTrendPlot::~QuTrendPlot()
{
    pdelete("~QuTrendPlot %p", this);
    delete d->plot_common;
    delete d;
}

void QuTrendPlot::m_init()
{
    d->auto_configure = true;
    d->read_ok = false;
    setTimeScaleDrawEnabled(true);
    setUpperBoundExtra(QwtPlot::xBottom, 0.1);
    setUpperBoundExtra(QwtPlot::xTop, 0.1);
    d->directPainter = new QwtPlotDirectPainter( this );
    setContextMenuStrategy(new QuPlotBaseContextMenuStrategy());
    if ( QwtPainter::isX11GraphicsSystem() )
        canvas()->setAttribute( Qt::WA_PaintOnScreen, true );
}

QString QuTrendPlot::source() const
{
    return sources().join(";");
}

QStringList QuTrendPlot::sources() const
{
    return d->plot_common->sources();
}

/** \brief Set the source name. Replcace currently configured sources, if any.
 *
 * Connect the plot to the source (or sources) specified.
 * More than one source can be set using a ";" separator.
 *
 * @see setSources
 *
 */
void QuTrendPlot::setSource(const QString &s)
{
    QStringList sl = s.split(";", QString::SkipEmptyParts);
    unsetSources();
    setSources(sl);
}

/** \brief Set a list of sources to display on the plot. Replace currently configured ones, if any.
 *
 * @param l a list of strings with the source names.
 */
void QuTrendPlot::setSources(const QStringList &l)
{
    unsetSources();
    d->plot_common->setSources(l, this);
}

void QuTrendPlot::addSource(const QString &s)
{
    d->plot_common->addSource(s, this);
}

void QuTrendPlot::unsetSource(const QString &s)
{
    d->plot_common->unsetSource(s);
    removeCurve(s);
}

void QuTrendPlot::setPeriod(int p)
{
    d->plot_common->getContext()->sendData(CuData("period", p));
}

void QuTrendPlot::unsetSources()
{
    foreach(CuControlsReaderA *r, getContext()->readers())
        removeCurve(r->source());
    d->plot_common->unsetSources();
}

void QuTrendPlot::onUpdate(const CuData &da)
{
    QuWidgetUpdateStrategyI *us = updateStrategy();
    if(!us)
        update(da);
    else
        us->update(da, this);
    emit newData(da);
}

void QuTrendPlot::update(const CuData &da)
{
    d->read_ok = !da["err"].toBool();
    QString src = QString::fromStdString(da["src"].toString());

    // update link statistics
    CuLinkStats *link_s = d->plot_common->getContext()->getLinkStats();
    link_s->addOperation();
    if(!d->read_ok)
        link_s->addError(da["msg"].toString());

    // configure triggers replot at the end but should not be too expensive
    // to do it once here at configuration time and once more from appendData
    if(d->read_ok && d->auto_configure && da["type"].toString() == "property")
        configure(da);


    QuPlotCurve *crv = curve(src);
    if(!crv)
        addCurve(src, crv = new QuPlotCurve(src));

    // set the curve state
    d->read_ok ? crv->setState(QuPlotCurve::Normal) : crv->setState(QuPlotCurve::Invalid);

    const CuVariant &v = da["value"];
    if(d->read_ok && v.isValid() && v.getFormat() == CuVariant::Scalar)
    {
        double x, y;
        if(da.containsKey("timestamp_ms") && crv)
            x = static_cast<qint64>(da["timestamp_ms"].toLongInt());
        else
            x = crv->size() > 0 ? crv->x(crv->size() - 1) + 1 : 0;

        v.to(y);
        appendData(src, x, y);
    }
    else {
        // appendData triggers a replot when necessary. If !d->read_ok, then there's at least
        // one curve with an Invalid state. A replot is necessary in this case
        replot();
    }
    setToolTip(QString("\"%1\": %2").arg(src).arg(QString::fromStdString(da["msg"].toString())));
}

void QuTrendPlot::setTimeScaleDrawEnabled(bool enable)
{
    d->timeScaleDrawEnabled = enable;
    /* if xAutoscale is enabled, scales are automatically adjusted at first refresh */
    if(!enable)
    {
        setAxisScaleDraw(QwtPlot::xBottom, new QwtScaleDraw());
        setAxisScaleEngine(QwtPlot::xBottom, new QwtLinearScaleEngine());
    }
    else
    {
        d->timeScaleDraw = new QuTimeScaleDraw();
        setAxisScaleDraw(QwtPlot::xBottom, d->timeScaleDraw);
        setAxisLabelRotation(QwtPlot::xBottom, -50.0);
        setAxisScaleEngine(QwtPlot::xBottom, new QwtDateScaleEngine(Qt::LocalTime));
        setAxisLabelAlignment( QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom );
    }
    /* deleting d->timeScaleDraw produces a crash */
    replot();
}

bool QuTrendPlot::timeScaleDrawEnabled()
{
    return d->timeScaleDrawEnabled;
}

void QuTrendPlot::setShowDateOnTimeAxis(bool en)
{
    if(d->timeScaleDraw)
        d->timeScaleDraw->setDateEnabled(en);
}

/** \brief Updates the marker and the scales. If the scales are not changed and
 *         the marker is not viisble, then the replot is performed incrementally.
 *
 * If a full replot is needed (a marker is visible or a scale has changed), QwtPlot::replot
 * is called. An incremental draw is performed otherwise.
 */
void QuTrendPlot::refresh()
{
    bool fullReplot = (updateMarker() || curves().size() > 1);
    fullReplot |= updateScales();
    if(fullReplot)
    {
        QwtPlot::replot();
    }
    else
    {
        foreach(QwtPlotCurve *c, this->curves())
        {
            QwtSeriesData<QPointF> *data = c->data();
            if(data->size() > 1)
                d->directPainter->drawSeries(c, data->size() - 2, data->size() -1);
        }
    }
    resetZoom();
}

void QuTrendPlot::requestLinkStats()
{
    emit linkStatsRequest(this, this);
}

bool QuTrendPlot::showDateOnTimeAxis() const
{
    return d->timeScaleDraw != NULL && d->timeScaleDraw->dateEnabled();
}

int QuTrendPlot::period() const
{
    CuData d_inout("period", -1);
    d->plot_common->getContext()->getData(d_inout);
    return d_inout["period"].toInt();
}

CuContext *QuTrendPlot::getContext() const
{
    return d->plot_common->getContext();
}

