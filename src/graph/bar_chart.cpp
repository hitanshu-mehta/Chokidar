#include "bar_chart.hpp"

#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSeries>

#include <QtCharts/QLegend>
#include <QtCharts/QValueAxis>
#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>

// bar_chart::bar_chart(std::vector<pair<std::string, int>> name_vals) {
// 	for(auto el : name_vals) {
// 		bars.push_back(new QBarSet(el.first));
// 		bars.back()->append(el.second);

// 		series.push_back(new QBarSeries());
// 		series.back()->append(bars.back());
// 	}

// 	for(auto el : series) }

bar_chart::~bar_chart() { }
