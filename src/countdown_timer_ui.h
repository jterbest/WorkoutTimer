#pragma once
#include "timer.h"

typedef void (*ShowNextTimerHandler)(void);

void show_countdown_timer_ui(PTimerState timer_state, ShowNextTimerHandler handler);
void hide_countdown_timer_ui(void);
void refresh_countdown_timer(void);
void refresh_header_footer(void);
