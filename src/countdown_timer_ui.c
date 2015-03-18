#include <pebble.h>
#include "countdown_timer_ui.h"

typedef struct {
  TextLayer *header_layer;
  TextLayer *footer_layer;
  TextLayer *countdown_time_layer;
  ShowNextTimerHandler next_timer_handler;
  PTimerState timer_state;
} TimerUI;

typedef TimerUI* PTimerUI;

static TextLayer* create_header_layer() {
  TextLayer *layer = text_layer_create(GRect(0,0,144,25));
  text_layer_set_background_color(layer, GColorClear);
  text_layer_set_text_color(layer, GColorBlack);
  text_layer_set_font(layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(layer, GTextAlignmentCenter);
  return layer;
}

static TextLayer* create_footer_layer() {
  TextLayer *layer = text_layer_create(GRect(0,120,144,25));
  text_layer_set_background_color(layer, GColorClear);
  text_layer_set_text_color(layer, GColorBlack);
  text_layer_set_font(layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(layer, GTextAlignmentCenter);
  return layer;
}

static TextLayer* create_countdown_time_layer() {
  TextLayer *layer = text_layer_create(GRect(0,50,144,50));
  text_layer_set_background_color(layer, GColorClear);
  text_layer_set_text_color(layer, GColorBlack);
  text_layer_set_font(layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(layer, GTextAlignmentCenter);
  return layer;
}

static void update_countdown_time(PTimerUI timer_ui) {
  PTimerState timer_state = timer_ui->timer_state;
  static char counter_text[5];
  int min = timer_state->current_time_sec / 60;
  int sec = timer_state->current_time_sec % 60;
  snprintf(counter_text, sizeof(counter_text), "%d:%02d", min, sec);
  text_layer_set_text(timer_ui->countdown_time_layer, counter_text);
}

static void refresh_header_footer(void) {
  Window *window = window_stack_get_top_window();
  PTimerUI timer_ui = window_get_user_data(window);
  text_layer_set_text(timer_ui->header_layer, timer_ui->timer_state->header);
  text_layer_set_text(timer_ui->footer_layer, timer_ui->timer_state->footer);
}

static void window_load(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "loading window");
  PTimerUI timer_ui = window_get_user_data(window);
  Layer *window_layer = window_get_root_layer(window);
  
  timer_ui->header_layer = create_header_layer();
  timer_ui->countdown_time_layer = create_countdown_time_layer();
  timer_ui->footer_layer = create_footer_layer();
  
  refresh_header_footer();
  refresh_countdown_timer();
  
  layer_add_child(window_layer, text_layer_get_layer(timer_ui->header_layer));
  layer_add_child(window_layer, text_layer_get_layer(timer_ui->countdown_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(timer_ui->footer_layer));
}

static void window_unload(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "unloading window");
  PTimerUI timer_ui = window_get_user_data(window);
  
  text_layer_destroy(timer_ui->header_layer);
  text_layer_destroy(timer_ui->countdown_time_layer);
  text_layer_destroy(timer_ui->footer_layer);
}

void refresh_countdown_timer(void) {
  Window *window = window_stack_get_top_window();
  PTimerUI timer_ui = window_get_user_data(window);
  PTimerState timer_state = timer_ui->timer_state;
  
  if (timer_state->is_running) {
    update_countdown_time(timer_ui);
  } else {
    text_layer_set_text(timer_ui->countdown_time_layer, "--");
  }
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  PTimerUI timer_ui = (PTimerUI)context;
  PTimerState timer_state = timer_ui->timer_state;
  PTimerHandlers timer_handlers = timer_state->handlers;
  
  timer_handlers->timer_select_button_handler();
  
  refresh_countdown_timer();
  refresh_header_footer();
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  PTimerUI timer_ui = (PTimerUI)context;
  if (!timer_ui->timer_state->is_running)
    timer_ui->next_timer_handler();
}

static void countdown_timer_click_provider(Window *window) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

void show_countdown_timer_ui(PTimerState timer_state, ShowNextTimerHandler handler) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "showing window");
  PTimerUI timer_ui = malloc(sizeof(TimerUI));
  timer_ui->timer_state = timer_state;
  timer_ui->next_timer_handler = handler;
  
  Window *window = window_create();
  WindowHandlers handlers = {
    .load = window_load,
    .unload = window_unload
  };
  
  window_set_user_data(window, timer_ui);
  window_set_window_handlers(window, handlers);
  window_set_click_config_provider_with_context(window, (ClickConfigProvider)countdown_timer_click_provider, timer_ui);
  window_stack_push(window, true);
}

void hide_countdown_timer_ui(void) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "hiding window");
  Window *window = window_stack_pop(true);
  if (window != NULL) {
    window_destroy(window);
    PTimerUI timer_ui = window_get_user_data(window);
    free(timer_ui);
  }
}
