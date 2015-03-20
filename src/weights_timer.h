#pragma once
#include "timer.h"

/**
 * \brief Create a weight lifting timer that provides a 'get ready'
 *        vibrate notice in addition to timer expiration vibrate.
 * 
 * \param timer The timer initialize as a weight timer.
 * 
 */
void create_weights_timer(PTimerState timer);
