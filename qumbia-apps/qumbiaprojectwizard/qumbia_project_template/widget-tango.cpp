#include "$HFILE$"
#include "ui_$HFILE$"

#include <cumbiapool.h>
#include <cumbiatango.h>
#include <cutcontrolsreader.h>
#include <cutcontrolswriter.h>
#include <cutango-world.h>
#include <cuthreadfactoryimpl.h>
#include <qthreadseventbridgefactory.h>
#include <cuserviceprovider.h>
#include <cucontextactionbridge.h>
#include <cumacros.h>

$MAINCLASS$::$MAINCLASS$(CumbiaTango *cut, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::$MAINCLASS$)
{
    cu_t = cut;
    m_log = new CuLog(&m_log_impl);
    cu_t->getServiceProvider()->registerService(CuServices::Log, m_log);

    ui->setupUi(this, cu_t, cu_tango_r_fac, cu_tango_w_fac);


    // after all cumbia widgets have been instantiated
    // if you add widgets later, you can use the connectObject(QObject *w)
    // method from CuContextActionBridge.
    new CuContextActionBridge(this, cu_t, cu_tango_r_fac);
}

$MAINCLASS$::~$MAINCLASS$()
{
    delete ui;
}