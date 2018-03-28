/***************************************************************************
 *   Copyright (C) 2007 by Vincenzo Forchi`,,,   *
 *   vincenzo.forchi@elettra.trieste.it   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "interlock.h"
#include "Sound.h"
#include <qtango.h>
#include <QtSql>
#include <QScrollBar>
#include "mysqlmodel.h"
#include <TLog>
#include <QMessageBox>
#include <elettracolors.h>

Interlock::Interlock(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
    setupDb();
    configureUi();
    createConnections();
}

void Interlock::setupDb()
{
    /* connect to db */
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("srv-db-srf");
    db.setDatabaseName("intlkfermi");
    db.setUserName("ifermi");
    db.setPassword("ferm");
    if (!db.open())
        QMessageBox::critical(0, "Error", "Can't establish connection with db intlkboost on host log");

    queryType = QueryConfiguration::LastDay;
    modulatorModel = new MySqlModel();
    psvacModel = new MySqlModel();
    ui.treeLogs->setModel(modulatorModel);
    ui.treeLogsPSVAC->setModel(psvacModel);

    dbTimer = new QTimer(this);
    connect(dbTimer, SIGNAL(timeout()), this, SLOT(readFromDb()));
}

void Interlock::createConnections()
{
    connect(ui.pushRefresh, SIGNAL(clicked()), this, SLOT(readFromDb()));
    connect(ui.pushRefreshPSVacuum, SIGNAL(clicked()), this, SLOT(readFromDb()));
    connect(ui.pushQueryConfiguration, SIGNAL(clicked()), this, SLOT(queryConfiguration()));
    connect(ui.pushQueryConfigurationPSVAC, SIGNAL(clicked()), this, SLOT(queryConfiguration()));
    connect(ui.comboAck, SIGNAL(currentIndexChanged(int)), this, SLOT(changeAckTarget(int)));
    connect(ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

    /* 1 tab: mod filter */
    connect(ui.pbClearModFilter, SIGNAL(clicked()), ui.leFilterMod, SLOT(clear()));
    connect(ui.leFilterMod, SIGNAL(textChanged(QString)), ui.treeAlarms, SLOT(filter(QString)));

    /* 2 tab: mod history */
    connect(ui.pbClearModHist, SIGNAL(clicked()), ui.leFilterModHist, SLOT(clear()));
    connect(ui.leFilterModHist, SIGNAL(textChanged(QString)), ui.treeLogs, SLOT(filter(QString)));

    /* 3 tab: ps vac linac */
    connect(ui.pbClearFilterVacLinac, SIGNAL(clicked()), ui.leFilterPsVacLinac, SLOT(clear()));
    connect(ui.leFilterPsVacLinac, SIGNAL(textChanged(QString)), ui.treeAlarmsVPS, SLOT(filter(QString)));

    /* 4 tab: ps vacuum uh */
    connect(ui.pbClearFilterPSVacUH, SIGNAL(clicked()), ui.leFilterPsVacUH, SLOT(clear()));
    connect(ui.leFilterPsVacUH, SIGNAL(textChanged(QString)), ui.treeAlarmsVUH, SLOT(filter(QString)));

    /* 5 tab ps vacuum padres */
    connect(ui.pbClearFilterPsVacPadres, SIGNAL(clicked()), ui.leFilterPsVacPadres, SLOT(clear()));
    connect(ui.leFilterPsVacPadres, SIGNAL(textChanged(QString)), ui.treeAlarmsPadres, SLOT(filter(QString)));

    /* 6. tab ps and vacuum history */
    connect(ui.pbClearFilterPsVacHistory, SIGNAL(clicked()), ui.leFilterPsVacHistory, SLOT(clear()));
    connect(ui.leFilterPsVacHistory, SIGNAL(textChanged(QString)), ui.treeLogsPSVAC, SLOT(filter(QString)));
}

void Interlock::configureUi()
{
    QStringList headers;
    headers << "Date Time" << "Name" << "Description";
    foreach(TTreeWidget *ttw, findChildren<TTreeWidget *>(QRegExp("treeAlarms.*")))
        ttw->setHeaderLabels(headers);
    QStringList mods;
    for(int i = 0; i <= 15; i++)
        mods << QString("mod%1").arg(i, 2, 10, QChar('0'));
    mods.push_front("General");
    ui.comboAck->insertItems(0, mods);
}

Interlock::~Interlock()
{
}

void Interlock::queryConfiguration()
{
    QueryConfiguration win;
    win.setQueryType(queryType);
    if (queryType == QueryConfiguration::Interval)
    {
        win.setStartDateTime(begin);
        win.setStopDateTime(end);
    }

    if (win.exec() == QDialog::Accepted)
    {
        queryType = win.queryType();
        begin = win.startDateTime();
        end = win.stopDateTime();
        readFromDb();
    }
}

void Interlock::readFromDb()
{
    MySqlModel *model = NULL;
    QString dbnam;
    bool modulator = true;
    if(ui.tabWidget->currentIndex() == ModHistory)
    {
        model = modulatorModel;
        dbnam = "real_time_";
    }
    else if(ui.tabWidget->currentIndex() == PsVacHistory)
    {
        modulator = false;
        model = psvacModel;
        dbnam = "vac_mag_";
    }
    else
        return;

    if (!db.isOpen())
    {
        if (!db.open())
        {
            QMessageBox::critical(0, "Error", "Can't establish connection with db FastInterlock on host log");
            return;
        }
    }
    QDateTime adesso = QDateTime::currentDateTime();
    int year = adesso.date().year();

    QString query, endquery;
    if(modulator)
    {
        query = "SELECT FROM_UNIXTIME(" + dbnam + "%1.timestamp) AS 'Date Time', mod_description.name AS 'Name', "
                + dbnam + "%1.event AS 'Event', "
                "mod_description.comment AS Description, mod_description.modulator AS 'Modulator' FROM "
                + dbnam + "%1 left join mod_description on (" + dbnam +
                "%1.id_mod_description=mod_description.id_mod_description) WHERE timestamp>%2 "
                ;
        endquery = " ORDER BY timestamp desc";
    }
    else
    {
        query = "SELECT FROM_UNIXTIME(" + dbnam + "%1.timestamp) AS 'Date Time', vac_mag_desc.name AS 'Name', "
                + dbnam + "%1.event AS 'Event', "
                "vac_mag_desc.comment AS Description, vac_mag_desc.plc  AS 'PLC' FROM "
                + dbnam + "%1 left join vac_mag_desc on (" + dbnam +
                "%1.id_vac_mag_desc=vac_mag_desc.id_vac_mag_desc) WHERE timestamp>%2 "
                ;
        endquery = " ORDER BY timestamp desc";
    }


    unsigned int inizioRicerca;
    switch (queryType)
    {
    case QueryConfiguration::LastDay:
        inizioRicerca = adesso.addDays(-1).toTime_t();
        year = adesso.date().year();
        query = QString(query).arg(year).arg(inizioRicerca) + endquery;
        break;
    case QueryConfiguration::LastWeek:
        inizioRicerca = adesso.addDays(-7).toTime_t();
        year = adesso.date().year();
        query = QString(query).arg(year).arg(inizioRicerca) + endquery;
        break;
    case QueryConfiguration::LastMonth:
        inizioRicerca = adesso.addMonths(-1).toTime_t();
        year = adesso.date().year();
        query = QString(query).arg(year).arg(inizioRicerca) + endquery;
        break;
    case QueryConfiguration::Interval:
        year = end.date().year();
        query = QString(query).arg(year).arg(begin.toTime_t());
        query += QString(" AND timestamp<%1").arg(end.toTime_t()) + endquery;
        break;
    default:
        break;
    }


    printf("\e[1;33mQUERY: %s\e[0m\n", qstoc(query));

    model->setQuery(query);

    /* must apply filter if set */
    if(modulator)
        ui.treeLogs->updateFilter();
    else
        ui.treeLogsPSVAC->updateFilter();

    QSqlError error = model->lastError();
    if (error.type() != QSqlError::NoError)
    {
        QMessageBox::critical(0, "Database Error", QString("Error reading from Db:\n") + error.text() + "\nreconnecting");
        db.close();
        if (!db.open())
            QMessageBox::critical(0, "Error", "Can't establish connection with db FastInterlock on host log");
    }
}



