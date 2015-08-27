#include <pebble.h>
#include <stdio.h>
#include <ctype.h>

static Window *s_main_window;

static GFont s_time_font, s_date_font;
static BitmapLayer *clock_layer;
static GBitmap *clock_bitmap;

static TextLayer *s_time_layer, *date_layer;

// Create a long-lived buffer
static char buffer[] = "00:00";
static char date[] = "WEDNESDAY 00";

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
  
  BatteryChargeState state = battery_state_service_peek();

  #ifdef PBL_COLOR
  switch(state.charge_percent) {
    case 20: text_layer_set_text_color(s_time_layer, GColorOrange); break;
    case 10: text_layer_set_text_color(s_time_layer, GColorRed); break;
    case 0: text_layer_set_text_color(s_time_layer, GColorRed); break;
    default: text_layer_set_text_color(s_time_layer, GColorBlack); break;
  }
  #else
      text_layer_set_text_color(s_time_layer, GColorBlack); 
  #endif 
   
  gbitmap_destroy(clock_bitmap);
  
  switch(tick_time->tm_hour) {
     case 0:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_24); break;
     case 1:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_01); break;
     case 2:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_02); break;
     case 3:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_03); break;
     case 4:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_04); break;
     case 5:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_05); break;
     case 6:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_06); break;
     case 7:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_07); break;
     case 8:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_08); break;
     case 9:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_09); break;
     case 10:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_10); break;
     case 11:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_11); break;
     case 12:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_12); break;
     case 13:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_13); break;
     case 14:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_14); break;
     case 15:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_15); break;
     case 16:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_16); break;
     case 17:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_17); break;
     case 18:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_18); break;
     case 19:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_19); break;
     case 20:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_20); break;
     case 21:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_21); break;
     case 22:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_22); break;
     case 23:   clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_23); break;
     default: clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_23); break;
  }
  
  bitmap_layer_set_bitmap(clock_layer, clock_bitmap); 
  
  text_layer_set_text(s_time_layer, buffer);
  text_layer_set_text(date_layer, date);
}

static void set_text_to_window() {

  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_mario_36));
  //Time TextLayer 
  s_time_layer = text_layer_create(GRect(0, 90, 144, 40));
  text_layer_set_background_color(s_time_layer, GColorClear);
  #ifdef PBL_COLOR
    text_layer_set_text_color(s_time_layer, GColorRed);
  #else
    text_layer_set_text_color(s_time_layer, GColorBlack);
  #endif 
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
 
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_mario_16)); 
  //Time TextLayer 
  date_layer = text_layer_create(GRect(0, 142, 144, 50));
  text_layer_set_background_color(date_layer, GColorClear);
  text_layer_set_text_color(date_layer, GColorWhite);
  text_layer_set_text(date_layer,  "SEPTEMBER 00");
  text_layer_set_font(date_layer, s_date_font);
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
}

static void main_window_load(Window *window) {
  //ACTION: Create GBitmap, then set to created BitmapLayer
  clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_12);
  clock_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
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
  
  fonts_unload_custom_font(s_time_font);

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


