#include <pebble.h>
#include "anim.h"
#include "pimp.h"
static GBitmap *s_bitmap = NULL;

static void anim_timer_handler (void *data) 
{
  if(anim_frame_no <= NO_OF_FRAMES) 
  {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "anim_timer_handler: frame %i", anim_frame_no);
    if (s_bitmap != NULL) 
    {
      gbitmap_destroy(s_bitmap);
      s_bitmap = NULL;
    }
    s_bitmap = gbitmap_create_with_resource(anim1_frames[anim_frame_no]);
    bitmap_layer_set_bitmap(bitmap_layer, s_bitmap);
    layer_mark_dirty(bitmap_layer_get_layer(bitmap_layer));
    anim_frame_no++;
    app_timer_register(ANIM_WAIT, anim_timer_handler, NULL);
    //app_timer_reschedule(anim_timer_handle, ANIM_WAIT);
  }
  else
  {
    //app_timer_cancel (anim_timer_handle);
    layer_remove_from_parent (bitmap_layer_get_layer(bitmap_layer));
    watch_mode = show_time;
    update_time();
  }
}

void anim_start (void) 
{
  anim_frame_no = 0;
  watch_mode = show_anim;
  app_timer_register(ANIM_WAIT, anim_timer_handler, NULL);
}
