#ifndef QUTRENDPLOT_H
#define QUTRENDPLOT_H

#include <esimplelabel.h>
#include <cudatalistener.h>
#include "quwidgetinterface.h"
#include "quplot_base.h"

class QuTrendPlotPrivate;
class Cumbia;
class CumbiaPool;
class CuControlsFactoryPool;
class CuData;
class CuVariant;
class CuControlsReaderFactoryI;


/** \brief Draw a line for each data source over time
 *
 * QuTrendPlot is designed to accept a list of data sources.
 * Each data source is represented by a curve in the plot, which is updated according to
 * the CuData "value" and "timestamp_ms" properties.
 * If CuData "err" boolean is true or the "value" data format is not a scalar, the plot
 * is disabled.
 *
 * \par Plot update strategies.
 * Any implementation-specific data update is delegated to an update <b>strategy</b>
 * through setUpdateStrategy. If no update strategy is set, then the update method is called
 * from within onUpdate and the base operations aforementioned are performed.
 * <em>Strategies</em> must implement QuWidgetUpdateStrategyI interface.
 */
class QuTrendPlot :
        public QuPlotBase,
        public CuDataListener
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource DESIGNABLE true)
    Q_PROPERTY(int period READ period WRITE setPeriod DESIGNABLE true)

public:
    QuTrendPlot(QWidget *w, Cumbia *cumbia, const CuControlsReaderFactoryI &r_fac);

    QuTrendPlot(QWidget *w, CumbiaPool *cumbia_pool, const CuControlsFactoryPool &fpool);

    virtual ~QuTrendPlot();

    QString source() const;

    QStringList sources() const;

    bool timeScaleDrawEnabled();

    bool showDateOnTimeAxis() const;

    void setDefaultBounds(const CuVariant &lb, const CuVariant &ub, Axis axisId = QwtPlot::yLeft);

    int period() const;

public slots:

    void setSource(const QString& s);

    void setSources(const QStringList& l);

    void addSource(const QString& s);

    void unsetSource(const QString& s);

    void unsetSources();

    void setPeriod(int p);

    void sendData(const CuData &d);

    void setTimeScaleDrawEnabled(bool enable);

    void setShowDateOnTimeAxis(bool en);

    void refresh();

signals:
    void newData(const CuData&);


private:
    QuTrendPlotPrivate *d;

    void m_init();

    // CuTangoListener interface
public:
    void onUpdate(const CuData &d);

    // QuWidgetInterface interface
    void update(const CuData &d);

};

#endif // QUTLABEL_H
