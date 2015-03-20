#pragma once
#include "timer.h"

typedef void (*ShowNextTimerHandler)(void);

/**
 * \brief Display a new window with the specified timer.
 * 
 * \param timer_state The timer that will be displayed in the window
 * \param handler The callback function that will advance to next window
 *
 */
void show_countdown_timer_ui(PTimerState timer_state, ShowNextTimerHandler handler);

/**
 * \brief Hide the top-most timer window
 *
 */
void hide_countdown_timer_ui(void);

/**
 * \brief Refresh the timer state on the window
 * 
 */
void refresh_countdown_timer(void);
