#include <pebble.h>
#include <stdio.h>
#include <ctype.h>

static Window *s_main_window;

static BitmapLayer *clock_layer;
static GBitmap *clock_bitmap;

static GFont hour_font, date_font;

static TextLayer *s_time_layer, *date_layer;

// Create a long-lived buffer
static char buffer[] = "00:00";
static char date[] = "WEDNESDAY 00";

static int looper  = 0;
static const uint32_t anim[4] = {
  RESOURCE_ID_0, //0
  RESOURCE_ID_1, //1
  RESOURCE_ID_2, //2
  RESOURCE_ID_3 //3
};

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  
  if(clock_is_24h_style() == true) {
    //Use 2h hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
   } else {
    //Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }
  strftime(date, sizeof(date), "%A %d", tick_time);
  
  gbitmap_destroy(clock_bitmap);

  
  if (looper < 3) {
     clock_bitmap = gbitmap_create_with_resource(anim[looper]); 
     looper += 1;
   } else {
     clock_bitmap = gbitmap_create_with_resource(anim[looper]); 
     looper = 0;
   }
    bitmap_layer_set_bitmap(clock_layer, clock_bitmap); 
  
  text_layer_set_text(s_time_layer, buffer);
  text_layer_set_text(date_layer, date);
}

static void set_text_to_window() {

  hour_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_off_48));
  //Time TextLayer 
  s_time_layer = text_layer_create(GRect(0, -8, 144, 48));
  text_layer_set_background_color(s_time_layer, GColorClear);
    text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, hour_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
 
  date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_off_24)); 
  //Time TextLayer 
  date_layer = text_layer_create(GRect(0, 140, 144, 24));
  text_layer_set_background_color(date_layer, GColorClear);
  text_layer_set_text_color(date_layer, GColorWhite);
  text_layer_set_text(date_layer,  "SEPTEMBER 00");
  text_layer_set_font(date_layer,date_font);
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
}

static void main_window_load(Window *window) {
  //ACTION: Create GBitmap, then set to created BitmapLayer
  clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_0);
  clock_layer = bitmap_layer_create(GRect(0, 45, 144, 96));
  bitmap_layer_set_bitmap(clock_layer, clock_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(clock_layer));

  set_text_to_window();

  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));
  
  // Make sure the time is displayed from the start
  update_time();
}

static void main_window_unload(Window *window) {
  gbitmap_destroy(clock_bitmap);
  bitmap_layer_destroy(clock_layer);
 fonts_unload_custom_font(hour_font);
 fonts_unload_custom_font(date_font);

//  fonts_unload_custom_font(s_time_font);

  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(date_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}
  
static void bt_handler(bool connected) {
  
}
static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorBlack);

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
