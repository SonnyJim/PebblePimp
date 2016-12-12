#include <pebble.h>
/* Pebble classic resolution is 144×168 */
/* Pimp watch resolution is 6x12 */

#define BLOBSIZE 12
#define WIDTH 144
#define HEIGHT 168

int hours, minutes, hours_gmt, dst_gmt;
int month, day;

extern void canvas_update_proc(Layer *layer, GContext *ctx);