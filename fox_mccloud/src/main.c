#include <pebble.h>
#include "main.h"

static Window *window;
static Layer *s_simple_bg_layer, *s_hands_layer;

static TextLayer  *month_layer, *date_layer;

static char month[] = "00";
static char date[] = "00";
static GFont s_date_font;
static GFont s_month_font;

static BitmapLayer *clock_layer;
static GBitmap *clock_bitmap;

static GPath *s_minute_arrow, *s_hour_arrow, *s_second_arrow, *s_second_arrow2;

static void hands_update_proc(Layer *layer, GContext *ctx) {

  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  
  GRect bounds = layer_get_bounds(layer);
  GPoint center = grect_center_point(&bounds);

  /*
  int32_t second_angle = (TRIG_MAX_ANGLE * (((t->tm_hour % 12) * 6) + (t->tm_min / 10))) / (12 * 6);
  
  GPoint second_hand = {
    .x = (int16_t)(-sin_lookup(second_angle) * -45 / TRIG_MAX_RATIO) + center.x,
    .y = (int16_t)(cos_lookup(second_angle) * -45 / TRIG_MAX_RATIO) + center.y,
  };

  #ifdef PBL_COLOR
    graphics_context_set_stroke_color(ctx, GColorYellow);
    graphics_context_set_fill_color(ctx, GColorYellow);
  #else
    graphics_context_set_stroke_color(ctx, GColorWhite);
    graphics_context_set_fill_color(ctx, GColorWhite);
  #endif
    
  graphics_draw_circle(ctx, second_hand, 4);
  graphics_fill_circle(ctx, second_hand, 4);
*/   
  // HOUR HAND //
  gpath_rotate_to(s_hour_arrow, (TRIG_MAX_ANGLE * (((t->tm_hour % 12) * 6) + (t->tm_min / 10))) / (12 * 6));
  #ifdef PBL_COLOR
    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_context_set_stroke_color(ctx, GColorBlack);
  #else
    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_context_set_stroke_color(ctx, GColorBlack);
  #endif
  gpath_draw_filled(ctx, s_hour_arrow);
  gpath_draw_outline(ctx, s_hour_arrow);
  
  // MINUTE HAND //
  gpath_rotate_to(s_minute_arrow, TRIG_MAX_ANGLE * t->tm_min / 60);
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_context_set_stroke_color(ctx, GColorBlack);
  gpath_draw_filled(ctx, s_minute_arrow);
  gpath_draw_outline(ctx, s_minute_arrow);

  // SECOND HAND //
  gpath_rotate_to(s_second_arrow, (TRIG_MAX_ANGLE * t->tm_sec / 60));
  #ifdef PBL_COLOR
    graphics_context_set_fill_color(ctx, GColorRed);
    graphics_context_set_stroke_color(ctx, GColorRed);
  #else
    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_context_set_stroke_color(ctx, GColorBlack);
  #endif
  gpath_draw_filled(ctx, s_second_arrow);
  gpath_draw_outline(ctx, s_second_arrow);

  gpath_rotate_to(s_second_arrow2, (TRIG_MAX_ANGLE * t->tm_sec / 60));
  #ifdef PBL_COLOR
    graphics_context_set_fill_color(ctx, GColorRed);
    graphics_context_set_stroke_color(ctx, GColorRed);
  #else
    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_context_set_stroke_color(ctx, GColorBlack);
  #endif
  gpath_draw_filled(ctx, s_second_arrow2);
  gpath_draw_outline(ctx, s_second_arrow2);

  

  //CENTER CIRCLES
  GPoint center_circle = {
    .x = center.x,
    .y = center.y,
  };

  #ifdef PBL_COLOR
    graphics_context_set_stroke_color(ctx, GColorRed);
    graphics_context_set_fill_color(ctx, GColorRed);
  #else
    graphics_context_set_stroke_color(ctx, GColorBlack);
    graphics_context_set_fill_color(ctx, GColorBlack);
  #endif
    
  graphics_draw_circle(ctx, center_circle, 4);
  graphics_fill_circle(ctx, center_circle, 4);

  
    GPoint center_circle2 = {
    .x = center.x,
    .y = center.y,
  };

  graphics_context_set_stroke_color(ctx, GColorWhite);
  graphics_context_set_fill_color(ctx, GColorWhite);
    
  graphics_draw_circle(ctx, center_circle2, 2);
  graphics_fill_circle(ctx, center_circle2, 2);

  
  
  strftime(date, sizeof("XX"), "%d", t);
  strftime(month, sizeof("XXX"), "%a", t);

  text_layer_set_text(date_layer, date);
  text_layer_set_text(month_layer, month);

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

  //Month TextLayer 
  s_month_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_mermaid_10)); 
  month_layer = text_layer_create(GRect(52, 99, 25, 10));
  text_layer_set_background_color(month_layer, GColorClear);
  text_layer_set_text_color(month_layer, GColorBlack);
  text_layer_set_text(month_layer,  "00");
  text_layer_set_font(month_layer, s_month_font);
  text_layer_set_text_alignment(month_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(month_layer));
  
  //date TextLayer 
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_mermaid_10)); 
  date_layer = text_layer_create(GRect(77, 99, 15, 10));
  text_layer_set_background_color(date_layer, GColorClear);
  text_layer_set_text_color(date_layer, GColorWhite);
  text_layer_set_text(date_layer,  "00");
  text_layer_set_font(date_layer, s_date_font);
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));

  s_hands_layer = layer_create(bounds);
  layer_set_update_proc(s_hands_layer, hands_update_proc);
  layer_add_child(window_layer, s_hands_layer);
}

static void window_unload(Window *window) {
  layer_destroy(s_simple_bg_layer);
  layer_destroy(s_hands_layer);
  gbitmap_destroy(clock_bitmap);
  bitmap_layer_destroy(clock_layer);
  fonts_unload_custom_font(s_date_font);
  fonts_unload_custom_font(s_month_font);
  text_layer_destroy(date_layer);
  text_layer_destroy(month_layer);
}

static void init() {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(window, true);

  // init hand paths
  s_minute_arrow = gpath_create(&MINUTE_HAND_POINTS);
  s_hour_arrow = gpath_create(&HOUR_HAND_POINTS);
  s_second_arrow = gpath_create(&SECOND_HAND_POINTS);
  s_second_arrow2 = gpath_create(&SECOND_HAND_POINTS2);
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  GPoint center = grect_center_point(&bounds);
  gpath_move_to(s_minute_arrow, center);
  gpath_move_to(s_hour_arrow, center);
  gpath_move_to(s_second_arrow, center);
  gpath_move_to(s_second_arrow2, center);

  tick_timer_service_subscribe(SECOND_UNIT, handle_second_tick);
}

static void deinit() {
  gpath_destroy(s_minute_arrow);
  gpath_destroy(s_hour_arrow);
  gpath_destroy(s_second_arrow);
  gpath_destroy(s_second_arrow2);
  tick_timer_service_unsubscribe();
  window_destroy(window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}