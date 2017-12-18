#include "quappdbuscontroller.h"
#include "quappdbus.h"
#include "cumbiadbus_interface.h"
#include <QDBusReply>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusInterface>
#include <QList>
#include <cumacros.h>

#define SERVICE_NAME "eu.elettra.quappdbus.controller"

class QuAppDBusControllerPrivate
{
public:
    QDBusConnectionInterface *dbus_if;
    QList<QuAppDBusControllerListener *> ctrl_listeners;
};

QuAppDBusController::QuAppDBusController(QObject *parent) : QObject(parent)
{
    d = new QuAppDBusControllerPrivate;
    d->dbus_if  = NULL;
    d->dbus_if = m_getDbusConnectionInterface();
}

QuAppDBusController::~QuAppDBusController()
{
    delete d;
}

QStringList QuAppDBusController::args_noPath(const QStringList &args) const
{
    QStringList ret;
    if(args.size() > 0)
    {
        QString exe = args.first();
        if(exe.count('/') > 0)
            exe = exe.split('/', QString::SkipEmptyParts).last();
        ret << exe;
    }
    for(int i = 1; i < args.size(); i++)
        ret << args[i];
    return ret;
}

/** Find information about a cumbia-qtcontrols QuApplication currently in execution with the given arguments
 *
 * @param args a QStringList of arguments. The first argument must be the executable name, without the
 *        path. The following arguments are argv[1], argv[2], ...
 *        If args is an empty list, then all applications registered as <em>quapplication</em> in the
 *        session bus are returned.
 *
 * @return a list of QuAppInfo with the list of applications matching args
 *
 */
QList<QuAppInfo>  QuAppDBusController::findApps(const QStringList &args)
{
    qDebug() << __FUNCTION__ << "finding by args " << args;
    QList<QuAppInfo>  il;

    if(!d->dbus_if)
        return il;

    QDBusReply<QStringList> services = d->dbus_if->registeredServiceNames();
    if(services.isValid())
    {
        /* populate the refreshed information list */
        foreach(QString service, services.value())
        {
            qDebug() << __FUNCTION__ << service;
            if(service.contains("quapplication"))
            {
                EuElettraQudbusQuAppDBusInterfaceInterface quappdbusi(service,  "/QuApplication", QDBusConnection::sessionBus(), this);
                if(quappdbusi.isValid())
                {
                    /* retrieve pid from service */
                    QDBusPendingReply<QStringList> argsReply = quappdbusi.arguments();
                    if(argsReply.isValid())
                        perr("QuAppDBusController.findApps: reply error: %s", qstoc(argsReply.error().message()));
                    else
                    {
                        QStringList arguments = args_noPath(argsReply.value());
                        if(args.isEmpty() || arguments == args)
                        {
                            QuAppInfo ai(arguments);
                            ai.dbus_servicename = service;
                            il.append(ai);
                        }
                    }
                }
                else
                    qDebug() << __FUNCTION__ << "interface " << service << "is not valid";
            }
        }
    }
    return il;
}

int QuAppDBusController::closeAll(const QList<QuAppInfo> &ail)
{
    int ret = 0;
    QDBusConnection connection = QDBusConnection::sessionBus();
    if(connection.isConnected())
    {
        foreach(QuAppInfo ai, ail)
        {
            if(connection.isConnected())
            {
                EuElettraQudbusQuAppDBusInterfaceInterface quappdbusi(ai.dbus_servicename,  "/QuApplication", QDBusConnection::sessionBus(), this);
                quappdbusi.quit();
                ret++;
            }
        }
    }
    else
        perr("QuAppDBusController.closeAll: failed to connect to session bus: %s %s",
             qstoc(connection.lastError().name()), qstoc(connection.lastError().message()));
    return ret;
}

void QuAppDBusController::start_monitor(const QString &serviceName)
{
    EuElettraQudbusQuAppDBusInterfaceInterface *quappdbusi =
            new EuElettraQudbusQuAppDBusInterfaceInterface(serviceName,  "/QuApplication", QDBusConnection::sessionBus(), this);
    connect(quappdbusi, SIGNAL(dbusRegistered(QString,QStringList,QString)), this,
            SLOT(onAppRegistered(QString,QStringList,QString)));
    connect(quappdbusi, SIGNAL(dbusUnregistered(QString,QStringList,QString)), this,
            SLOT(onAppUnregistered(QString,QStringList,QString)));
    quappdbusi->setObjectName(serviceName);
    qDebug() << __FUNCTION__ << "mapped signals to monitor " << serviceName;
}

void QuAppDBusController::stop_monitor(const QString &serviceName)
{
    EuElettraQudbusQuAppDBusInterfaceInterface *quappdbusi = findChild<EuElettraQudbusQuAppDBusInterfaceInterface *>(serviceName);
    if(quappdbusi)
    {
        qDebug() << __FUNCTION__ << "deleting monitored interface " << quappdbusi->objectName();
        delete quappdbusi;
    }
}

void QuAppDBusController::addCtrlListener(QuAppDBusControllerListener *l)
{
    d->ctrl_listeners.append(l);
}

void QuAppDBusController::removeCtrlListener(QuAppDBusControllerListener *l)
{
    d->ctrl_listeners.removeAll(l);
}

void QuAppDBusController::onAppRegistered(const QString &exenam, const QStringList &args, const QString &dbus_servicenam)
{
    qDebug() << __FUNCTION__ << exenam << args << dbus_servicenam;
    QuAppInfo ai(exenam, args, dbus_servicenam);
    foreach(QuAppDBusControllerListener *l, d->ctrl_listeners)
        l->onAppRegistered(ai);
}

void QuAppDBusController::onAppUnregistered(const QString &exenam, const QStringList &args, const QString &dbus_servicenam)
{
    qDebug() << __FUNCTION__ << exenam << args << dbus_servicenam;
    QuAppInfo ai(exenam, args, dbus_servicenam);
    foreach(QuAppDBusControllerListener *l, d->ctrl_listeners)
        l->onAppUnregistered(ai);
}

QDBusConnectionInterface *QuAppDBusController::m_getDbusConnectionInterface()
{
    if(d->dbus_if)
        return d->dbus_if;

    QDBusConnection connection = QDBusConnection::sessionBus();

    if(!connection.registerService(SERVICE_NAME))
        perr("QuAppDBusController: failed to register service \"%s\": %s: %s", SERVICE_NAME, qstoc(connection.lastError().name()), qstoc(connection.lastError().message()));

    else if(!connection.registerObject("/QuAppDBusController", this))
        perr("QuAppDBusController: failed to register object with path \"%s\" %s: %s:", SERVICE_NAME, qstoc(connection.lastError().name()), qstoc(connection.lastError().message()));

    if(connection.isConnected())
        d->dbus_if = connection.interface();

    return d->dbus_if;
}