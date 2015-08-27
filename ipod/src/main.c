#include <pebble.h>
#include "main.h"

static Window *window;
static Layer *s_simple_bg_layer, *s_hands_layer;

static BitmapLayer *clock_layer;
static GBitmap *clock_bitmap;

static void hands_update_proc(Layer *layer, GContext *ctx) {

  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  
  GRect bounds = layer_get_bounds(layer);
  GPoint center = grect_center_point(&bounds);

  //HOUR
  int32_t second_angle = (TRIG_MAX_ANGLE * (((t->tm_hour % 12) * 6) + (t->tm_min / 10))) / (12 * 6);
  
  GPoint second_hand = {
    .x = (int16_t)(-sin_lookup(second_angle) * -40 / TRIG_MAX_RATIO) + center.x,
    .y = (int16_t)(cos_lookup(second_angle) * -40 / TRIG_MAX_RATIO) + center.y,
  };
     
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_draw_circle(ctx, second_hand, 11);
  graphics_fill_circle(ctx, second_hand, 11);
  
  #ifdef PBL_COLOR
    graphics_context_set_stroke_color(ctx, GColorLightGray);
    graphics_context_set_fill_color(ctx, GColorLightGray);
  #else
    graphics_context_set_stroke_color(ctx, GColorWhite);
    graphics_context_set_fill_color(ctx, GColorWhite);
  #endif
   
  graphics_draw_circle(ctx, second_hand, 9);
  graphics_fill_circle(ctx, second_hand, 9);


  
  // MINUTE //
  int32_t minute_angle = (TRIG_MAX_ANGLE * t->tm_min / 60);
  
  GPoint minute_hand = {
    .x = (int16_t)(-sin_lookup(minute_angle) * -60 / TRIG_MAX_RATIO) + center.x,
    .y = (int16_t)(cos_lookup(minute_angle) * -60 / TRIG_MAX_RATIO) + center.y,
  };
  
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_draw_circle(ctx, minute_hand, 8);
  graphics_fill_circle(ctx, minute_hand, 8);

  #ifdef PBL_COLOR
    graphics_context_set_stroke_color(ctx, GColorLightGray);
    graphics_context_set_fill_color(ctx, GColorLightGray);
  #else
    graphics_context_set_stroke_color(ctx, GColorWhite);
    graphics_context_set_fill_color(ctx, GColorWhite);
  #endif
  
  graphics_draw_circle(ctx, minute_hand, 6);
  graphics_fill_circle(ctx, minute_hand, 6);
}

static void handle_second_tick(struct tm *tick_time, TimeUnits units_changed) {
  layer_mark_dirty(window_get_root_layer(window));
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  //ACTION: Create GBitmap, then set to created BitmapLayer
  clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_bg_image);
  clock_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(clock_layer, clock_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(clock_layer));
   
  s_hands_layer = layer_create(bounds);
  layer_set_update_proc(s_hands_layer, hands_update_proc);
  layer_add_child(window_layer, s_hands_layer);
}

static void window_unload(Window *window) {
  layer_destroy(s_simple_bg_layer);
  layer_destroy(s_hands_layer);
  gbitmap_destroy(clock_bitmap);
  bitmap_layer_destroy(clock_layer);

}

static void init() {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(window, true);
    
  tick_timer_service_subscribe(MINUTE_UNIT, handle_second_tick);
}

static void deinit() {
  tick_timer_service_unsubscribe();
  window_destroy(window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}