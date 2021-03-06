#ifndef QUACTIONEXTENSIONPLUGININTERFACE_H
#define QUACTIONEXTENSIONPLUGININTERFACE_H

#include <QObject>
#include <QString>
#include <cudata.h>
#include <vector>

class Cumbia;
class CumbiaPool;
class CuControlsReaderFactoryI;
class CuControlsFactoryPool;
class QString;
class QStringList;
class CuContext;

class QuActionExtensionI {
  public:

    virtual ~QuActionExtensionI() {}

    virtual QString getName() const = 0;

    virtual CuData execute(const CuData& in) = 0;

    virtual std::vector<CuData> execute(const std::vector<CuData>& in_list) = 0;

    virtual QObject* get_qobject() = 0;

    virtual const CuContext *getContext() const = 0;

    virtual std::string message() const = 0;

    virtual bool error() const = 0;
};

class QuActionExtensionFactoryI
{
public:
    virtual ~QuActionExtensionFactoryI() {}

    virtual QuActionExtensionI *create(const QString& name, const CuContext *ctx) = 0;
};

/** \brief Interface for a plugin
 *
 */
class QuActionExtensionPluginInterface
{
public:
    virtual ~QuActionExtensionPluginInterface() { }

    virtual void registerExtension(const QString& name, QuActionExtensionI *ae) = 0;

    virtual QStringList extensions() const = 0;

    virtual QuActionExtensionI *getExtension(const QString& name) = 0;

    virtual QuActionExtensionFactoryI *getExtensionFactory() = 0;
};

#define QuActionExtensionPluginInterface_iid "eu.elettra.qutils.QuActionExtensionPluginInterface"

Q_DECLARE_INTERFACE(QuActionExtensionPluginInterface, QuActionExtensionPluginInterface_iid)

#endif // QUACTIONEXTENSIONPLUGININTERFACE_H
