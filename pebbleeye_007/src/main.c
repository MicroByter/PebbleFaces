#include <pebble.h>
#include <stdio.h>
#include <ctype.h>
  
static Window *s_main_window;

static TextLayer *s_time_layer;
static TextLayer *current_date_layer;
static TextLayer *timephase_layer;

static GFont s_time_font;
static GFont s_timephase_font;
static GFont s_date_font;

static BitmapLayer *clock_layer;
static BitmapLayer *batt_layer;
static BitmapLayer *bt_layer;

static GBitmap *clock_bitmap;
static GBitmap *batt_bitmap;
static GBitmap *bt_bitmap;

// Create a long-lived buffer
static char buffer[] = "00:00";
static char current_date_buffer[] = "00.00 000";
static char timephase_buffer[] = "00";

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  
  if(clock_is_24h_style() == true) {
    //Use 2h hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
    strftime(timephase_buffer, sizeof("00"), "  ", tick_time);
      
  } else {
    //Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
    strftime(timephase_buffer, sizeof("00"), "%p", tick_time);
  }       
  
  strftime(current_date_buffer, sizeof("00.00 000"), "%m.%d %a", tick_time);

  int i=0;
  char c;
  while (current_date_buffer[i])
  {
    c = current_date_buffer[i];
    current_date_buffer[i] = toupper((unsigned char)c);
    i++;
  }  //write to all text layers

  gbitmap_destroy(bt_bitmap);

  if (bluetooth_connection_service_peek()) {
     bt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_bt_on);
  } else {
     bt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_bt_off);  
  }
  bitmap_layer_set_bitmap(bt_layer, bt_bitmap);

  BatteryChargeState state = battery_state_service_peek();
  gbitmap_destroy(batt_bitmap);
  
  switch (state.charge_percent) {
    case 90: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt100);
       break;
    case 80: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt80);
       break;
    case 70: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt70);
       break;
    case 60: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt60);
       break;
    case 50: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt50);
       break;
    case 40: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt40);
       break;
    case 30: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt30);
       break;
    case 20: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt20);
       break;
    case 10: 
       if ( tick_time->tm_sec % 2 == 0)
          batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt00);
       else
          batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt10);       
       break;
    case 00: 
       if ( tick_time->tm_sec % 2 == 0)
          batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt00);
       else
          batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt10);       
       break;
    default: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt100);
       break;
  }
  
  bitmap_layer_set_bitmap(batt_layer, batt_bitmap);
    
  text_layer_set_text(s_time_layer, buffer);
  text_layer_set_text(timephase_layer, timephase_buffer);
  text_layer_set_text(current_date_layer, current_date_buffer);
}

static void set_text_to_window() {

  //Time TextLayer 
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CRYSTAL_36));
  s_time_layer = text_layer_create(GRect(5, 50, 130, 40));
  text_layer_set_background_color(s_time_layer, GColorClear);
    text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  //Time TextLayer 
  s_timephase_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CRYSTAL_18));
  timephase_layer = text_layer_create(GRect(85, 85, 30, 30));
  text_layer_set_background_color(timephase_layer, GColorClear);
  text_layer_set_text_color(timephase_layer, GColorWhite);
  text_layer_set_text(timephase_layer, "00");
  text_layer_set_font(timephase_layer, s_timephase_font);
  text_layer_set_text_alignment(timephase_layer, GTextAlignmentCenter);
  
  // Create current date TextLayer 
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CRYSTAL_18));
  current_date_layer = text_layer_create(GRect(12, 133, 120, 30));
  text_layer_set_background_color(current_date_layer, GColorClear);
  text_layer_set_text_color(current_date_layer, GColorWhite);
  text_layer_set_text(current_date_layer, "00.00 000");
  text_layer_set_font(current_date_layer, s_date_font);
  text_layer_set_text_alignment(current_date_layer, GTextAlignmentCenter);
}

static void main_window_load(Window *window) {
  //ACTION: Create GBitmap, then set to created BitmapLayer
  clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_bg_image);
  clock_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(clock_layer, clock_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(clock_layer));

  //BATTERY: Create GBitmap, then set to created BitmapLayer
  batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt100);
  batt_layer = bitmap_layer_create(GRect(0 ,0, 44, 168));
  bitmap_layer_set_bitmap(batt_layer, batt_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(batt_layer));
  
    //BATTERY: Create GBitmap, then set to created BitmapLayer
  bt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_bt_off);
  bt_layer = bitmap_layer_create(GRect( 98, 0, 46, 168));
  bitmap_layer_set_bitmap(bt_layer, bt_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(bt_layer));
  

  
  set_text_to_window();
    
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(current_date_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(timephase_layer));
  
  // Make sure the time is displayed from the start
  update_time();
}

static void main_window_unload(Window *window) {
    
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_timephase_font);
  fonts_unload_custom_font(s_date_font);

  //Destroy GBitmap
  gbitmap_destroy(clock_bitmap);
  gbitmap_destroy(batt_bitmap);
  gbitmap_destroy(bt_bitmap);

  //Destroy BitmapLayer
  bitmap_layer_destroy(clock_layer);
  bitmap_layer_destroy(batt_layer);
  bitmap_layer_destroy(bt_layer);
  
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(timephase_layer);
  text_layer_destroy(current_date_layer);
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

