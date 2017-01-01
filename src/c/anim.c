#include "pimp.h"
#include "anim.h"
#include "src/c/draw.h"

extern void redraw_canvas (void);
AppTimer * anim_timer;

static void timer_callback(void *context) 
{
  layer_mark_dirty(anim_layer);
  app_timer_register(50, timer_callback, NULL);
}

static void display_face (void)
{
  //put in here however we are going to update the display
}

void anim_start ()
{
  watch_mode = show_anim;
  anim_layer_add ();
  anim_timer = app_timer_register(50, timer_callback, NULL); 
}

void anim_stop ()
{
  app_timer_cancel (anim_timer);
  anim_layer_remove ();
  watch_mode = show_time;
}

void anim_draw (GContext *ctx)
{
  int i,j,x,y;
  
  face_init_array ();
  
  //draw_grid (ctx);
  //draw_text (ctx);
  
  //Load up the initial values
  x = anim1[0][0];
  y = anim1[0][1];

  face_fill (x, y, ctx);

  
  for (i = 0; i < ANIM1_STEPS; i++)
  {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Loop index now %d, %d, %d", anim1[i][0], anim1[i][1], anim1[i][2]);
    x = anim1[i][0];
    y = anim1[i][1];
    
    if (anim1[i][2] == STOP)
      break;
    
    for (j = 0; j < 12; j++)
    {
      switch (anim1[i][2])
      {
        case UP:
          y++;
          break;
        case DOWN:
          y--; 
          break;
        case LEFT:
          x--;
          break;
        case RIGHT:
           x++;
           break;
         default:
           break;
      }
          
      APP_LOG(APP_LOG_LEVEL_DEBUG, "%d, %d, want %d %d", x,y, anim1[i + 1][0], anim1[i + 1][1]);
      if (x == anim1[i + 1][0] && y == anim1[i + 1][1])
        break;
      }
    }
  //Animation is finished
}

void anim_update_proc(Layer *layer, GContext *ctx)
{
    
}