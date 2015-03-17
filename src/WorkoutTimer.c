#include <pebble.h>
#include "countdown_timer_ui.h"
#include "warm_up_timer.h"

static PTimerState current_timer;

static void tick_handler(struct tm *tick_time, TimeUnits units) {
  if (!current_timer->is_running)
    return;
  
  PTimerHandlers timer_handlers = current_timer->handlers;
  current_timer->current_time_sec--;
  if (current_timer->current_time_sec < 1) {
    timer_handlers->timer_expire_handler();
  }
  
  refresh_countdown_timer();
}

static void init() {
  current_timer = create_warm_up_timer();
  
  show_countdown_timer_ui(current_timer);
  
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

static void deinit() {
  tick_timer_service_unsubscribe();
  hide_countdown_timer_ui();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
