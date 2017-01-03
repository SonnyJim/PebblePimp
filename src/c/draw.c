#include "src/c/pimp.h"
#include "draw.h"
int offset_x = BLOBSIZE + 20;
int offset_y = BLOBSIZE / 2;

void draw_grid (GContext *ctx)
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

void draw_text (GContext *ctx)
{
  GRect text_rect = GRect(0, 0, BLOBSIZE, BLOBSIZE);
  char str[3];
  int i;
  
  //Skip to the bottom of the screen
  text_rect.origin.y = HEIGHT - BLOBSIZE - offset_y - 4;
  
  for (i = 1; i <= 12; i++)
  {
    //Draw hour text
    if (i < 10)
      snprintf (str, sizeof(str), "0%i", i);
    else
      snprintf (str, sizeof(str), "%i", i);
    text_rect.origin.x = offset_x - 14;
    graphics_draw_text(ctx, str, fonts_get_system_font(FONT_KEY_GOTHIC_14), 
                     text_rect, GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
    //Draw the GMT dot
    if (hours_gmt == i && watch_mode == show_date)
      graphics_fill_circle (ctx, GPoint (10, text_rect.origin.y + 9), 2);
    
    //Draw minutes text
    if (i * 5 < 10)
      snprintf (str, sizeof(str), "0%i", i * 5);
    else
      snprintf (str, sizeof(str), "%i", i * 5);

    text_rect.origin.x = offset_x + (BLOBSIZE * 7);
    text_rect.origin.x -= 4;
    
    graphics_draw_text(ctx, str, fonts_get_system_font(FONT_KEY_GOTHIC_14), 
                     text_rect, GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
    
    text_rect.origin.y -= BLOBSIZE;
    text_rect.origin.y -= 1; //spacing
  }
}

//Draw the array contents onto the watchface
void draw_face_array (GContext *ctx)
{
  int x, y;
  for (y = 0; y < FACE_HEIGHT; y++)
  {
    for (x = 0; x < FACE_WIDTH; x++)
      if (face_array[x][y])
        face_fill (x, y, ctx);
  }
}

//Draws a rounded rectangle at a given position on the face
int face_fill (int x, int y, GContext *ctx)
{
  int x_face, y_face;
  
  if (x > 6 || y > 12)
  {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "face_fill warning:  %d,%d requested out of range", x,y);
    return 1;
  }
  
  //Convert the grid array coords into the screen coords
  x_face = 1 + offset_x + (x * (BLOBSIZE + 1));
  y_face = HEIGHT - BLOBSIZE - offset_y + 1 - (y * (BLOBSIZE + 1));

  graphics_fill_rect(ctx, GRect(x_face, y_face, BLOBSIZE - 2, BLOBSIZE - 2), 1, GCornersAll);
  return 0;
}

