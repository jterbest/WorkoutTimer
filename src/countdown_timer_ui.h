#pragma once

typedef struct {
  char* header;
  char* footer;
  int current_time_sec;
  bool is_running;
  ClickHandler select_click;
} CountdownTimer;

typedef CountdownTimer* PCountdownTimer;

void show_countdown_timer_ui(PCountdownTimer timer_def);
void hide_countdown_timer_ui(void);
void refresh_countdown_timer(void);
void refresh_header_footer(void);
