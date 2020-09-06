#ifndef UI_RELOAD_WORKER_HPP
#define UI_RELOAD_WORKER_HPP

#include "../session_stats.hpp"
#include "ui_mainwindow.h"

#include "callback_timer.hpp"

class ui_reload
{
	Ui::MainWindow* ui;
	callback_timer* timer;

	long no_pkts = 0;
	long total_bytes = 0;
	void update();

public:
	ui_reload(Ui::MainWindow*);
};

#endif