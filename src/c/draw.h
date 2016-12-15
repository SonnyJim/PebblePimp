#pragma once
void draw_grid (GContext *ctx);
void draw_text (GContext *ctx);
void draw_face_array (GContext *ctx);
void face_fill (int x, int y, GContext *ctx);
void draw_gmt (GContext *ctx);

int offset_x, offset_y;

extern bool face_array[FACE_WIDTH][FACE_HEIGHT];