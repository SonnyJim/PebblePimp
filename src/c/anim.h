#pragma once
typedef enum
{
  UP,
  DOWN,
  LEFT,
  RIGHT,
  STOP
} direction_t;

#define ANIM1_STEPS 8
const int anim1[ANIM1_STEPS][3] = 
{
  {0,0, UP}, {0,11,RIGHT}, {5, 11, DOWN}, {5,0, LEFT}, 
  {1,0, UP}, {1,10, RIGHT}, {4,10, DOWN}, {4,2,STOP}
};

extern void anim_layer_add (void);
extern void anim_layer_remove (void);

/*
#define ANIM1_STEPS 5
const int anim1[ANIM1_STEPS][3] = 
{
  {0,0,UP}, {0,11,RIGHT}, {5, 11, DOWN}, {5,0, LEFT}, {0,0,STOP}
};
*/