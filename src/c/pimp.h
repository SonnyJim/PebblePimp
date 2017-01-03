#include <pebble.h>
#pragma once

/* Pebble classic resolution is 144×168 */
/* Pimp watch resolution is 6x12 */

#define BLOBSIZE 12
//Screen resolution
#define WIDTH 144
#define HEIGHT 168

//Face resolution
#define FACE_WIDTH 6
#define FACE_HEIGHT 12

int hours, minutes, hours_gmt, dst_gmt;
int month, day;

extern void grid_update_proc(Layer *layer, GContext *ctx);
extern void dots_update_proc(Layer *layer, GContext *ctx);

enum watch_modes
{
  show_time,
  show_date,
  show_anim
};

int watch_mode;

AppTimer * tap_timer;

Layer *grid_layer;
Layer *dots_layer;

void update_time (void);
void face_update (int hours, int minutes);