#include "src/c/pimp.h"

static Layer *s_canvas_layer;
static Window *s_main_window;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *now_tm = localtime(&temp);
  struct tm *gmt_tm = gmtime(&temp);

  //Extract the hours and minutes
  hours = now_tm->tm_hour;
  minutes = now_tm->tm_min;
  hours_gmt = gmt_tm->tm_hour;
  dst_gmt = gmt_tm->tm_isdst;
  
  month = now_tm->tm_mon;
  day = now_tm->tm_mday;
  
  
  //Only show 12hr time format
  if (hours > 12)
    hours -= 12;
  
  if (hours_gmt > 12)
    hours_gmt -= 12;
  
  // Redraw the watchface
  layer_mark_dirty(s_canvas_layer);
}


static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window) {
  GRect bounds = layer_get_bounds(window_get_root_layer(window));
  s_canvas_layer = layer_create(bounds);
}

static void main_window_unload(Window *window) {
  //Destroy the canvas layer??
}

static void accel_tap_handler(AccelAxisType axis, int32_t direction) {
  // A tap event occured
   APP_LOG(APP_LOG_LEVEL_DEBUG, "A tap happened");

}


static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  // Assign the custom drawing procedure
  layer_set_update_proc(s_canvas_layer, canvas_update_proc);
  // Add to Window
  layer_add_child(window_get_root_layer(s_main_window), s_canvas_layer);
  // Make sure the time is displayed from the start
  update_time();

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // Subscribe to tap events
  accel_tap_service_subscribe(accel_tap_handler);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
  accel_tap_service_unsubscribe();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}