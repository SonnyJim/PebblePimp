#include "pimp.h"
#include "anim.h"
#include "src/c/draw.h"

extern void redraw_canvas (void);

void anim_draw (GContext *ctx)
{
  anim_step = 0;
  face_init_array ();
  draw_grid (ctx);
  draw_text (ctx);
  while (anim_step < MAX_STEPS)
  {
    face_fill (anim1[anim_step][0], anim1[anim_step][1], ctx);
    redraw_canvas ();
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Loop index now %d, %d", anim1[anim_step][0], anim1[anim_step][1]);
    anim_step++;
    psleep (50);
  }
  //Animation is finished, show watchface again
  //watch_mode = show_time;
}