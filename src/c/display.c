#include "src/c/pimp.h"

static int offset_x = BLOBSIZE + 20;
static int offset_y = BLOBSIZE / 2;

static void draw_grid (GContext *ctx){
  
  //Draw outline
  GRect rect_bounds = GRect(offset_x - 1, offset_y, ((BLOBSIZE + 1) * 6) + 1, ((BLOBSIZE + 1) * 12) + 1);
  graphics_draw_rect(ctx, rect_bounds);
  
  //Draw lines
  GPoint start = GPoint(offset_x, 0);
  GPoint end = GPoint(offset_x, 0);
  
  int i;
  //Draw vertical lines
  for (i = 1; i < 6; i++)
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
  for (i = 1; i < 12; i++)
  { 
    start.y -= BLOBSIZE;
    start.y -= 1;
    end.y = start.y;
    graphics_draw_line(ctx, start, end);  
    
  }
}

static void draw_minutes (int minutes, GContext *ctx){
  GRect rect_bounds = GRect(0, 0, BLOBSIZE - 2, BLOBSIZE - 2);
  int corner_radius = 3;
  int i, j, rows, remainder;

  if (minutes > 60)
    minutes = 60;
  else if (minutes <= 0)
    return;

  //Draw minutes
  rows = minutes / 5;
  remainder = minutes % 5;
  //Skip to the bottom of the screen
  rect_bounds.origin.y = HEIGHT - BLOBSIZE - offset_y + 2;
  rect_bounds.origin.x = offset_x + BLOBSIZE + 1;
  
  //Draw the complete rows   
  for (i = 0; i < rows; i++)
  {
    rect_bounds.origin.y -= 1;
    for (j = 0; j < 5; j++)
    {
      rect_bounds.origin.x += 1;
      
      //graphics_draw_round_rect(ctx, rect_bounds, corner_radius);
      graphics_fill_rect(ctx, rect_bounds, corner_radius, GCornersAll);

      // Step across one blob and add spacing
      rect_bounds.origin.x += BLOBSIZE;
      
    }
    //Move up to the next row
    rect_bounds.origin.y -= BLOBSIZE;
    rect_bounds.origin.x = offset_x + BLOBSIZE + 1;
  }
  //Draw the remainder
  rect_bounds.origin.y -= 1;
  for (i = 0; i < remainder; i++)
  {
    rect_bounds.origin.x += 1;
    
    //graphics_draw_round_rect(ctx, rect_bounds, corner_radius);
    graphics_fill_rect(ctx, rect_bounds, corner_radius, GCornersAll);

    // Step across one blob and add spacing
    rect_bounds.origin.x += BLOBSIZE;
    
  }
}

static void draw_hours (int hours, GContext *ctx) {
  GRect rect_bounds = GRect(0, 0, BLOBSIZE - 2, BLOBSIZE - 2);
  int corner_radius = 3;
  int i;
  
  //Skip to the bottom of the screen
  rect_bounds.origin.y = HEIGHT - BLOBSIZE - offset_y + 1;
  rect_bounds.origin.x = offset_x + 1;
  
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "hours is %d", hours);
  for (i = 1; i <= hours; i++)
  {
    graphics_fill_rect(ctx, rect_bounds, corner_radius, GCornersAll);
    rect_bounds.origin.y -=1; //Spacing
    rect_bounds.origin.y -= BLOBSIZE;   
  }
}

static void draw_text (GContext *ctx, Layer *layer){
  GRect text_rect = GRect(0, 0, BLOBSIZE, BLOBSIZE);
  //GRect bounds = layer_get_frame(layer);
  //graphics_context_set_text_color(ctx, GColorBlack);
  char str[3];
  int i;
  
  //Skip to the bottom of the screen
  text_rect.origin.y = HEIGHT - BLOBSIZE - offset_y - 3;
  
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
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_context_set_fill_color(ctx, GColorBlack);
  GRect rect_bounds = GRect (0, 0, WIDTH, HEIGHT);
  graphics_fill_rect(ctx, rect_bounds, 0, GCornersAll);
  
  graphics_context_set_stroke_color(ctx, GColorWhite);
  graphics_context_set_fill_color(ctx, GColorWhite);
  
  draw_grid(ctx);
  
  if (current_mode == show_time)
  {
    draw_hours(hours, ctx);
    draw_minutes(minutes, ctx);
  }
  else
  {
    draw_hours (month, ctx);
    draw_minutes (day, ctx);
  }
  //Draw the hours and minutes
  graphics_context_set_text_color(ctx, GColorWhite);
  draw_text (ctx, layer);
}