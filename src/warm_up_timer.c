#include <pebble.h>
#include "warm_up_timer.h"

#define HEADER_MSG          "Warm up"
#define STOPPED_FOOTER_MSG  "Select to start"
#define RUNNING_FOOTER_MSG  "Select to stop"
#define RESET_TIME_SEC      15

static void select_clicked(void);
static void timer_reset(void);
static void timer_expire(void);

TimerHandlers warm_up_handlers = {
  .timer_expire_handler = timer_expire,
  .timer_reset_handler = timer_reset,
  .timer_select_button_handler = select_clicked
};

TimerState warm_up_timer = {
  .header = HEADER_MSG,
  .footer = STOPPED_FOOTER_MSG,
  .current_time_sec = RESET_TIME_SEC,
  .is_running = false,
  .handlers = &warm_up_handlers
};

void select_clicked(void) {
  if (!warm_up_timer.is_running) {
    timer_reset();
    warm_up_timer.footer = RUNNING_FOOTER_MSG;
  } else {
    warm_up_timer.footer = STOPPED_FOOTER_MSG;
  }
  
  warm_up_timer.is_running = !warm_up_timer.is_running;
}

void timer_reset(void) {
  warm_up_timer.current_time_sec = RESET_TIME_SEC;
}

void timer_expire(void) {
  timer_reset();
  vibes_short_pulse();
}

PTimerState create_warm_up_timer(void) {
  return &warm_up_timer;
}
