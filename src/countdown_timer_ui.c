#include <pebble.h>
#include "countdown_timer_ui.h"

static Window *window;
static TextLayer *header_layer;
static TextLayer *footer_layer;
static TextLayer *countdown_time_layer;

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

static void update_countdown_time(int current_time_sec) {
  static char counter_text[5];
  snprintf(counter_text, sizeof(counter_text), "%d", current_time_sec);
  text_layer_set_text(countdown_time_layer, counter_text);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  
  header_layer = create_header_layer();
  countdown_time_layer = create_countdown_time_layer();
  footer_layer = create_footer_layer();
  
  refresh_header_footer();
  refresh_countdown_timer();
  
  layer_add_child(window_layer, text_layer_get_layer(header_layer));
  layer_add_child(window_layer, text_layer_get_layer(countdown_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(footer_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(header_layer);
  text_layer_destroy(countdown_time_layer);
  text_layer_destroy(footer_layer);
}

void refresh_countdown_timer(void) {
  PTimerState timer_state = window_get_user_data(window);
  if (timer_state->is_running) {
    update_countdown_time(timer_state->current_time_sec);
  } else {
    text_layer_set_text(countdown_time_layer, "--");
  }
}

void refresh_header_footer(void) {
  PTimerState timer_state = window_get_user_data(window);
  text_layer_set_text(header_layer, timer_state->header);
  text_layer_set_text(footer_layer, timer_state->footer);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  PTimerState timer_state = (PTimerState)context;
  PTimerHandlers timer_handlers = timer_state->handlers;
  
  timer_handlers->timer_select_button_handler();
  
  refresh_countdown_timer();
  refresh_header_footer();
}

static void countdown_timer_click_provider(Window *window) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

void show_countdown_timer_ui(PTimerState timer_state) {
  window = window_create();
  WindowHandlers handlers = {
    .load = window_load,
    .unload = window_unload
  };
  window_set_user_data(window, timer_state);
  window_set_window_handlers(window, handlers);
  window_set_click_config_provider_with_context(window, (ClickConfigProvider)countdown_timer_click_provider, timer_state);
  window_stack_push(window, true);
}

void hide_countdown_timer_ui(void) {
  window_destroy(window);
}
