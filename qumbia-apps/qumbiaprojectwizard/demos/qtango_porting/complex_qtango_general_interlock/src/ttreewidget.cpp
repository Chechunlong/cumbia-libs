#include <QtDebug>

#include <QMenu>
#include <QContextMenuEvent>
#include <QHeaderView>
#include <QDateTime>
#include "ttreewidget.h"
#include <elettracolors.h>
#include <QScrollBar>
#include "treeitem.h"

#define DECOLOR_COUNT_MAX 10
#define DECOLOR_INTERVAL 500 /* milliseconds of decoloring effect */

TTreeWidget::TTreeWidget(QWidget *p) : QTreeWidget(p), QTangoComProxyReader(this), QTangoWidgetCommon(this), _dataSize(0)
{
	setAutoConfiguration(true);
	setDesiredAttributePropertyList(QStringList() << "labels" << "positions");
    header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    setSortingEnabled(true);
    sortByColumn(0);
	connect(qtangoComHandle(), SIGNAL(newData(const TVariant&)), this, SLOT(refresh(const TVariant&)),
	  Qt::DirectConnection);
	connect(qtangoComHandle(), SIGNAL(attributeAutoConfigured(const TangoConfigurationParameters *)), this, SLOT(configure(const 		TangoConfigurationParameters *)));  

    tmpCounter = 1;

}

void TTreeWidget::configure(const TangoConfigurationParameters *cp)
{
   if(cp->propertyFound("labels"))
	_labels = cp->attributePropertyValue("labels");
   
   if(cp->propertyFound("positions"))
      _descriptions = cp->attributePropertyValue("positions");
}

void TTreeWidget::refresh(const TVariant& v)
{
    /* save scrollbar value */
    int scrollPos = this->verticalScrollBar()->value();
    /* first time do not emit signal for sound */
    bool alarmPresent;
//	setToolTip(getMessage());
	if (v.quality() == ATTR_INVALID)
	{
		setDisabled(true);
         }
/*	else if (canConvertToBoolVector())
	{
		setDisabled(false);
		clear();
		vector<bool>	data = getDataAsBoolVector();
		QList<QTreeWidgetItem *> items;
		for (unsigned int i = 0; i < data.size(); i++)
		{
			QTreeWidgetItem *item = new QTreeWidgetItem((QTreeWidget*) 0, QStringList(_labels.value(i)));
     			items.append(item);
		}
		insertTopLevelItems(0, items);
	}*/
	else if (v.canConvertToDoubleVector())
        {
		setDisabled(false);
        QVector<double>	data = v.toDoubleVector();
    //    qSort(data.begin(), data.end());

		if (data == oldData)
			return;

        qDebug() << __FUNCTION__ << data << data.size();

        double lastTimestamp = data.last();
        double previousLastTimestamp;
        if(oldData.isEmpty()) /* first refresh, no matter if visible or not */
        {
            previousLastTimestamp = QDateTime::currentDateTime().toTime_t();
            qDebug() << "setting last timestamp seen in first time " << objectName() <<
                        previousLastTimestamp << QDateTime::currentDateTime();
            mLastTimestampSeen = previousLastTimestamp;
        }
        else
            previousLastTimestamp = oldData.last();

        alarmPresent = lastTimestamp > previousLastTimestamp && !oldData.isEmpty();
        if(lastTimestamp > previousLastTimestamp)
            mAlarmColor = KRED;

        /* now we can update oldData, sorted */
		oldData = data;
		clear();
		_dataSize = data.size();


		QList<QTreeWidgetItem *> items;

        for (int i = data.size() - 1; i >= 0; i--)
		{
			if (data[i])
			{
				QStringList cells;
				QDateTime date;
                date.setTime_t((int) data[i]);
				date = date.addMSecs((int)(1000 *(data[i] - (int) data[i])));
				cells << date.toString("d MMM yyyy hh:mm:ss.zzz") << _labels.value(i) << _descriptions.value(i) << QString().setNum(i);
                                TreeItem *item = new TreeItem(this, cells);
                                item->setData(0, Qt::UserRole, data[i]);
                items.append(item);
                if(data[i] > mLastTimestampSeen) /* red colour new alarms */
                {
                    QFont f = item->font(0);
                    f.setBold(true);
                    for(int i = 0; i < item->columnCount(); i++)
                        item->setFont(i, f);
                }
			}
		}
		insertTopLevelItems(0, items);

        if(alarmPresent)
        {
            emit newAlarmArrived();
        }
	}
    filter(mCurrentFilter);

    /* restore last scroll position */
    if(scrollPos != this->verticalScrollBar()->value())
        this->verticalScrollBar()->setValue(scrollPos);
}

void TTreeWidget::filter(const QString& text)
{
    mCurrentFilter = text;
    QList<QTreeWidgetItem *> items = this->findItems("*", Qt::MatchWildcard, 0);
    if(text.isEmpty()) /* show all items */
    {
        foreach(QTreeWidgetItem* it, items)
            if(it->isHidden())
                it->setHidden(false);
    }
    else
    {
        foreach(QTreeWidgetItem* it, items)
        {
            it->setHidden(!it->text(1).contains(text, Qt::CaseInsensitive));
        }
    }
}

void TTreeWidget::ackSelected()
{
	emit acknowledgeSelected();
}

void TTreeWidget::ackAll()
{
	emit acknowledgeAll();
}

void TTreeWidget::contextMenuEvent(QContextMenuEvent *event)
{
        QMenu *menu = new QMenu(this);
        menu->addAction("Acknowledge Selected", this, SLOT(ackSelected()));
        menu->addAction("Acknowledge All", this, SLOT(ackAll()));
        menu->exec(event->globalPos());
}

void TTreeWidget::hideEvent(QHideEvent *e)
{
    QTreeWidget::hideEvent(e);
    mLastTimestampSeen = QDateTime::currentDateTime().toTime_t();
    qDebug() << "hide event: last timestamp seen for " << objectName() << QDateTime::currentDateTime() << mLastTimestampSeen;
}

void TTreeWidget::showEvent(QShowEvent *e)
{
    QTreeWidget::showEvent(e);
    mLastTimestampSeen = QDateTime::currentDateTime().toTime_t();
    qDebug() << "show event : last timestamp seen for " << objectName() << QDateTime::currentDateTime() << mLastTimestampSeen;

}


