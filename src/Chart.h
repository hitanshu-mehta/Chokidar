
#ifndef CHART_H
#define CHART_H

#include <QtCharts/QChart>
#include <QtCore/QTimer>

QT_CHARTS_BEGIN_NAMESPACE
class QSplineSeries;
class QValueAxis;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

#include "session_stats.hpp"

#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QtCore/QDateTime>

class Chart : public QChart
{
	Q_OBJECT
public:
	Chart(QGraphicsItem* parent = nullptr, Qt::WindowFlags wFlags = {});
	virtual ~Chart();

public slots:
	void handleTimeout();

private:
	long total_bytes = 0;
	QTimer m_timer;
	QSplineSeries* m_series;
	QStringList m_titles;
	QValueAxis* m_axisY;
	QDateTimeAxis* m_axisX;
	qreal m_step;
	QDateTime m_x;
	qreal m_y;
	int scroll_count = 0;
};

#endif /* CHART_H */