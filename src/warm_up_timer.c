#include <pebble.h>
#include "warm_up_timer.h"

#define HEADER_MSG          "Warm up"
#define STOPPED_FOOTER_MSG  "Select to start"
#define RUNNING_FOOTER_MSG  "Select to stop"
#define RESET_TIME_SEC      15

static void select_clicked(PTimerState timer);
static void timer_reset(PTimerState timer);
static void timer_expire(PTimerState timer);

void select_clicked(PTimerState timer) {
  if (!timer->is_running) {
    timer_reset(timer);
    timer->footer = RUNNING_FOOTER_MSG;
  } else {
    timer->footer = STOPPED_FOOTER_MSG;
  }
  
  timer->is_running = !timer->is_running;
}

void timer_reset(PTimerState timer) {
  timer->current_time_sec = RESET_TIME_SEC;
}

void timer_expire(PTimerState timer) {
  timer_reset(timer);
  vibes_short_pulse();
}

void create_warm_up_timer(PTimerState timer) {
  timer->header = HEADER_MSG;
  timer->footer = STOPPED_FOOTER_MSG;
  timer->current_time_sec = RESET_TIME_SEC;
  timer->is_running = false;
  timer->handlers.timer_expire_handler = timer_expire;
  timer->handlers.timer_reset_handler = timer_reset;
  timer->handlers.timer_select_button_handler = select_clicked;
}
