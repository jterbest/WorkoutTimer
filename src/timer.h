#pragma once

typedef void (*TimerExpireHandler)(void);
typedef void (*TimerResetHandler)(void);
typedef void (*TimerSelectButtonHandler)(void);

typedef struct {
  TimerExpireHandler timer_expire_handler;
  TimerResetHandler timer_reset_handler;
  TimerSelectButtonHandler timer_select_button_handler;
} TimerHandlers;

typedef TimerHandlers* PTimerHandlers;

typedef struct {
  char* header;
  char* footer;
  int current_time_sec;
  bool is_running;
  PTimerHandlers handlers;
} TimerState;

typedef TimerState* PTimerState;
