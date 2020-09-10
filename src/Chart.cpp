#include "Chart.h"
#include <QtCharts/QAbstractAxis>

#include <QtCharts/QSplineSeries>

#include <QtCore/QDebug>
#include <QtCore/QRandomGenerator>
#include <QtCore/QTime>

Chart::Chart(QGraphicsItem* parent, Qt::WindowFlags wFlags)
	: QChart(QChart::ChartTypeCartesian, parent, wFlags)
	, m_series(0)
	, m_axisX(new QDateTimeAxis())
	, m_axisY(new QValueAxis())
	, m_step(0)
	, m_x(QDateTime::currentDateTime())
	, m_y(0) {
	QObject::connect(&m_timer, &QTimer::timeout, this, &Chart::handleTimeout);
	m_timer.setInterval(1000);

	m_series = new QSplineSeries(this);
	QPen green(Qt::red);
	green.setWidth(3);
	m_series->setPen(green);
	m_series->append(m_x.toMSecsSinceEpoch(), m_y);

	addSeries(m_series);

	addAxis(m_axisX, Qt::AlignBottom);
	addAxis(m_axisY, Qt::AlignLeft);

	m_series->attachAxis(m_axisX);
	m_series->attachAxis(m_axisY);

	m_axisX->setTickCount(30);

	m_axisX->setRange(m_x.addSecs(-1800), m_x.addSecs(3600));

	m_axisY->setRange(0, 15);
	m_axisX->setFormat("h:mm");
	m_timer.start();
}

Chart::~Chart() { }

void Chart::handleTimeout() {
	++scroll_count;

	session_stats* stats = session_stats::get_instance();
	double val = (8 * (stats->get_total_bytes() - total_bytes)) / 1000000.0;
	total_bytes = stats->get_total_bytes();

	qreal x = plotArea().width() / m_axisX->tickCount();
	// qreal y = (m_axisX->max() - m_axisX->min()) / m_axisX->tickCount();
	m_x = m_x.addSecs(1);
	m_y = val;
	m_series->append(m_x.toMSecsSinceEpoch(), m_y);

	if(scroll_count == 180) {
		scroll(x, 0);
		scroll_count = 0;
	}
}
