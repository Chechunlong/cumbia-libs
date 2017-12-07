#ifndef CUCONTEXTACTIONBRIDGE_H
#define CUCONTEXTACTIONBRIDGE_H

#include <QObject>

class CuLinkStatsConnectorPrivate;
class QPoint;
class CuContextWidgetI;
class Cumbia;
class CumbiaPool;
class CuControlsReaderFactoryI;
class CuControlsFactoryPool;

class CuContextActionBridge : public QObject
{
    Q_OBJECT
public:
    explicit CuContextActionBridge(QWidget *parent, Cumbia* cumbia, const CuControlsReaderFactoryI &r_fac);

    explicit CuContextActionBridge(QWidget *parent, CumbiaPool *cumbia_pool, const CuControlsFactoryPool &fpool);

    virtual ~CuContextActionBridge();

    void connectObject(QObject *w);

private slots:
    void onLinkStatsRequest(QWidget *sender, CuContextWidgetI *);

private:
    CuLinkStatsConnectorPrivate *d;

    void m_connect();
};

#endif // CULINKSTATSCONNECTOR_H