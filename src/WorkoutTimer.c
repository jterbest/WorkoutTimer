#include <pebble.h>
#include "countdown_timer_ui.h"
#include "warm_up_timer.h"
#include "rest_timer.h"
#include "weights_timer.h"

#define NUMBER_OF_TIMERS 3

static PTimerState current_timer;
static PTimerState *timers;
static int active_timer_index;

static void tick_handler(struct tm *tick_time, TimeUnits units) {
  if (!current_timer->is_running)
    return;
  
  PTimerHandlers timer_handlers = &current_timer->handlers;
  current_timer->current_time_sec--;
  if (current_timer->current_time_sec < 1) {
    timer_handlers->timer_expire_handler(current_timer);
  }
  
  if (timer_handlers->timer_tick_notify_handler != NULL)
    timer_handlers->timer_tick_notify_handler(current_timer);
  
  refresh_countdown_timer();
}

static void ShowNextTimer(void)
{
  active_timer_index++;
  if (active_timer_index > NUMBER_OF_TIMERS-1) {
    active_timer_index = 0;
  }
  current_timer = timers[active_timer_index];
  
  hide_countdown_timer_ui();
  show_countdown_timer_ui(current_timer, ShowNextTimer);
}

static void init() {
  active_timer_index = -1;
  timers = (PTimerState *)malloc(sizeof(PTimerState) * NUMBER_OF_TIMERS);
  int i;
  for (i=0;i<NUMBER_OF_TIMERS;i++) {
    timers[i] = (PTimerState)malloc(sizeof(TimerState));
  }
    
  create_warm_up_timer(timers[0]);
  create_rest_timer(timers[1]);
  create_weights_timer(timers[2]);
  
  ShowNextTimer();
  
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

static void deinit() {
  tick_timer_service_unsubscribe();
  hide_countdown_timer_ui();
  
  int i;
  for (i=0;i<NUMBER_OF_TIMERS;i++){
    free(timers[i]);
  }
  free(timers);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
  APP_LOG(APP_LOG_LEVEL_DEBUG, "exiting");
}
