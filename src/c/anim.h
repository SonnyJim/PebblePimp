#pragma once
#define MAX_STEPS 12*6
//Animation starts from 01, up to 12, then right to 60, then down to 05
typedef enum
{
  UP,
  DOWN,
  LEFT,
  RIGHT
} direction_t;
  
const int anim1[MAX_STEPS][2] = 
{
  {0,0}, {0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7}, {0,8}, {0,9}, 
  {0,10}, {0,11}, {1,11}, {2,11}, {3,11}, {4,11}, {5,11}
};
int anim_step;