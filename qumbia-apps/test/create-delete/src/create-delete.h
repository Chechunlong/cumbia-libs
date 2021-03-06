#ifndef Createdelete_H
#define Createdelete_H

#include <QWidget>

#include <qulogimpl.h>
#include <cucontrolsfactorypool.h>

class CuData;
class CumbiaPool;

class CreateDelete : public QWidget
{
    Q_OBJECT

public:
    explicit CreateDelete(CumbiaPool *cumbia_pool, QWidget *parent = 0);
    ~CreateDelete();

private slots:
    void addReaders();

    void addReader();

    void disconnectSrc();

    void deleteReader();

    void deleteMulti();

    void setPeriod();

private:

    void deleteReader(const QString& row, const QString &gb_arg);

    QStringList getDevList() const;

    CumbiaPool *cu_pool;
    QuLogImpl m_log_impl;
    CuLog *m_log;
    CuControlsFactoryPool m_ctrl_factory_pool;

    int get_random_timeout() const;

    int auto_delete_create_within_millis;

};

#endif // Create-delete_H
