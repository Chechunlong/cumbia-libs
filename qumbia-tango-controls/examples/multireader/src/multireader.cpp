#include "multireader.h"
#include "ui_multireader.h"

#include <cumbiapool.h>
#include <cumbiatango.h>
#include <cutcontrolsreader.h>
#include <cutcontrolswriter.h>
#include <cutango-world.h>
#include <cuthreadfactoryimpl.h>
#include <qthreadseventbridgefactory.h>
#include <cuserviceprovider.h>
#include <cumacros.h>

Multireader::Multireader(CumbiaTango *cut, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Multireader)
{
    cu_t = cut;
    m_log = new CuLog(&m_log_impl);
    cu_t->getServiceProvider()->registerService(CuServices::Log, m_log);

    ui->setupUi(this, cu_t, cu_tango_r_fac, cu_tango_w_fac);
}

Multireader::~Multireader()
{
    delete ui;
}
