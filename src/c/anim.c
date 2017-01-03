#include "pimp.h"
#include "anim.h"
#include "src/c/draw.h"
#include "src/c/frame.h"

AppTimer * anim_timer;

static void timer_callback(void *context) 
{
  layer_mark_dirty(dots_layer);
  app_timer_register(anim.wait, timer_callback, NULL);
}

static int anim_check_dot (int frame_no, int j, int i)
{
  switch (anim.num)
  {
    case 1:
      return anim1_frame[frame_no][j][i];
      break;
    
    case 2:
      return anim2_frame[frame_no][j][i];
      break;
    
    case 3:
      return anim3_frame[frame_no][j][i];
      break;
    
    case 4:
      return anim4_frame[frame_no][j][i];
      break;
    default:
      return 0;
      break;
  }
  return 0;
}

static void anim_load (int anim_num)
{
  anim.step = 0;
  anim.num = anim_num;
  anim.wait = 1;
  switch (anim_num)
  {
    case 1:
      anim.state = PING_PONG;
      anim.max_steps = ANIM1_STEPS;
      break;
    
    case 2:
      anim.state = PING_PONG;
      anim.max_steps = ANIM2_STEPS;
      break;
    
    case 3:
      anim.wait = 50;
      anim.state = PING_PONG;
      anim.max_steps = ANIM3_STEPS;
      break;
       
    case 4:
      anim.wait = 50;
      anim.state = PING_PONG;
      anim.max_steps = ANIM4_STEPS;
      break;
    
    default:
      anim.state = STOP;
      anim.max_steps = 0;
      break;
  }
}

static void anim_backup_array (void)
{
  int i, j;
  
  for (i = 0; i < FACE_WIDTH; i++)
  {
    for (j = 0; j < FACE_HEIGHT; j++)
    {
      face_array_old[i][j] = face_array[i][j];
    }
  }
}

void anim_start (int anim_num)
{
  if (anim.state != STOP)
    return;
  face_update (hours, minutes);
  anim_backup_array ();
  //Randomise animation if none passed
  if (anim_num == 0)
  {
    	anim_num = rand() / (RAND_MAX / MAX_ANIMS + 1) + 1;
  }
  
  if (anim_num > MAX_ANIMS || anim_num < 1)
  {
    //FIXME APP_LOG here
    return;
  }
  
  anim_load (anim_num);
  watch_mode = show_anim;
  anim_timer = app_timer_register(anim.wait, timer_callback, NULL); 
}

void anim_stop (void)
{
  anim.step = 0;
  anim.state = STOP;
  anim.max_steps = 0;
  app_timer_cancel (anim_timer);
  watch_mode = show_time;
  update_time ();
}

void anim_step_increment (void)
{
  
  switch (anim.state)
  {
    case PING_PONG:
      if (anim.step >= anim.max_steps - 1)
        anim.state = BACKWARD;
      else
        anim.step++;
    break;
    
    case FORWARD:
      if (anim.step >= anim.max_steps - 1)
        anim_stop ();
      else
        anim.step++;
      break;
    
    case BACKWARD:
      anim.step--;
      if (anim.step <= 0)
        anim_stop ();
      break;
    
    case STOP:
    default:
      anim_stop ();
      break;
  }
}

void frame_draw_array (int frame_no)
{
  int i,j,n;
  
  //Odd X/Y is due to formatting in header file FIXME
  for (i = 0; i < FACE_WIDTH; i++)
  {
    n = 11;
    for (j = 0; j < FACE_HEIGHT; j++)
    {   
//      if (anim1_frame[frame_no][j][i])
      if (anim_check_dot (frame_no, j, i))
          face_array[i][n] = 1;
      else 
        face_array[i][n] = 0;
      
      if (anim.state == BACKWARD && face_array_old[i][n])
      {
        face_array[i][n] = face_array_old[i][n];  
      }
      n--;
    }
  }
}