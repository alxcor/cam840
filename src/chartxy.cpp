#include <QtGui/QtGui>
#include "chartxy.h"

ChartXY::ChartXY(QWidget *parent)
{
}

ChartXY::~ChartXY(void)
{
}

QRect	ChartXY::visualRect(const QModelIndex &index) const
  {
  return(QRect());
  }

void ChartXY::scrollTo(const QModelIndex &index,ScrollHint hint)
  {
  update();
  }

QModelIndex ChartXY::indexAt(const QPoint &point) const
  {
  return(QModelIndex());
  }

QModelIndex ChartXY::moveCursor(CursorAction cursorAction,Qt::KeyboardModifiers modifiers)
  {
  return(QModelIndex());
  }

int ChartXY::horizontalOffset() const
  {
  return(0);
  }

int ChartXY::verticalOffset() const
  {
  return(0);
  }

bool ChartXY::isIndexHidden(const QModelIndex &index) const
  {
  return(false);
  }

void ChartXY::setSelection(const QRect &rect,QItemSelectionModel::SelectionFlags command)
  {
  update();
  }

QRegion ChartXY::visualRegionForSelection(const QItemSelection &selection) const
  {
  return(QRegion());
  }

void ChartXY::dataChanged(const QModelIndex &topLeft,const QModelIndex &bottomRight)
  {
  QAbstractItemView::dataChanged(topLeft, bottomRight);
  updateChart();
  }

void ChartXY::paintEvent(QPaintEvent *event)
  {
    //variabile
    bool b_autoscale = true;
    int i, j;
    int m_border_width = 20;
    QColor m_border_color = QColor(255, 255, 255);
    QColor m_back_color = QColor(192, 192, 192);
    QColor m_graph_color[5];
    int r_graph_l, r_graph_t, r_graph_w, r_graph_h;
    QRect r_grafic;
    QPainter painter(viewport());
    QPointF punct1, punct2;
    qreal val_x, val_y, min_x, min_y, max_x, max_y;
    QPen pen_line;
    QString text;

    m_graph_color[0] = QColor(212, 45, 47);	//Strong red
    m_graph_color[1] = QColor(119,160, 71);	//Dark moderate green
    m_graph_color[2] = QColor( 70,130,180);	//Steel blue
    m_graph_color[3] = QColor(239,246,123);	//Soft yellow
    m_graph_color[4] = QColor(180, 70,130);	//Moderate Pink

    //calculez baza
    r_graph_l = m_border_width * 3;
    r_graph_t = m_border_width;
    r_graph_w = event->rect().width() - (4 * m_border_width);
    if (r_graph_w < 0)
            r_graph_w = 0;
    r_graph_h = event->rect().height() - (2 * m_border_width);
    if (r_graph_h < 0)
            r_graph_h = 0;

    painter.save();
//    painter.setRenderHint(QPainter::Antialiasing);
    painter.setFont(QFont("serif", 8, 1, false));
    painter.setPen(Qt::SolidLine);
    painter.setPen(QColor(0, 0, 0));
    //desenez baza
//    painter.fillRect(event->rect(), m_border_color);
    painter.drawRect(r_graph_l-1, r_graph_t-1, r_graph_w+1, r_graph_h+1);
    r_grafic = QRect(r_graph_l, r_graph_t, r_graph_w, r_graph_h);
//    painter.fillRect(r_grafic, m_back_color);

    //initializez valorile pe axa x
    min_x = model()->data(model()->index(0, 0)).toDouble();
    max_x = min_x;
	//calculez valorile minima si maxima pentru scalare
    for(i=0; i<model()->rowCount(); i++){
        val_x = model()->data(model()->index(i, 0)).toDouble();
        if (val_x > max_x) max_x = val_x;
        if (val_x < min_x) min_x = val_x;
    }
    //initializez valorile pe axa y
	if (fx_min_y >= fx_max_y){
		b_autoscale = true;
		min_y = model()->data(model()->index(0, 1)).toDouble();
	    max_y = min_y;
		//calculez valorile minima si maxima pentru scalare
		for (j=1; j < model()->columnCount() ; j++)
		{
			for(i=0; i<model()->rowCount(); i++){
				val_y = model()->data(model()->index(i, j)).toDouble();
				if (val_y > max_y) max_y = val_y;
				if (val_y < min_y) min_y = val_y;
			}
		}
	}
	else{
		b_autoscale = false;
		min_y = fx_min_y;
		max_y = fx_max_y;
	}
	//afisare stare ON/OFF
	painter.setFont(QFont("serif", 9, 75, false));
	if (b_ison == true){text = QString("ON");}
	else{text = QString("OFF");}
    painter.drawText(QRect(5, 5, 30, 16), text, QTextOption((Qt::AlignLeft)|(Qt::AlignCenter)));

	painter.setFont(QFont("serif", 8, 1, false));
    text = QString("%1").arg(max_y, 0, 'f', 2, '0');
    painter.drawText(QRect(5, m_border_width, m_border_width * 3 - 10, 16), text, QTextOption((Qt::AlignRight)|(Qt::AlignCenter)));
    text = QString("%1").arg(min_y, 0, 'f', 2, '0');
    painter.drawText(QRect(5, r_grafic.height()+ m_border_width - 16, m_border_width * 3 - 10, 16), text, QTextOption((Qt::AlignRight)|(Qt::AlignCenter)));

    for (j=1; j < model()->columnCount() ; j++)
    {
        pen_line.setWidth(2);
        pen_line.setColor(m_graph_color[j-1]);
        pen_line.setStyle(Qt::SolidLine);
        painter.setPen(pen_line);
        val_x = model()->data(model()->index(0, 0)).toDouble();
        val_y = model()->data(model()->index(0, j)).toDouble();
		if (b_autoscale == false){
			if (val_y < min_y){ val_y = min_y;}
			if (val_y > max_y){ val_y = max_y;}
		}
        punct1 = dataGridPosition(r_grafic, val_x, val_y, min_x, max_x, min_y, max_y);
        for(i=1; i<model()->rowCount(); i++){
            val_x = model()->data(model()->index(i, 0)).toDouble();
            val_y = model()->data(model()->index(i, j)).toDouble();
			if (b_autoscale == false){
				if (val_y < min_y){ val_y = min_y;}
				if (val_y > max_y){ val_y = max_y;}
			}
            punct2 = dataGridPosition(r_grafic, val_x, val_y, min_x, max_x, min_y, max_y);
            painter.drawLine(punct1, punct2);
            punct1 = punct2;
        }
    }

    pen_line.setWidth(1);
    pen_line.setColor(QColor(Qt::black));
    pen_line.setStyle(Qt::DotLine);
    painter.setPen(pen_line);

    //grila orizontala

    for (i=1; i<4; i++)
    {
    val_x = 0;
    val_y = (i * max_y + (4 - i) * min_y) / 4;
    punct1 = dataGridPosition(r_grafic, val_x, val_y, min_x, max_x, min_y, max_y);
    punct1.setX(r_graph_l - 5);
    punct2.setX(r_graph_l + r_graph_w);
    punct2.setY(punct1.y());
    painter.drawLine(punct1, punct2);
    text = QString("%1").arg(val_y, 0, 'f', 2, '0');
    painter.drawText(QRect(5, punct1.y() - 8, m_border_width * 3 - 10, 16), text, QTextOption((Qt::AlignRight)|(Qt::AlignCenter)));
    }

    //grila verticala
    if (r_graph_w < 200) j=4;
    else j=8;

    text = QString("%1").arg(min_x, 0, 'f', 2, '0');
    painter.drawText(QRect(r_graph_l - 25, m_border_width + r_graph_h, 50, 16), text, QTextOption((Qt::AlignHCenter)|(Qt::AlignTop)));
    text = QString("%1").arg(max_x, 0, 'f', 2, '0');
    painter.drawText(QRect(r_graph_l + r_graph_w - 25, m_border_width + r_graph_h, 50, 16), text, QTextOption((Qt::AlignHCenter)|(Qt::AlignTop)));

    for (i=1; i<j; i++)
    {
        val_y = min_y;
        val_x = (i * max_x + (j - i) * min_x) / j;
        punct1 = dataGridPosition(r_grafic, val_x, val_y, min_x, max_x, min_y, max_y);
        punct1.setY(r_graph_t - 5);
        punct2.setY(r_graph_t + r_graph_h);
        punct2.setX(punct1.x());
        painter.drawLine(punct1, punct2);
        text = QString("%1").arg(val_x, 0, 'f', 2, '0');
        painter.drawText(QRect(punct1.x()-25, m_border_width + r_graph_h, 50, 16), text, QTextOption((Qt::AlignHCenter)|(Qt::AlignTop)));
    }
    painter.restore();


}

QPointF ChartXY::dataGridPosition(QRect &rect, qreal x, qreal y, qreal min_x, qreal max_x, qreal min_y, qreal max_y)
  {

    if (max_x == min_x){
        max_x = min_x + 1;}
    if (max_y == min_y){
        max_y = min_y + 1;}

  qreal re_x1 = (qreal)rect.bottomLeft().x();
  qreal re_x2 = (qreal)rect.bottomRight().x() - re_x1;
  qreal re_y1 = (qreal)rect.topLeft().y();
  qreal re_y2 = (qreal)rect.bottomLeft().y() - re_y1;

  qreal point_x = (x-min_x)*re_x2/(max_x-min_x) + re_x1;
  qreal point_y = re_y2 - (y-min_y)*re_y2/(max_y-min_y) + re_y1;

  return(QPointF(point_x,point_y));

}

void ChartXY::resizeEvent(QResizeEvent *event)
  {
  updateGeometries();
  }

void ChartXY::scrollContentsBy(int dx, int dy)
  {
  }

void ChartXY::mousePressEvent(QMouseEvent *event)
  {
//    setCursor(Qt::CrossCursor);
  }

void ChartXY::mouseReleaseEvent(QMouseEvent *event)
  {
//  setCursor(Qt::ArrowCursor);
  }

void ChartXY::updateChart(void)
  {
//  m_setting->scale().calculateLimit(model(),m_setting->grid().horizzontalTick().tickMajor()-1,m_setting->grid().verticalTick().tickMajor()-1);
  viewport()->update();
  }
