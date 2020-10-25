#ifndef BAR_CHART
#define BAR_CHART

#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

#include <vector>

QT_CHARTS_BEGIN_NAMESPACE
class QBarSeries;
class QValueAxis;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class bar_chart : public QChart
{

private:
	std::vector<QBarSet*> bars;
	std::vector<QBarSeries*> series;
	std::string chart_title;
	QStringList categories;

	QStringList titles;
	QValueAxis* axisX;
	QValueAxis* axisY;

public:
	// bar_chart(std::vector<pair<std::string, int>>);
	virtual ~bar_chart();
};

#endif