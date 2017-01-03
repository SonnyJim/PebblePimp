#pragma once
enum anim_states
{
  STOP,
  PING_PONG,
  FORWARD,
  BACKWARD
};

struct
{
//  int init_state;
  int step;
  int max_steps;
  int state;
  int num; //Which animation we are playing
  int wait; //How long to wait between frame updates
} anim;

void anim_start (int anim_num);
void anim_stop (void);

void anim_step_increment (void);
void frame_draw_array (int frame_no);