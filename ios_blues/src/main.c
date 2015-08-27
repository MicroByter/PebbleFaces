#include <pebble.h>
#include <stdio.h>
#include <ctype.h>
  
static Window *s_main_window;

static TextLayer *s_time_layer;

static GFont s_time_font;

static BitmapLayer *clock_layer;

static GBitmap *clock_bitmap;
 
// Create a long-lived buffer
static char buffer[] = "Pebble Time 00:00";

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  if (tick_time->tm_sec % 2 == 0) {
    strftime(buffer, sizeof("Pebble Time 00:00"), "Pebble Time %H %M", tick_time);
  } else {
    strftime(buffer, sizeof("Pebble Time 00:00"), "Pebble Time %H:%M", tick_time);
  }    

  text_layer_set_text(s_time_layer, buffer);
}

static void set_text_to_window() {

  //Time TextLayer 
  s_time_layer = text_layer_create(GRect(10, 145, 130, 40));
  text_layer_set_background_color(s_time_layer, GColorClear);
    text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "Pebble Time 00:00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

}

static void main_window_load(Window *window) {
  //ACTION: Create GBitmap, then set to created BitmapLayer
  clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ios_bg);
  clock_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(clock_layer, clock_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(clock_layer));


  set_text_to_window();
    
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  
  // Make sure the time is displayed from the start
  update_time();
}

static void main_window_unload(Window *window) {
    

  //Destroy GBitmap
  gbitmap_destroy(clock_bitmap);

  //Destroy BitmapLayer
  bitmap_layer_destroy(clock_layer);
  
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}
  
static void bt_handler(bool connected) {
  
}
static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  bluetooth_connection_service_subscribe(bt_handler);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
  tick_timer_service_unsubscribe();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}


