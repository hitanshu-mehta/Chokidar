#ifndef UI_RELOAD_WORKER_HPP
#define UI_RELOAD_WORKER_HPP

#include "../session_stats.hpp"
#include "ui_mainwindow.h"
#include <QtCore/QTimer>

#include "callback_timer.hpp"

class ui_reload : public QObject
{
	Q_OBJECT
	QTimer m_timer;
	Ui::MainWindow* ui;
	callback_timer* timer;
	int g_timer = 0;
	long no_pkts = 0;
	long total_bytes = 0;
	void reload_protocol_graphs(session_stats*);
	void reload_ip_graphs(session_stats*);

public slots:
	void update();

public:
	virtual ~ui_reload();
	ui_reload(Ui::MainWindow*);
};

#endif