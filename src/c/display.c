#include "src/c/pimp.h"
#include "src/c/draw.h"

bool face_array[FACE_WIDTH][FACE_HEIGHT];

//Initialise the face array
static void face_init_array (void)
{
  int x,y;
  
  for (x = 0; x < FACE_WIDTH; x++)
  {
    for (y = 0; y < FACE_HEIGHT; y++)
    {
      face_array[x][y] = false;  
    }
  }
}

//Update the face array with the hours and minutes provided
static void face_update (int hours, int minutes)
{
  int x, y = 0;
  int rows = minutes / 5;
  int remainder = minutes % 5;
    
  //Fill the array with the hours
  x = 0;
  for (y = 0; y < hours; y++)
  {
      face_array[x][y] = true;
  }
  
  //Fill out the array with the complete rows of minutes
  for (y = 0; y < rows; y++)
  {
    for (x = 1; x < 6; x++)
    {
      face_array [x][y] = true;
    }
  }
  
  //Fill out the remainder on the last row
  y = rows;
  for (x = 1; x <= remainder; x++)
  {
    face_array [x][y] = true;  
  }
}

//Draw the 'D' symbol on the array when in date mode
static void face_draw_date_symbol (void)
{
  face_array[3][11] = true;
  face_array[4][11] = true;
  face_array[3][10] = true;
  face_array[5][10] = true;
  face_array[3][9] = true;
  face_array[5][9] = true;
  face_array[3][8] = true;
  face_array[5][8] = true;
  face_array[3][7] = true;
  face_array[4][7] = true;
}


static void draw_face (GContext *ctx)
{
  //Clear the face
  face_init_array ();
  draw_grid (ctx);
  draw_text (ctx);
  //draw_gmt (ctx);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "gmt time %d", hours_gmt);
  if (watch_mode == show_time)
    face_update (hours, minutes);
  else
  {
    face_draw_date_symbol ();
    face_update (month, day);
    draw_gmt (ctx);
  }
  draw_face_array (ctx);
}


//Main drawing routine
void canvas_update_proc(Layer *layer, GContext *ctx) {
  //Set the background to black
  GRect rect_bounds = GRect (0, 0, WIDTH, HEIGHT);
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, rect_bounds, 0, GCornersAll);
  
  //Set the colours for everything else
  graphics_context_set_stroke_color(ctx, GColorWhite);
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_context_set_text_color(ctx, GColorWhite);
  
  draw_face (ctx);
}