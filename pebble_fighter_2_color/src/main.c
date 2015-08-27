#include <pebble.h>
  
static Window *s_main_window;

static TextLayer *s_time_layer;
static TextLayer *s_second_layer;
static TextLayer *date_layer;

static GFont s_time_font;

static BitmapLayer *healthbar_layer; 
static BitmapLayer *batt_layer;
static BitmapLayer *action_layer;
static BitmapLayer *clock_layer;

static GBitmap *healthbar_bitmap;
static GBitmap *batt_bitmap;
static GBitmap *action_bitmap;
static GBitmap *clock_bitmap;
  
// Create a long-lived buffer
static char buffer[] = "00:00";
static char buffer2[] = "00";
static char date_value[] = "- 00.00.00 -";

static void set_battery() {
  BatteryChargeState state = battery_state_service_peek();
  
  gbitmap_destroy(batt_bitmap);
  
  switch (state.charge_percent) {
    case 90: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt90); break;
    case 80: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt80); break;
    case 70: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt70); break;
    case 60: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt60); break;
    case 50: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt50); break;
    case 40: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt40); break;
    case 30: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt30); break;
    case 20: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt20); break;
    case 10: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt10); break;
    case 00: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt00); break;
    default: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt100); break;
  }
  bitmap_layer_set_bitmap(batt_layer, batt_bitmap); 
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    //Use 2h hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
   } else {
    //Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }
   //strftime(buffer2, sizeof("00"), "%S", tick_time);
  snprintf(buffer2, sizeof("00"), "%d", (60 - tick_time->tm_sec));
  strftime(date_value, sizeof("- 00.00.00 -"), "- %m.%d.%y -", tick_time);
  int sec = tick_time->tm_sec;
  int min = tick_time->tm_min;
  
  //Destroy GBitmaps
  gbitmap_destroy(action_bitmap);
  if (sec < 51) { //regular stance
     if ( sec % 2 == 0)
     {
       action_bitmap = gbitmap_create_with_resource(RESOURCE_ID_action1);
     } 
    else 
     {
       action_bitmap = gbitmap_create_with_resource(RESOURCE_ID_action2);
     }
  } 
  else 
  { // RYU attack
  if (min % 2 == 0){
    if (sec == 51 ) {
         action_bitmap = gbitmap_create_with_resource(RESOURCE_ID_action3a);    
     }
     if (sec == 52 ) {
          action_bitmap = gbitmap_create_with_resource(RESOURCE_ID_action4a);    
     }
     if (sec == 53 ) {
          action_bitmap = gbitmap_create_with_resource(RESOURCE_ID_action5a);    
     }
     if (sec == 54 ) {
          action_bitmap = gbitmap_create_with_resource(RESOURCE_ID_action6a);    
     }
     if (sec == 55 ) {
          action_bitmap = gbitmap_create_with_resource(RESOURCE_ID_action7a);    
     }
     if (sec == 56 ) {
          action_bitmap = gbitmap_create_with_resource(RESOURCE_ID_action8a);    
     }
     if (sec == 57 ) {
          action_bitmap = gbitmap_create_with_resource(RESOURCE_ID_action9a);    
     }
     if (sec == 58 ) {
          action_bitmap = gbitmap_create_with_resource(RESOURCE_ID_action10a);    
     }
     if (sec == 59 ) {
          action_bitmap = gbitmap_create_with_resource(RESOURCE_ID_action11a);    
     }
    
  } else {
    if (sec == 51 ) {
         action_bitmap = gbitmap_create_with_resource(RESOURCE_ID_action3);    
     }
     if (sec == 52 ) {
          action_bitmap = gbitmap_create_with_resource(RESOURCE_ID_action4);    
     }
     if (sec == 53 ) {
          action_bitmap = gbitmap_create_with_resource(RESOURCE_ID_action5);    
     }
     if (sec == 54 ) {
          action_bitmap = gbitmap_create_with_resource(RESOURCE_ID_action6);    
     }
     if (sec == 55 ) {
          action_bitmap = gbitmap_create_with_resource(RESOURCE_ID_action7);    
     }
     if (sec == 56 ) {
          action_bitmap = gbitmap_create_with_resource(RESOURCE_ID_action8);    
     }
     if (sec == 57 ) {
          action_bitmap = gbitmap_create_with_resource(RESOURCE_ID_action9);    
     }
     if (sec == 58 ) {
          action_bitmap = gbitmap_create_with_resource(RESOURCE_ID_action10);    
     }
     if (sec == 59 ) {
          action_bitmap = gbitmap_create_with_resource(RESOURCE_ID_action11);    
     }
  }
  }
  bitmap_layer_set_bitmap(action_layer, action_bitmap);
  
  set_battery();
  
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
  text_layer_set_text(s_second_layer, buffer2);
  text_layer_set_text(date_layer, date_value);
}


static void set_graphics_to_window() {
    //HEALTHBAR: Create GBitmap, then set to created BitmapLayer
  healthbar_bitmap = gbitmap_create_with_resource(RESOURCE_ID_health_bar);
  healthbar_layer = bitmap_layer_create(GRect(0, 49, 144, 35));
  bitmap_layer_set_bitmap(healthbar_layer, healthbar_bitmap);

  //ACTION: Create GBitmap, then set to created BitmapLayer
  action_bitmap = gbitmap_create_with_resource(RESOURCE_ID_action1);
  action_layer = bitmap_layer_create(GRect(0, 81, 144, 87));
  bitmap_layer_set_bitmap(action_layer, action_bitmap);

  //ACTION: Create GBitmap, then set to created BitmapLayer
  clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_clock_back);
  clock_layer = bitmap_layer_create(GRect(0, 0, 144, 49));
  bitmap_layer_set_bitmap(clock_layer, clock_bitmap);

  //BATTERY: Create GBitmap, then set to created BitmapLayer
  batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt90);
  batt_layer = bitmap_layer_create(GRect(79 ,50, 63, 8));
  bitmap_layer_set_bitmap(batt_layer, batt_bitmap);
}

static void set_text_to_window() {
    // Create time TextLayer
  //s_time_layer = text_layer_create(GRect(5, -7, 139, 50));
  s_time_layer = text_layer_create(GRect(0, -6, 139, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  #ifdef PBL_COLOR
    text_layer_set_text_color(s_time_layer, GColorDarkCandyAppleRed);
  #else
    text_layer_set_text_color(s_time_layer, GColorBlack);
  #endif 
  text_layer_set_text(s_time_layer, "00:00");
  
  // Create Seconds TextLayer
  s_second_layer = text_layer_create(GRect(3, 54, 139, 50));
  text_layer_set_background_color(s_second_layer, GColorClear);
  #ifdef PBL_COLOR
    text_layer_set_text_color(s_second_layer, GColorDarkCandyAppleRed);
  #else
    text_layer_set_text_color(s_second_layer, GColorBlack);
  #endif 
  text_layer_set_text(s_second_layer, "00");

  // Create date TextLayer
  date_layer = text_layer_create(GRect(41, 34, 60, 14));
  text_layer_set_background_color(date_layer, GColorClear);
  #ifdef PBL_COLOR
    text_layer_set_text_color(date_layer, GColorDarkCandyAppleRed);
  #else
    text_layer_set_text_color(date_layer, GColorBlack);
  #endif 
  text_layer_set_text(date_layer, "- 00.00.00 -");

  
  //Create GFont
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_48));
  
  //Apply to TextLayer
  //text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  text_layer_set_font(s_second_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_alignment(s_second_layer, GTextAlignmentCenter);

  text_layer_set_font(date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
}

static void main_window_load(Window *window) {
  
  set_graphics_to_window();
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(healthbar_layer));
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(action_layer));
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(clock_layer));
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(batt_layer));  

  set_text_to_window();
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_second_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));
  
  // Make sure the time is displayed from the start
  update_time();
}

static void main_window_unload(Window *window) {
  //Unload GFont
  fonts_unload_custom_font(s_time_font);
  
  //Destroy GBitmap
  gbitmap_destroy(healthbar_bitmap);
  gbitmap_destroy(batt_bitmap);
  gbitmap_destroy(action_bitmap);
  gbitmap_destroy(clock_bitmap);

  //Destroy BitmapLayer
  bitmap_layer_destroy(healthbar_layer);
  bitmap_layer_destroy(batt_layer);
  bitmap_layer_destroy(action_layer);
  bitmap_layer_destroy(clock_layer);
  
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_second_layer);
  text_layer_destroy(date_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
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
