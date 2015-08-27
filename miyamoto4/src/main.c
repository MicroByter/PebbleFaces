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
  GRect bounds = layer_get_bounds(layer);
  GPoint center = grect_center_point(&bounds);
  int16_t second_hand_length = bounds.size.w / 5.2;
  
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  
  gbitmap_destroy(clock_bitmap);

  switch(t->tm_hour) {
     case 0:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_h24); break;
     case 1:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_h01); break;
     case 2:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_h02); break;
     case 3:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_h03); break;
     case 4:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_h04); break;
     case 5:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_h05); break;
     case 6:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_h06); break;
     case 7:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_h07); break;
     case 8:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_h08); break;
     case 9:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_h09); break;
     case 10:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_h10); break;
     case 11:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_h11); break;
     case 12:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_h12); break;
     case 13:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_h13); break;
     case 14:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_h14); break;
     case 15:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_h15); break;
     case 16:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_h16); break;
     case 17:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_h17); break;
     case 18:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_h18); break;
     case 19:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_h19); break;
     case 20:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_h20); break;
     case 21:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_h21); break;
     case 22:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_h22); break;
     case 23:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_h23); break;
     default: clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_h23); break;
  }
   
  bitmap_layer_set_bitmap(clock_layer, clock_bitmap);
  
  int32_t second_angle = TRIG_MAX_ANGLE * t->tm_sec / 60;
  GPoint second_hand = {
    .x = (int16_t)(sin_lookup(second_angle) * (int32_t)second_hand_length / TRIG_MAX_RATIO) + center.x,
    .y = (int16_t)(-cos_lookup(second_angle) * (int32_t)second_hand_length / TRIG_MAX_RATIO) + center.y,
  };

  // second hand
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_draw_line(ctx, second_hand, center);

  // minute/hour hand
  //GColorCobaltBlue
  graphics_context_set_fill_color(ctx, GColorCobaltBlue);
  graphics_context_set_stroke_color(ctx, GColorCobaltBlue);

  gpath_rotate_to(s_minute_arrow, TRIG_MAX_ANGLE * t->tm_min / 60);
  gpath_draw_filled(ctx, s_minute_arrow);

//GColorJaegerGreen
  graphics_context_set_fill_color(ctx, GColorJaegerGreen);
  graphics_context_set_stroke_color(ctx, GColorJaegerGreen);

  gpath_draw_outline(ctx, s_minute_arrow);

  gpath_rotate_to(s_hour_arrow, (TRIG_MAX_ANGLE * (((t->tm_hour % 12) * 6) + (t->tm_min / 10))) / (12 * 6));
  gpath_draw_filled(ctx, s_hour_arrow);
  gpath_draw_outline(ctx, s_hour_arrow);

  // dot in the middle
  graphics_context_set_fill_color(ctx, GColorRed);
  graphics_fill_rect(ctx, GRect(bounds.size.w / 2 - 1, bounds.size.h / 2 - 1, 3, 3), 0, GCornerNone);
}

static void date_update_proc(Layer *layer, GContext *ctx) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  strftime(s_day_buffer, sizeof(s_day_buffer), "%a", t);
//  text_layer_set_text(s_day_label, s_day_buffer);

  strftime(s_num_buffer, sizeof(s_num_buffer), "%d", t);
//  text_layer_set_text(s_num_label, s_num_buffer);
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

  /*  
  s_simple_bg_layer = layer_create(bounds);
  layer_set_update_proc(s_simple_bg_layer, bg_update_proc);
  layer_add_child(window_layer, s_simple_bg_layer);
*/
  s_date_layer = layer_create(bounds);
  layer_set_update_proc(s_date_layer, date_update_proc);
  layer_add_child(window_layer, s_date_layer);

/*  s_day_label = text_layer_create(GRect(46, 120, 27, 20));
  text_layer_set_text(s_day_label, s_day_buffer);
  text_layer_set_background_color(s_day_label, GColorClear);
  text_layer_set_text_color(s_day_label, GColorBlack);
  text_layer_set_font(s_day_label, fonts_get_system_font(FONT_KEY_GOTHIC_18));

  layer_add_child(s_date_layer, text_layer_get_layer(s_day_label));

  s_num_label = text_layer_create(GRect(73, 120, 18, 20));
  text_layer_set_text(s_num_label, s_num_buffer);
  text_layer_set_background_color(s_num_label, GColorClear);
  text_layer_set_text_color(s_num_label, GColorBlack);
  text_layer_set_font(s_num_label, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));

  layer_add_child(s_date_layer, text_layer_get_layer(s_num_label));
*/
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