#pragma once
#include "countdown_timer_ui.h"
#include "timer.h"

void warm_up_select_clicked(void);
void warm_up_time_expire(void);
void warm_up_reset(void);
PCountdownTimer create_warm_up_timer(void);
PTimerHandlers create_warm_up_handlers(void);
