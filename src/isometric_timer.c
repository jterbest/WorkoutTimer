#include <pebble.h>
#include "isometric_timer.h"

#define HEADER_MSG          "Isometric"
#define STOPPED_FOOTER_MSG  "Select to start"
#define RUNNING_FOOTER_MSG  "Select to stop"
#define REP_COUNT_MSG       "Rep %d"
#define SHORT_RESET_TIME    10
#define MEDIUM_RESET_TIME   30

typedef enum {
  short_and_auto_reset,
  med_manual_reset,
  med_auto_reset,
  timer_mode_count
} Isometric_Timer_Modes;

#define BEFORE_FIRST_TIMER_MODE    -1;

static char rep_count_text[7];
static int current_rep_reset_sec;
static int rep_count = 0;
static Isometric_Timer_Modes current_timer_mode = short_and_auto_reset;

static void reset_timer_mode(PTimerState timer);

static void update_rep_count_text(PTimerState timer) {
  snprintf(rep_count_text, sizeof(rep_count_text), REP_COUNT_MSG, rep_count);
  timer->stopped_counter_text = rep_count_text;
}

void timer_reset(PTimerState timer) {
  timer->current_time_sec = current_rep_reset_sec;
}

void timer_expire(PTimerState timer) {
  ++rep_count;
  update_rep_count_text(timer);
  
  switch(current_timer_mode) {
    case short_and_auto_reset:
    case med_auto_reset:
      vibes_short_pulse();
      timer_reset(timer);
      break;
    default:
      timer->is_running = false;
      vibes_long_pulse();
      break;
  }
}

void start_stop_timer(PTimerState timer) {
  if (!timer->is_running) {
    timer_reset(timer);
    timer->footer = RUNNING_FOOTER_MSG;
  } else {
    timer->footer = STOPPED_FOOTER_MSG;
  }
  
  timer->is_running = !timer->is_running;
  update_rep_count_text(timer);
}

static void timer_reset_reps(PTimerState timer) {
  if (!timer->is_running) {
    rep_count = 0;
    update_rep_count_text(timer);
  }
}

static void select_rep_time(PTimerState timer) {
  ++current_timer_mode;
  
  switch(current_timer_mode) {
    case short_and_auto_reset:
      current_rep_reset_sec = SHORT_RESET_TIME;
      timer->stopped_counter_text = "10*";
      break;
    case med_manual_reset:
      timer->stopped_counter_text = "30s";
      current_rep_reset_sec = MEDIUM_RESET_TIME;
      break;
    case med_auto_reset:
      timer->stopped_counter_text = "30*";
      current_rep_reset_sec = MEDIUM_RESET_TIME;
      break;
    case timer_mode_count:
      reset_timer_mode(timer);
      return;
  }
  
  timer_reset(timer);
}

static void reset_timer_mode(PTimerState timer) {
  current_timer_mode = BEFORE_FIRST_TIMER_MODE;
  select_rep_time(timer);
}

void create_isometric_timer(PTimerState timer) {
  timer->header           = HEADER_MSG;
  timer->footer           = STOPPED_FOOTER_MSG;
  timer->is_running       = false;
  
  timer->handlers.timer_expire_handler             = timer_expire;
  timer->handlers.timer_reset_handler              = timer_reset;
  timer->handlers.timer_select_button_handler      = start_stop_timer;
  timer->handlers.timer_long_select_button_handler = timer_reset_reps;
  timer->handlers.timer_back_button_handler        = select_rep_time;
  
  reset_timer_mode(timer);
}
