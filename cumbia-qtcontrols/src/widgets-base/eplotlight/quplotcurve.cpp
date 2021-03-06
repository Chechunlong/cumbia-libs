#include "quplotcurve.h"
#include <QtDebug>
#include <cumacros.h>
#include <QVariant>
#include <qwt_plot.h>
#include <qwt_scale_div.h>
#include <qwt_painter.h>
#include <QPainter>
#include <elettracolors.h>
#include <qwt_plot_canvas.h>
#include <QPolygon>
#include <qwt_symbol.h>

#define MAX_STATE_SYMBOLS 6
#define STATE_SYMBOL_SIZE 14
#define YLABEL_OFFSET 3

class EPlotCurvePrivate
{
public:
    int bufSize; /* no more needed: managed by EPlotLight */
    CurveData *data;
    QuPlotCurve::State state;
    int max_data_size_for_symbol;
    // keep a boolean to test if symbol is enabled instead of making a call to
    // QwtPlotCurve::symbol
    bool symbol;
};

QuPlotCurve::QuPlotCurve() :  QwtPlotCurve()
{
    init();
}

QuPlotCurve:: QuPlotCurve(const QwtText &title) : QwtPlotCurve(title)
{
    init();
}

QuPlotCurve:: QuPlotCurve(const QString &title) : QwtPlotCurve(title)
{
    init();
}

void QuPlotCurve::init()
{
    d = new EPlotCurvePrivate;
    d->data = NULL;
    d->data = new CurveData();
    d->bufSize = -1;
    d->state = Normal;
    d->symbol = false;
    d->max_data_size_for_symbol = 5;
}

void QuPlotCurve::m_setSymbol(size_t dataSiz)
{
    QwtSymbol *sym = NULL;
    if(dataSiz <= d->max_data_size_for_symbol) {
        if(!d->symbol) {
            QPen pe(pen());
            pe.setWidthF(1.45);
            sym = new QwtSymbol(QwtSymbol::XCross);
            sym->setPen(pe);
            sym->setSize(10);
            setSymbol(sym);
            d->symbol = true;
        }
    }
    else  if(d->symbol) {
        setSymbol(NULL);
        d->symbol = false;
    }
}

QuPlotCurve::~QuPlotCurve()
{
    if(d->data)
        delete d->data;
    delete d;
}

void QuPlotCurve::setData(const QVector< double > &xData, const QVector< double > &yData)
{
    m_setSymbol(xData.size());
//    d->data->set(xData, yData);
    QwtPlotCurve::setSamples(xData, yData);
}

void QuPlotCurve::appendData(double *x, double *y, int count)
{
    /* add count elements */
    m_setSymbol(data()->size());
    d->data->append(x, y, count);
}

void QuPlotCurve::popFront()
{
    if(d->data->count() > 0)
        d->data->removeFirstElements(1);
}

void QuPlotCurve::updateRawData()
{
    setRawSamples(d->data->x(), d->data->y(), d->data->count());
}

void QuPlotCurve::clearData()
{
    d->data->removeFirstElements(d->data->count());
    QwtPlotCurve::setSamples(QVector<double>(), QVector<double>());
}

int QuPlotCurve::count() const { return d->data->count(); }

QuPlotCurve::State QuPlotCurve::state() const { return d->state; }

void QuPlotCurve::setState(QuPlotCurve::State s) { d->state = s; }

/** \brief one of the drawing methods.
  *
  * This method is invoked to draw the curve from the point from to the point to.
  * When data is appended, the method is normally called with from and to reflecting the distance
  * between the point added and the previous point.
  * When from == 0 and to == -1, then a full redraw is requested.
  * When the state of the curve is Invalid, a certain number of markers, according to the value
  * of the defined MAX_STATE_SYMBOLS, is drawn. In the case of the invalid state, all the symbols
  * are redrawn on the full curve, whether a full draw is requested or not.
  * On the other side, when d->showYValuesEnabled is true, only the requested labels are drawn,
  * according to the value of from and to, eventually all the labels in case of from == 0 and to == -1.
  */
void QuPlotCurve::drawCurve(QPainter *p, int style,
                            const QwtScaleMap &xMap,
                            const QwtScaleMap &yMap,
                            const QRectF &canvasRect,
                            int from, int to) const
{
    double y = 0.0, x = 0.0;
    if(d->state == Invalid)
    {
        printf("drawCurve: invalid state drawing\n");
        /* always (re)draw all the markers, because the number of them is limited to
         * MAX_STATE_SYMBOLS.
         */
        int step;
        QColor errColor(KDARKRED);
        errColor.setAlpha(160);
        QPen errPen(errColor);
        QFont f = p->font();
        f.setPixelSize(0.8  * STATE_SYMBOL_SIZE);
        p->save();
        p->setFont(f);

        /* draw an error message */
        QString errMsg = "Invalid data or read error on one (or more) curves.";
        QPointF bottomRight = canvasRect.bottomRight();
        QFontMetrics fm(f);
        QRect errMsgRect(QPoint(bottomRight.x() - fm.width(errMsg),
                                bottomRight.y() - 2 * fm.height()), QSize(fm.width(errMsg), fm.height()));
        p->setPen(errPen);
        QwtPainter::fillRect(p, errMsgRect, Qt::white);
        QwtPainter::drawText(p, errMsgRect, Qt::AlignHCenter|Qt::AlignCenter, errMsg);
        QString errMsg2 = "Right click and select \"Info...\" for details.";
        errMsgRect.moveTop(errMsgRect.top() + fm.height());
        errMsgRect.setWidth(fm.width(errMsg2));
        QwtPainter::fillRect(p, errMsgRect, Qt::white);
        QwtPainter::drawText(p, errMsgRect, Qt::AlignHCenter|Qt::AlignCenter, errMsg2);

        if(count() / MAX_STATE_SYMBOLS > 0)
            step = count() / MAX_STATE_SYMBOLS;
        else
            step = 1;
        int i = 1;
        while(i * step < count())
        {
            x = data()->sample(i * step).x();
            y = data()->sample(i * step).y();
            QRect rect(xMap.transform(x), yMap.transform(y) - STATE_SYMBOL_SIZE / 2,
                       STATE_SYMBOL_SIZE, STATE_SYMBOL_SIZE);
            QPolygon poly(QVector<QPoint>() << rect.bottomLeft() << QPoint(rect.left() + rect.width()/2, rect.top())
                          << rect.bottomRight());
            p->setPen(Qt::black);
            QwtPainter::drawText(p, rect, Qt::AlignHCenter|Qt::AlignCenter, "!");
            errPen.setColor(KRED);
            p->setPen(errPen);
            QwtPainter::drawPolygon(p, poly);
            i++;
        }
        p->restore();
    }

//    printf("\e[1;33mNOTE\e[0m: calling drawCurve %s from %d to %d!\n", qstoc(title().text()), from, to);
    QwtPlotCurve::drawCurve(p, style, xMap, yMap, canvasRect, from, to);
}

double QuPlotCurve::x(int index) const
{
    return this->data()->sample(index).x();
}

double QuPlotCurve::y(int index) const
{
    return this->data()->sample(index).y();
}

size_t QuPlotCurve::size() const
{
    return this->data()->size();
}



