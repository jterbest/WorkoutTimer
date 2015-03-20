#pragma once
#include "timer.h"

/**
 * \brief Create a warm up timer that is used to vibrate and auto-reset
 *        continuously to notify the user to change stretch.
 * 
 * \param timer The timer initialize as a rest timer.
 * 
 */
void create_warm_up_timer(PTimerState timer);
