#include <pebble.h>

static GBitmapSequence *s_sequence;
static GBitmap *s_bitmap;
static BitmapLayer *s_bitmap_layer;

extern Window *s_main_window;

void init_apng (void)
{
  // Create sequence
  s_sequence = gbitmap_sequence_create_with_resource(RESOURCE_ID_APNG);

  // Create blank GBitmap using APNG frame size
  GSize frame_size = gbitmap_sequence_get_bitmap_size(s_sequence);
  s_bitmap = gbitmap_create_blank(frame_size, GBitmapFormat8Bit);  
  s_bitmap_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
 // s_bitmap_layer = bitmap_layer_create(frame_size);
  bitmap_layer_set_compositing_mode(s_bitmap_layer, GCompOpSet);
  bitmap_layer_set_bitmap(s_bitmap_layer, s_bitmap);
  layer_add_child(window_get_root_layer(s_main_window), bitmap_layer_get_layer(s_bitmap_layer));
}

static void timer_handler(void *context) 
{
  uint32_t next_delay;
  
  next_delay = 10;
  // Advance to the next APNG frame, and get the delay for this frame
  if(gbitmap_sequence_update_bitmap_next_frame(s_sequence, s_bitmap, &next_delay)) {
    // Set the new frame into the BitmapLayer
    bitmap_layer_set_bitmap(s_bitmap_layer, s_bitmap);
    layer_mark_dirty(bitmap_layer_get_layer(s_bitmap_layer));

    // Timer for that frame's delay
    app_timer_register(next_delay, timer_handler, NULL);
  }
  //gbitmap_sequence_destroy(s_sequence);
  //gbitmap_destroy(s_bitmap);
  //bitmap_layer_destroy(s_bitmap_layer);
}

void start_apng (void)
{
  init_apng();
  uint32_t first_delay_ms = 10;
  // Schedule a timer to advance the first frame
  app_timer_register(first_delay_ms, timer_handler, NULL);  
}