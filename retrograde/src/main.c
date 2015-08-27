#include "main.h"
#include "pebble.h"

static Window *window;
static Layer *s_simple_bg_layer, *s_date_layer, *s_hands_layer;
static TextLayer *s_day_label, *s_num_label;

static BitmapLayer *clock_layer;
static GBitmap *clock_bitmap;
 

static GPath *s_minute_arrow, *s_hour_arrow;
static char s_num_buffer[4], s_day_buffer[6];

static void bg_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
  graphics_context_set_fill_color(ctx, GColorBlack);
}

static void hands_update_proc(Layer *layer, GContext *ctx) {

  //*************** SECONDS HAND *****************//
  //*************** SECONDS HAND *****************//
  //*************** SECONDS HAND *****************//
  // SECONDS HAND - WHITE LONG
  GRect bounds = layer_get_bounds(layer);
  GPoint center = grect_center_point(&bounds);
  int16_t second_hand_length = 55;
  
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  
  int32_t second_angle = TRIG_MAX_ANGLE * (22.5 + (.25 * t->tm_sec)) / 60;
  GPoint second_hand = {
    .x = (int16_t)(-sin_lookup(second_angle) * (int32_t)second_hand_length / TRIG_MAX_RATIO) + center.x,
    .y = (int16_t)(-cos_lookup(second_angle) * (int32_t)second_hand_length / TRIG_MAX_RATIO) + center.y,
  };

  #ifdef PBL_COLOR
     graphics_context_set_stroke_color(ctx, GColorRed);
  #else
     graphics_context_set_stroke_color(ctx, GColorWhite);
  #endif
  
  graphics_draw_line(ctx, second_hand, center);
  
  //SECONDS HAND - BLACK NUB IN CENTER 
  int16_t second_hand_length2 = 3;
  int32_t second_angle2 = TRIG_MAX_ANGLE * (22.5 + (.25 * t->tm_sec)) / 60;
  GPoint second_hand2 = {
    .x = (int16_t)(-sin_lookup(second_angle) * (int32_t)second_hand_length2 / TRIG_MAX_RATIO) + center.x,
    .y = (int16_t)(-cos_lookup(second_angle) * (int32_t)second_hand_length2 / TRIG_MAX_RATIO) + center.y,
  };
  
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_draw_line(ctx, second_hand2, center);
  
  //*************** MINUTE HAND *****************//
  //*************** MINUTE HAND *****************//
  //*************** MINUTE HAND *****************//
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_context_set_stroke_color(ctx, GColorWhite);

  if (t->tm_min <= 30)
     gpath_rotate_to(s_minute_arrow, TRIG_MAX_ANGLE * (0 + (t->tm_min * .5)) / 60);
  else 
     gpath_rotate_to(s_minute_arrow, TRIG_MAX_ANGLE *  (45 + (t->tm_min - 30) * .5) / 60);

  gpath_draw_filled(ctx, s_minute_arrow);
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_context_set_stroke_color(ctx, GColorWhite);
  gpath_draw_outline(ctx, s_minute_arrow);

    //MINUTE HAND - THIN LINE 
  int16_t min_hand_length2 = 62;
  int32_t min_angle2;
  if (t->tm_min <= 30)
     min_angle2 =  TRIG_MAX_ANGLE * (0 + (t->tm_min * .5)) / 60;
  else 
     min_angle2 =  TRIG_MAX_ANGLE *  (45 + (t->tm_min - 30) * .5) / 60;
  
  GPoint min_hand2 = {
    .x = (int16_t)(sin_lookup(min_angle2) * (int32_t)min_hand_length2 / TRIG_MAX_RATIO) + center.x,
    .y = (int16_t)(-cos_lookup(min_angle2) * (int32_t)min_hand_length2 / TRIG_MAX_RATIO) + center.y,
  };
  
  graphics_context_set_stroke_color(ctx, GColorWhite);
  graphics_draw_line(ctx, min_hand2, center);

  //MINUTE HAND - BLACK NUB 
  int16_t min_hand_length3 = 3;
  GPoint min_hand3 = {
    .x = (int16_t)(sin_lookup(min_angle2) * (int32_t)min_hand_length3 / TRIG_MAX_RATIO) + center.x,
    .y = (int16_t)(-cos_lookup(min_angle2) * (int32_t)min_hand_length3 / TRIG_MAX_RATIO) + center.y,
  };
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_draw_line(ctx, min_hand3, center);
  
  //*************** HOUR HAND *****************//
  //*************** HOUR HAND *****************//
  //*************** HOUR HAND *****************//
//  int hour  = 23;
  
//  if (hour <= 6 || hour >= 19)
//     gpath_rotate_to(s_hour_arrow, TRIG_MAX_ANGLE *  (0 + (hour * 2.5)) / 60);  
//  else
//     gpath_rotate_to(s_hour_arrow, TRIG_MAX_ANGLE *  (45 + (hour - 6) * 2.5) / 60);
  if (t->tm_hour <= 6 || t->tm_hour >= 19)
     gpath_rotate_to(s_hour_arrow, TRIG_MAX_ANGLE *  (0 + (t->tm_hour * 2.5)) / 60);  
  else
     gpath_rotate_to(s_hour_arrow, TRIG_MAX_ANGLE *  (45 + (t->tm_hour - 6) * 2.5) / 60);

  gpath_draw_filled(ctx, s_hour_arrow);
  gpath_draw_outline(ctx, s_hour_arrow);

      //HOUR HAND - THIN LINE 
  int16_t hour_hand_length2 = 38;
  int32_t hour_angle2;

  if (t->tm_hour <= 6 || t->tm_hour >= 19)
     hour_angle2 = TRIG_MAX_ANGLE *  (0 + (t->tm_hour * 2.5)) / 60;  
  else
     hour_angle2 = TRIG_MAX_ANGLE *  (45 + (t->tm_hour - 6) * 2.5) / 60;  

  GPoint hour_hand2 = {
    .x = (int16_t)(sin_lookup(hour_angle2) * (int32_t)hour_hand_length2 / TRIG_MAX_RATIO) + center.x,
    .y = (int16_t)(-cos_lookup(hour_angle2) * (int32_t)hour_hand_length2 / TRIG_MAX_RATIO) + center.y,
  };
  
  graphics_context_set_stroke_color(ctx, GColorWhite);
  graphics_draw_line(ctx, hour_hand2, center);
  
  //HOUR HAND - BLACK NUB 
  int16_t hour_hand_length3 = 3;
  GPoint hour_hand3 = {
    .x = (int16_t)(sin_lookup(hour_angle2) * (int32_t)hour_hand_length3 / TRIG_MAX_RATIO) + center.x,
    .y = (int16_t)(-cos_lookup(hour_angle2) * (int32_t)hour_hand_length3 / TRIG_MAX_RATIO) + center.y,
  };
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_draw_line(ctx, hour_hand3, center);

  
  
}

static void date_update_proc(Layer *layer, GContext *ctx) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  strftime(s_day_buffer, sizeof(s_day_buffer), "%a", t);
  strftime(s_num_buffer, sizeof(s_num_buffer), "%d", t);
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
  layer_destroy(s_date_layer);

  gbitmap_destroy(clock_bitmap);
  bitmap_layer_destroy(clock_layer);
  
  text_layer_destroy(s_day_label);
  text_layer_destroy(s_num_label);

  layer_destroy(s_hands_layer);
}

static void init() {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(window, true);

  s_day_buffer[0] = '\0';
  s_num_buffer[0] = '\0';

  // init hand paths
  s_minute_arrow = gpath_create(&MINUTE_HAND_POINTS);
  s_hour_arrow = gpath_create(&HOUR_HAND_POINTS);

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  GPoint center = grect_center_point(&bounds);
  gpath_move_to(s_minute_arrow, center);
  gpath_move_to(s_hour_arrow, center);

  tick_timer_service_subscribe(SECOND_UNIT, handle_second_tick);
}

static void deinit() {
  gpath_destroy(s_minute_arrow);
  gpath_destroy(s_hour_arrow);
  tick_timer_service_unsubscribe();
  window_destroy(window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}
