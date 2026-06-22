//#pragma once
//#include "b:\release\siemens\sinumerik\hmi\osal\qt\src\gui\itemviews\qabstractitemview.h"
#ifndef CHARTXY_H
#define CHARTXY_H

#include <QtGui/QAbstractItemView>
#include <QtGui/QStandardItemModel>
#include <QtCore/QtCore>
#include <QtGui/QWidget>

class ChartXY :
	public QAbstractItemView
{
public:
	ChartXY(QWidget *parent = 0);
	~ChartXY(void);

    virtual QRect       visualRect(const QModelIndex &index) const;
    virtual void        scrollTo(const QModelIndex &index,ScrollHint hint=EnsureVisible);
    virtual QModelIndex indexAt(const QPoint &point) const;

	qreal	fx_min_y, fx_max_y;
	bool	b_ison;

protected:
    virtual QModelIndex moveCursor(CursorAction cursorAction,Qt::KeyboardModifiers modifiers);
    virtual int         horizontalOffset() const;
    virtual int         verticalOffset() const;
    virtual bool        isIndexHidden(const QModelIndex &index) const;
    virtual void        setSelection(const QRect &rect,QItemSelectionModel::SelectionFlags command);
    virtual QRegion     visualRegionForSelection(const QItemSelection &selection) const;
    virtual void        dataChanged(const QModelIndex &topLeft,const QModelIndex &bottomRight);

    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void scrollContentsBy(int dx, int dy);

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

protected slots:
    QPointF dataGridPosition(QRect &rect, qreal x, qreal y, qreal min_x, qreal max_x, qreal min_y, qreal max_y);

signals:

public slots:
    void updateChart(void);

};

#endif
