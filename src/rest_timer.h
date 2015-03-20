#pragma once
#include "timer.h"

/**
 * \brief Create a rowing machine rest interval timer that provides
 *        vibrate notice to return to machine.
 * 
 * \param timer The timer initialize as a rest timer.
 * 
 */
void create_rest_timer(PTimerState timer);
