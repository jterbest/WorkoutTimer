#include <pebble.h>
#include "rest_timer.h"

#define HEADER_MSG          "Rest"
#define STOPPED_FOOTER_MSG  "Select to start"
#define RUNNING_FOOTER_MSG  "Select to stop"
#define RESET_TIME_SEC      150

static void select_clicked(PTimerState timer);
static void timer_reset(PTimerState timer);
static void timer_expire(PTimerState timer);

static void select_clicked(PTimerState timer) {
  if (!timer->is_running) {
    timer_reset(timer);
    timer->footer = RUNNING_FOOTER_MSG;
  } else {
    timer->footer = STOPPED_FOOTER_MSG;
  }
  
  timer->is_running = !timer->is_running;
}

static void timer_reset(PTimerState timer) {
  timer->current_time_sec = RESET_TIME_SEC;
}

static void timer_expire(PTimerState timer) {
  vibes_long_pulse();
  timer->is_running = false;
}

void create_rest_timer(PTimerState timer) {
  timer->header = HEADER_MSG;
  timer->footer = STOPPED_FOOTER_MSG;
  timer->current_time_sec = RESET_TIME_SEC;
  timer->is_running = false;
  timer->handlers.timer_expire_handler = timer_expire;
  timer->handlers.timer_reset_handler = timer_reset;
  timer->handlers.timer_select_button_handler = select_clicked;
}
