#pragma once

typedef struct TimerHandlers TimerHandlers;
typedef struct TimerState TimerState;

typedef TimerState* PTimerState;

typedef void (*TimerExpireHandler)(PTimerState timer);
typedef void (*TimerResetHandler)(PTimerState timer);
typedef void (*TimerSelectButtonHandler)(PTimerState timer);
typedef void (*TimerLongSelectButtonHandler)(PTimerState timer);
typedef void (*TimerTickNotifyHandler)(PTimerState timer);

/**
 * \brief Callback handlers that define the timer-specific behavior.
 *
 */
struct TimerHandlers {
  TimerExpireHandler timer_expire_handler;
  TimerResetHandler timer_reset_handler;
  TimerSelectButtonHandler timer_select_button_handler;
  TimerLongSelectButtonHandler timer_long_select_button_handler;
  TimerTickNotifyHandler timer_tick_notify_handler;
};

/**
 * \brief Structure that holds all the current state and handler
 *        callbacks for the current timer.
 * 
 */
struct TimerState {
  char* header;
  char* footer;
  char* stopped_counter_text;
  int current_time_sec;
  bool is_running;
  TimerHandlers handlers;
};

typedef TimerHandlers* PTimerHandlers;
