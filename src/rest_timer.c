#include <pebble.h>
#include "rest_timer.h"

#define HEADER_MSG          "Rest"
#define STOPPED_FOOTER_MSG  "Select to start"
#define RUNNING_FOOTER_MSG  "Select to stop"
#define RESET_TIME_SEC      150

static void select_clicked(void);
static void timer_reset(void);
static void timer_expire(void);

TimerHandlers rest_handlers = {
  .timer_expire_handler = timer_expire,
  .timer_reset_handler = timer_reset,
  .timer_select_button_handler = select_clicked
};

TimerState rest_timer = {
  .header = HEADER_MSG,
  .footer = STOPPED_FOOTER_MSG,
  .current_time_sec = RESET_TIME_SEC,
  .is_running = false,
  .handlers = &rest_handlers
};

static void select_clicked(void) {
  if (!rest_timer.is_running) {
    timer_reset();
    rest_timer.footer = RUNNING_FOOTER_MSG;
  } else {
    rest_timer.footer = STOPPED_FOOTER_MSG;
  }
  
  rest_timer.is_running = !rest_timer.is_running;
}

static void timer_reset(void) {
  rest_timer.current_time_sec = RESET_TIME_SEC;
}

static void timer_expire(void) {
  vibes_long_pulse();
  rest_timer.is_running = false;
}

PTimerState create_rest_timer(void) {
  return &rest_timer;
}
