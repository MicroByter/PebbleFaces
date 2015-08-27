#include <pebble.h>
#include <stdio.h>
#include <ctype.h>

static Window *s_main_window;

static BitmapLayer *clock_layer, *gw_top_layer, *gw_bottom_layer;
static GBitmap *clock_bitmap, *gw_top_bitmap, *gw_bottom_bitmap;

static BitmapLayer *batt_layer;
static GBitmap *batt_bitmap;

static GFont hour_font, date_font;

static TextLayer *s_time_layer, *date_layer;

// Create a long-lived buffer
static char buffer[] = "00:00";
static char date[] = "WEDNESDAY 00";

static int looper  = 0;
static const uint32_t anim[23] = {
  RESOURCE_ID_h02, //1
  RESOURCE_ID_h04, //0
  RESOURCE_ID_h05, //1
  RESOURCE_ID_h06, //2
  RESOURCE_ID_h07, //0
  RESOURCE_ID_h08, //1
  RESOURCE_ID_h10, //0
  RESOURCE_ID_h11, //1
  RESOURCE_ID_h12, //2
  RESOURCE_ID_h14, //1
  RESOURCE_ID_h15, //3
  RESOURCE_ID_h16, //3
  RESOURCE_ID_h17, //3
  RESOURCE_ID_h18, //3
  RESOURCE_ID_h20, //3
  RESOURCE_ID_h21, //3
  RESOURCE_ID_h22, //3
  RESOURCE_ID_h23, //3
  RESOURCE_ID_h24, //3
  RESOURCE_ID_h25, //3
  RESOURCE_ID_h26, //3
  RESOURCE_ID_h27, //3
  RESOURCE_ID_h28  //3
};


static void set_battery() {
  
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  BatteryChargeState state = battery_state_service_peek();
  
  gbitmap_destroy(batt_bitmap);
  
  switch (state.charge_percent) {
    case 100:
    case 90: 
    case 80: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt100); break;
    case 70: 
    case 60: 
    case 50: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt70); break;
    case 40: 
    case 30: 
    case 20: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt40); break;
    case 10: 
       if ( tick_time->tm_sec % 2 == 0) {
          batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt100); 
       } else {
          batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt10);          
       }
       break;
     case 00: 
       if ( tick_time->tm_sec % 2 == 0) {
          batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt100); 
       } else {
          batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt10);          
       }
       break;
    default: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt100); break;
  }
  bitmap_layer_set_bitmap(batt_layer, batt_bitmap); 
}


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

  
  if (looper < 22) {
     clock_bitmap = gbitmap_create_with_resource(anim[looper]); 
     looper += 1;
   } else {
     clock_bitmap = gbitmap_create_with_resource(anim[looper]); 
     looper = 0;
   }
    bitmap_layer_set_bitmap(clock_layer, clock_bitmap); 
  
  set_battery();
 
  text_layer_set_text(s_time_layer, buffer);
  text_layer_set_text(date_layer, date);
}

static void set_text_to_window() {

  hour_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_poke_24));
  //Time TextLayer 
  s_time_layer = text_layer_create(GRect(0, 123, 144, 32));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, hour_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
 
  date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_poke_12)); 
  //Time TextLayer 
  date_layer = text_layer_create(GRect(0, 150, 144, 14));
  text_layer_set_background_color(date_layer, GColorClear);
  text_layer_set_text_color(date_layer, GColorBlack);
  text_layer_set_text(date_layer,  "WEDNESDAY 00");
  text_layer_set_font(date_layer,date_font);
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
}

static void main_window_load(Window *window) {
  //ACTION: Create GBitmap, then set to created BitmapLayer
  clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_h02);
  clock_layer = bitmap_layer_create(GRect(0, -23, 144, 168));
  bitmap_layer_set_bitmap(clock_layer, clock_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(clock_layer));

  gw_top_bitmap = gbitmap_create_with_resource(RESOURCE_ID_gw_top);
  gw_top_layer = bitmap_layer_create(GRect(0, 0, 144, 12));
  bitmap_layer_set_bitmap(gw_top_layer, gw_top_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(gw_top_layer));

  gw_bottom_bitmap = gbitmap_create_with_resource(RESOURCE_ID_gw_bottom);
  gw_bottom_layer = bitmap_layer_create(GRect(0, 109, 144, 92));
  bitmap_layer_set_bitmap(gw_bottom_layer, gw_bottom_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(gw_bottom_layer));

  batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt100);
  batt_layer = bitmap_layer_create(GRect(104, 24, 35, 11));
  bitmap_layer_set_bitmap(batt_layer, batt_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(batt_layer));

  
  set_text_to_window();

  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));
  
  // Make sure the time is displayed from the start
  update_time();
}

static void main_window_unload(Window *window) {
  gbitmap_destroy(clock_bitmap);
  bitmap_layer_destroy(clock_layer);
  gbitmap_destroy(gw_top_bitmap);
  bitmap_layer_destroy(gw_top_layer);
  gbitmap_destroy(gw_bottom_bitmap);
  bitmap_layer_destroy(gw_bottom_layer);
  gbitmap_destroy(batt_bitmap);
  bitmap_layer_destroy(batt_layer);
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