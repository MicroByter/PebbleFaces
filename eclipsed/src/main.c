#include <pebble.h>
#include "main.h"

static Window *window;
static Layer *s_simple_bg_layer, *s_hands_layer;

static TextLayer  *date_layer, *time_layer;

static char date[] = "WED 00";
static char buffer[] = "00:00"; 
static GFont s_date_font;
static GFont s_time_font;

static BitmapLayer *clock_layer;
static GBitmap *clock_bitmap;

static GPath *s_minute_arrow, *s_hour_arrow, *s_second_arrow;

static void hands_update_proc(Layer *layer, GContext *ctx) {

  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  
  GRect bounds = layer_get_bounds(layer);
  GPoint center = grect_center_point(&bounds);
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
   
/*  // HOUR HAND //
  gpath_rotate_to(s_hour_arrow, (TRIG_MAX_ANGLE * (((t->tm_hour % 12) * 6) + (t->tm_min / 10))) / (12 * 6));
  #ifdef PBL_COLOR
    graphics_context_set_fill_color(ctx, GColorYellow);
    graphics_context_set_stroke_color(ctx, GColorYellow);
  #else
    graphics_context_set_fill_color(ctx, GColorWhite);
    graphics_context_set_stroke_color(ctx, GColorWhite);
  #endif
  gpath_draw_filled(ctx, s_hour_arrow);
  gpath_draw_outline(ctx, s_hour_arrow);
*/  
  // MINUTE HAND //
  gpath_rotate_to(s_minute_arrow, TRIG_MAX_ANGLE * t->tm_min / 60);
  #ifdef PBL_COLOR
    graphics_context_set_fill_color(ctx, GColorWhite);
    graphics_context_set_stroke_color(ctx, GColorWhite);
  #else
    graphics_context_set_fill_color(ctx, GColorWhite);
    graphics_context_set_stroke_color(ctx, GColorWhite);
  #endif
  gpath_draw_filled(ctx, s_minute_arrow);
  gpath_draw_outline(ctx, s_minute_arrow);

  // SECOND HAND //
  gpath_rotate_to(s_second_arrow, (TRIG_MAX_ANGLE * t->tm_sec / 60));
  #ifdef PBL_COLOR
    graphics_context_set_fill_color(ctx, GColorRed);
    graphics_context_set_stroke_color(ctx, GColorRed);
  #else
    graphics_context_set_fill_color(ctx, GColorWhite);
    graphics_context_set_stroke_color(ctx, GColorWhite);
  #endif
  gpath_draw_filled(ctx, s_second_arrow);
  gpath_draw_outline(ctx, s_second_arrow);
 
  if(clock_is_24h_style() == true) {
    //Use 2h hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", t);
  } else {
    //Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", t);
  }       
 
  strftime(date, sizeof("XXX XX"), "%a %d", t);
  text_layer_set_text(date_layer, date);
  text_layer_set_text(time_layer, buffer);

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

  
  //Time TextLayer 
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_mermaid_10)); 
  date_layer = text_layer_create(GRect(50, 97, 45, 10));
  text_layer_set_background_color(date_layer, GColorClear);
  text_layer_set_text_color(date_layer, GColorWhite);
  text_layer_set_text(date_layer,  "Wed 00");
  text_layer_set_font(date_layer, s_date_font);
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));

  //Time TextLayer 
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_mermaid_18)); 
  time_layer = text_layer_create(GRect(43, 73, 58, 19));
  text_layer_set_background_color(time_layer, GColorBlack);
  text_layer_set_text_color(time_layer, GColorWhite);
  text_layer_set_text(time_layer,  "00:00");
  text_layer_set_font(time_layer, s_time_font);
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer));

  
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
  fonts_unload_custom_font(s_time_font);
  text_layer_destroy(date_layer);
  text_layer_destroy(time_layer);

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
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  GPoint center = grect_center_point(&bounds);
  gpath_move_to(s_minute_arrow, center);
  gpath_move_to(s_hour_arrow, center);
  gpath_move_to(s_second_arrow, center);

  tick_timer_service_subscribe(SECOND_UNIT, handle_second_tick);
}

static void deinit() {
  gpath_destroy(s_minute_arrow);
  gpath_destroy(s_hour_arrow);
  gpath_destroy(s_second_arrow);
  tick_timer_service_unsubscribe();
  window_destroy(window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}