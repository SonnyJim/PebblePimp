#include "src/c/pimp.h"

Window *s_main_window;
void start_apng (void);

static int time_convert (int hours)
{
  if (hours > 12)
    return hours -= 12;
  else if (hours == 0)
    return 12;
  else
    return hours;
}

void update_time (void) 
{
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *now_tm = localtime(&temp);
  struct tm *gmt_tm = gmtime(&temp);

  //Extract the hours and minutes
  hours = now_tm->tm_hour;
  minutes = now_tm->tm_min;
  hours_gmt = gmt_tm->tm_hour;
  dst_gmt = gmt_tm->tm_isdst;
  
  //Month is from 0 - 11, duuh
  month = now_tm->tm_mon + 1;
  day = now_tm->tm_mday;
  
  //Only show 12hr time format
  hours = time_convert (hours);
  hours_gmt = time_convert (hours_gmt);
  
  //Redraw the dots layer
  layer_mark_dirty(dots_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) 
{
  //don't update the time if the animation is running
  if (watch_mode == show_anim)
    return;
  
  update_time();
}

static void main_window_load(Window *window) 
{
  //Create the two layers used
  GRect bounds = layer_get_bounds(window_get_root_layer(window));
  grid_layer = layer_create(bounds);
  dots_layer = layer_create(bounds);
}

static void main_window_unload(Window *window) 
{
  layer_destroy (grid_layer);
  layer_destroy (dots_layer);
}

static void f_tap_timeout (void * value)
{
  watch_mode = show_time;
  update_time ();
}


static void accel_tap_handler(AccelAxisType axis, int32_t direction) 
{
  // A tap event occured
  if (watch_mode == show_time)
  {
    watch_mode = show_date;
    //Start a timer to change the mode back after timeout
    tap_timer = app_timer_register (TAP_TIMEOUT, f_tap_timeout, NULL);
  }
  else
  {
    watch_mode = show_time;
    if (tap_timer != NULL)
      app_timer_cancel (tap_timer);
  }
    
  //Force redraw
  update_time ();  
}

static void init (void) 
{
  //Initialise the RNG
  srand(time(NULL));
  
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
  layer_set_update_proc (grid_layer, grid_update_proc);
  layer_set_update_proc (dots_layer, dots_update_proc);
  
  // Add to Window
  layer_add_child(window_get_root_layer(s_main_window), grid_layer);
  layer_add_child(window_get_root_layer(s_main_window), dots_layer);

  //Set the watchface to show time
  watch_mode = show_time;
  
  // Make sure the time is displayed from the start
  update_time();
  
  // Register with TickTimerService
  tick_timer_service_subscribe (MINUTE_UNIT, tick_handler);
  
  // Subscribe to tap events
  accel_tap_service_subscribe (accel_tap_handler);
  
  //Show an animation
  start_apng();
}

static void deinit() 
{
  // Destroy Window
  window_destroy(s_main_window);
  accel_tap_service_unsubscribe();
}

int main (void) 
{
  init();
  app_event_loop();
  deinit();
}