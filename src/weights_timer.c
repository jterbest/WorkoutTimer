#include <pebble.h>
#include "weights_timer.h"

#define HEADER_MSG          "Weights"
#define STOPPED_FOOTER_MSG  "Select to start"
#define RUNNING_FOOTER_MSG  "Select to stop"
#define SET_COUNT_MSG       "Set %d"
#define RESET_TIME_SEC      60
#define GET_READY_TIME_SEC  10

static void timer_tick_second(PTimerState timer);
static void select_clicked(PTimerState timer);
static void timer_reset(PTimerState timer);
static void timer_expire(PTimerState timer);

char set_count_text[6];
int set_count=0;

static void update_set_count_text(PTimerState timer) {
  snprintf(set_count_text, sizeof(set_count_text), SET_COUNT_MSG, set_count);
  timer->stopped_counter_text = set_count_text;
}

static void timer_tick_second(PTimerState timer) {
  if (timer->current_time_sec == GET_READY_TIME_SEC)
    vibes_double_pulse();
}

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
  set_count++;
  update_set_count_text(timer);
}

static void timer_reset_sets(PTimerState timer) {
  if (!timer->is_running) {
    set_count = 0;
    update_set_count_text(timer);
  }
}

void create_weights_timer(PTimerState timer) {
  timer->header = HEADER_MSG;
  timer->footer = STOPPED_FOOTER_MSG;
  timer->current_time_sec = RESET_TIME_SEC;
  timer->is_running = false;
  timer->handlers.timer_expire_handler = timer_expire;
  timer->handlers.timer_reset_handler = timer_reset;
  timer->handlers.timer_select_button_handler = select_clicked;
  timer->handlers.timer_long_select_button_handler = timer_reset_sets;
  timer->handlers.timer_tick_notify_handler = timer_tick_second;
  
  timer_reset_sets(timer);
}
