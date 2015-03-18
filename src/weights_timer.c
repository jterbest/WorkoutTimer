#include <pebble.h>
#include "weights_timer.h"

#define HEADER_MSG          "Weights"
#define STOPPED_FOOTER_MSG  "Select to start"
#define RUNNING_FOOTER_MSG  "Select to stop"
#define SET_COUNT_MSG       "Set %d"
#define RESET_TIME_SEC      60
#define GET_READY_TIME_SEC  10

static void timer_tick_second(void);
static void select_clicked(void);
static void timer_reset(void);
static void timer_expire(void);

char set_count_text[6];
int set_count=0;

TimerHandlers weights_handlers = {
  .timer_expire_handler = timer_expire,
  .timer_reset_handler = timer_reset,
  .timer_select_button_handler = select_clicked,
  .timer_tick_notify_handler = timer_tick_second
};

TimerState weights_timer = {
  .header = HEADER_MSG,
  .footer = STOPPED_FOOTER_MSG,
  .current_time_sec = RESET_TIME_SEC,
  .is_running = false,
  .handlers = &weights_handlers
};

static void update_set_count_text(void) {
  snprintf(set_count_text, sizeof(set_count_text), SET_COUNT_MSG, set_count);
  weights_timer.stopped_counter_text = set_count_text;
}

static void timer_tick_second(void) {
  if (weights_timer.current_time_sec == GET_READY_TIME_SEC)
    vibes_double_pulse();
}

static void select_clicked(void) {
  if (!weights_timer.is_running) {
    timer_reset();
    weights_timer.footer = RUNNING_FOOTER_MSG;
  } else {
    weights_timer.footer = STOPPED_FOOTER_MSG;
  }
  
  weights_timer.is_running = !weights_timer.is_running;
}

static void timer_reset(void) {
  weights_timer.current_time_sec = RESET_TIME_SEC;
}

static void timer_expire(void) {
  vibes_long_pulse();
  weights_timer.is_running = false;
  set_count++;
  update_set_count_text();
}

PTimerState create_weights_timer(void) {
  set_count = 0;
  update_set_count_text();
  return &weights_timer;
}
