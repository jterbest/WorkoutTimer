#include <pebble.h>
#include "warm_up_timer.h"

#define HEADER_MSG          "Warm up"
#define STOPPED_FOOTER_MSG  "Select to start"
#define RUNNING_FOOTER_MSG  "Select to stop"
#define RESET_TIME_SEC      15

TimerHandlers warm_up_handlers = {
  .timer_expire_handler = warm_up_time_expire,
  .timer_reset_handler = warm_up_reset,
  .timer_select_button_handler = warm_up_select_clicked
};

TimerState warm_up_timer = {
  .header = HEADER_MSG,
  .footer = STOPPED_FOOTER_MSG,
  .current_time_sec = RESET_TIME_SEC,
  .is_running = false,
  .handlers = &warm_up_handlers
};

void warm_up_select_clicked(void) {
  if (!warm_up_timer.is_running) {
    warm_up_reset();
    warm_up_timer.footer = RUNNING_FOOTER_MSG;
  } else {
    warm_up_timer.footer = STOPPED_FOOTER_MSG;
  }
  
  warm_up_timer.is_running = !warm_up_timer.is_running;
}

void warm_up_reset(void) {
  warm_up_timer.current_time_sec = RESET_TIME_SEC;
}

void warm_up_time_expire(void) {
  warm_up_reset();
  vibes_short_pulse();
}

PTimerState create_warm_up_timer(void) {
  return &warm_up_timer;
}
