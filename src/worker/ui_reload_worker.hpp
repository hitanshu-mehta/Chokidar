#ifndef UI_RELOAD_WORKER_HPP
#define UI_RELOAD_WORKER_HPP

#include "../session_stats.hpp"
#include "ui_mainwindow.h"

#include "callback_timer.hpp"

class ui_reload
{
	Ui::MainWindow* ui;
	callback_timer* timer;

	void update();

public:
	ui_reload(Ui::MainWindow*);
};

#endif