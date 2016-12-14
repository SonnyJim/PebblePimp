#include "src/c/pimp.h"

static int offset_x = BLOBSIZE + 20;
static int offset_y = BLOBSIZE / 2;

bool face_array[FACE_WIDTH][FACE_HEIGHT];

//Initialise the face array
static void face_init (void)
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

static void face_update (int hours, int minutes)
{
  int x, y = 0;
  int rows = minutes / 5;
  int remainder = minutes % 5;
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "rows is %d, remainder %d", rows, remainder);
  
  face_init ();
  
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

//Draws a rounded rectangle at a particular position on the face
static void face_fill (int x, int y, GContext *ctx)
{
  int x_face, y_face;
  //Convert the grid array coords into the screen coords
  x_face = 1 + offset_x + (x * (BLOBSIZE + 1));
  y_face = HEIGHT - BLOBSIZE - offset_y + 1 - (y * (BLOBSIZE + 1));
  //GRect rect_bounds = GRect(x_face, y_face, BLOBSIZE - 2, BLOBSIZE - 2);

  graphics_fill_rect(ctx, GRect(x_face, y_face, BLOBSIZE - 2, BLOBSIZE - 2), 3, GCornersAll);
}

static void draw_face (GContext *ctx)
{
  int x, y;
  
  //Update the face array with the current time
  if (current_mode == show_time)
    face_update (hours, minutes);
  else
    face_update (month, day);
  
  //Draw the array contents into the grid
  for (y = 0; y < FACE_HEIGHT; y++)
  {
    for (x = 0; x < FACE_WIDTH; x++)
      if (face_array[x][y])
        face_fill (x, y, ctx);
  }
  
}

static void draw_grid (GContext *ctx)
{
  int i;
  //Draw outline
  GRect rect_bounds = GRect(offset_x - 1, offset_y, ((BLOBSIZE + 1) * 6) + 1, ((BLOBSIZE + 1) * 12) + 1);
  graphics_draw_rect(ctx, rect_bounds);
  
  //Draw vertical lines
  GPoint start = GPoint(offset_x, 0);
  GPoint end = GPoint(offset_x, 0);

  for (i = 1; i < FACE_WIDTH; i++)
  {
    start.x += BLOBSIZE ;
    start.y = offset_y;
    end.x = start.x;
    end.y = HEIGHT - offset_y;
    graphics_draw_line(ctx, start, end);  
    start.x += 1;
  }
  
  //Draw horizontal lines
  start.y = HEIGHT - offset_y;
  end.x = offset_x + ((BLOBSIZE + 1) * 6) - 1;
  start.x = offset_x;
  for (i = 1; i < FACE_HEIGHT; i++)
  { 
    start.y -= BLOBSIZE;
    start.y -= 1;
    end.y = start.y;
    graphics_draw_line(ctx, start, end);    
  }
}

static void draw_text (GContext *ctx)
{
  GRect text_rect = GRect(0, 0, BLOBSIZE, BLOBSIZE);
  char str[3];
  int i;
  
  //Skip to the bottom of the screen
  text_rect.origin.y = HEIGHT - BLOBSIZE - offset_y - 4;
  
  for (i = 1; i <= 12; i++)
  {
    //Draw hour text
    snprintf (str, sizeof(str), "%i", i);
    text_rect.origin.x = offset_x - 14;
    graphics_draw_text(ctx, str, fonts_get_system_font(FONT_KEY_GOTHIC_14), 
                     text_rect, GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
    
    //Draw minutes text
    snprintf (str, sizeof(str), "%i", i * 5);
    text_rect.origin.x = offset_x + (BLOBSIZE * 7);
    text_rect.origin.x -= 4;
    
    graphics_draw_text(ctx, str, fonts_get_system_font(FONT_KEY_GOTHIC_14), 
                     text_rect, GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
    
    text_rect.origin.y -= BLOBSIZE;
    text_rect.origin.y -= 1; //spacing
  }
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
  
  draw_grid(ctx);
  //Draw the hours and minutes text
  draw_text (ctx);

  draw_face (ctx);
}