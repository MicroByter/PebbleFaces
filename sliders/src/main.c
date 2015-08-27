#include <pebble.h>
  
static Window *s_main_window;

static TextLayer *s_time_layer;
static TextLayer *prev_date_1_layer;
static TextLayer *prev_date_2_layer;
static TextLayer *prev_date_3_layer;
static TextLayer *current_date_layer;
static TextLayer *next_date_1_layer;
static TextLayer *next_date_2_layer;
static TextLayer *next_date_3_layer;
static TextLayer *prev_month_layer;
static TextLayer *current_month_layer;
static TextLayer *next_month_layer;
static TextLayer *timephase_layer;

static BitmapLayer *clock_layer;
static BitmapLayer *batt_layer;
//static BitmapLayer *topbg_layer;

static GBitmap *clock_bitmap;
static GBitmap *batt_bitmap;
//static GBitmap *topbg_bitmap;
  
// Create a long-lived buffer
static char buffer[] = "00:00";
static char prev_date_1_buffer[] = "00";
static char prev_date_2_buffer[] = "00";
static char prev_date_3_buffer[] = "00";
static char next_date_1_buffer[] = "00";
static char next_date_2_buffer[] = "00";
static char next_date_3_buffer[] = "00";
static char current_date_buffer[] = "00";
static char prev_month_buffer[] = "000";
static char current_month_buffer[] = "000";
static char next_month_buffer[] = "000";
static char timephase_buffer[] = "00";

static void set_battery() {
  
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  BatteryChargeState state = battery_state_service_peek();
  
  gbitmap_destroy(batt_bitmap);
  
  switch (state.charge_percent) {
    case 90: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt100); break;
    case 80: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt80); break;
    case 70: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt80); break;
    case 60: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt60); break;
    case 50: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt60); break;
    case 40: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt40); break;
    case 30: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt40); break;
    case 20: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt20); break;
    case 10: 
       if ( tick_time->tm_sec % 2 == 0) {
          batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_battempty); 
       } else {
          batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt00);          
       }
       break;
 
    case 00: 
       if ( tick_time->tm_sec % 2 == 0) {
          batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_battempty); 
       } else {
          batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt00);          
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

  strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  
  strftime(current_date_buffer, sizeof("00"), "%d", tick_time);
  strftime(current_month_buffer, sizeof("000"), "%b", tick_time);
  strftime(timephase_buffer, sizeof("00"), "%p", tick_time);

  set_battery();
  time_t now = time(NULL);
  struct tm* tm = localtime(&now);
  
  tm->tm_mday -= 1;
  strftime(prev_date_1_buffer, sizeof("00"), "%d", tm);
  tm->tm_mday -= 1;
  strftime(prev_date_2_buffer, sizeof("00"), "%d", tm);
  tm->tm_mday -= 1;
  strftime(prev_date_3_buffer, sizeof("00"), "%d", tm);

  now = time(NULL);
  tm = localtime(&now);

  tm->tm_mday += 1;
  strftime(next_date_1_buffer, sizeof("00"), "%d", tm);
  tm->tm_mday += 1;
  strftime(next_date_2_buffer, sizeof("00"), "%d", tm);
  tm->tm_mday += 1;
  strftime(next_date_3_buffer, sizeof("00"), "%d", tm);
  
  now = time(NULL);
  tm = localtime(&now);

  switch (tm->tm_mon) {
    case 1:
        if (tm->tm_mday == 28) {
           strftime(next_date_1_buffer, sizeof("00"), "01", tm);
           strftime(next_date_2_buffer, sizeof("00"), "02", tm);
           strftime(next_date_3_buffer, sizeof("00"), "03", tm);
        }
        break;
    case 3:
    case 5:
    case 8:
    case 10:
        if (tm->tm_mday == 30) {
           strftime(next_date_1_buffer, sizeof("00"), "01", tm);
           strftime(next_date_2_buffer, sizeof("00"), "02", tm);
           strftime(next_date_3_buffer, sizeof("00"), "03", tm);
        }
        break;
     case 00:
     case 02:
     case 04:
     case 06:
     case 7:
     case 9:
     case 11:
        if (tm->tm_mday == 31) {
           strftime(next_date_1_buffer, sizeof("00"), "01", tm);
           strftime(next_date_2_buffer, sizeof("00"), "02", tm);
           strftime(next_date_3_buffer, sizeof("00"), "03", tm);
        }
        break;
  }
  
  now = time(NULL);
  tm = localtime(&now);
  tm->tm_mon -= 1;
  strftime(prev_month_buffer, sizeof("000"), "%b", tm);
 
  now = time(NULL);
  tm = localtime(&now);
  tm->tm_mon += 1;
  strftime(next_month_buffer, sizeof("000"), "%b", tm);

  
  //write to all text layers
  text_layer_set_text(s_time_layer, buffer);
  text_layer_set_text(timephase_layer, timephase_buffer);
  text_layer_set_text(prev_date_1_layer, prev_date_1_buffer);
  text_layer_set_text(prev_date_2_layer, prev_date_2_buffer);
  text_layer_set_text(prev_date_3_layer, prev_date_3_buffer);
  text_layer_set_text(current_date_layer, current_date_buffer);
  text_layer_set_text(next_date_1_layer, next_date_1_buffer);
  text_layer_set_text(next_date_2_layer, next_date_2_buffer);
  text_layer_set_text(next_date_3_layer, next_date_3_buffer);

  text_layer_set_text(current_month_layer, current_month_buffer);
  text_layer_set_text(prev_month_layer, prev_month_buffer);
  text_layer_set_text(next_month_layer, next_month_buffer);
}

static void set_text_to_window() {
  //Time TextLayer 
  s_time_layer = text_layer_create(GRect(-5, 15, 139, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
//  #ifdef PBL_COLOR
//    text_layer_set_text_color(s_time_layer, GColorFromHEX(0x00ffff));
//  #else
    text_layer_set_text_color(s_time_layer, GColorBlack);
//  #endif 
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  //Time TextLayer 
  timephase_layer = text_layer_create(GRect(120, 44, 25, 25));
  text_layer_set_background_color(timephase_layer, GColorClear);
  text_layer_set_text_color(timephase_layer, GColorBlack);
  text_layer_set_text(timephase_layer, "00");
  text_layer_set_font(timephase_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(timephase_layer, GTextAlignmentCenter);
  
  // Create current date TextLayer
  current_date_layer = text_layer_create(GRect(60, 97, 25, 24));
  text_layer_set_background_color(current_date_layer, GColorClear);
  text_layer_set_text_color(current_date_layer, GColorWhite);
  text_layer_set_text(current_date_layer, "00");
  text_layer_set_font(current_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(current_date_layer, GTextAlignmentCenter);

  // Create Prev Date 1 TextLayer
  prev_date_1_layer = text_layer_create(GRect(40, 107, 15, 15));
  text_layer_set_background_color(prev_date_1_layer, GColorClear);
  text_layer_set_text_color(prev_date_1_layer, GColorWhite);
  text_layer_set_text(prev_date_1_layer, "00");
  text_layer_set_font(prev_date_1_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(prev_date_1_layer, GTextAlignmentCenter);

  // Create Prev Date 2 TextLayer
  prev_date_2_layer = text_layer_create(GRect(20, 107, 15, 15));
  text_layer_set_background_color(prev_date_2_layer, GColorClear);
  text_layer_set_text_color(prev_date_2_layer, GColorWhite);
  text_layer_set_text(prev_date_2_layer, "00");
  text_layer_set_font(prev_date_2_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(prev_date_2_layer, GTextAlignmentCenter);

    // Create Prev Date 3 TextLayer
  prev_date_3_layer = text_layer_create(GRect(0, 107, 15, 15));
  text_layer_set_background_color(prev_date_3_layer, GColorClear);
  text_layer_set_text_color(prev_date_3_layer, GColorWhite);
  text_layer_set_text(prev_date_3_layer, "00");
  text_layer_set_font(prev_date_3_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(prev_date_3_layer, GTextAlignmentCenter);

  // Create Next Date 1 TextLayer
  next_date_1_layer = text_layer_create(GRect(90, 107, 15, 15));
  text_layer_set_background_color(next_date_1_layer, GColorClear);
  text_layer_set_text_color(next_date_1_layer, GColorWhite);
  text_layer_set_text(next_date_1_layer, "00");
  text_layer_set_font(next_date_1_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(next_date_1_layer, GTextAlignmentCenter);

    // Create Next Date 3 TextLayer
  next_date_2_layer = text_layer_create(GRect(110, 107, 15, 15));
  text_layer_set_background_color(next_date_2_layer, GColorClear);
  text_layer_set_text_color(next_date_2_layer, GColorWhite);
  text_layer_set_text(next_date_2_layer, "00");
  text_layer_set_font(next_date_2_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(next_date_2_layer, GTextAlignmentCenter);

    // Create Next Date 3 TextLayer
  next_date_3_layer = text_layer_create(GRect(130, 107, 15, 15));
  text_layer_set_background_color(next_date_3_layer, GColorClear);
  text_layer_set_text_color(next_date_3_layer, GColorWhite);
  text_layer_set_text(next_date_3_layer, "00");
  text_layer_set_font(next_date_3_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(next_date_3_layer, GTextAlignmentCenter);

  // Create current month TextLayer
  current_month_layer = text_layer_create(GRect(52, 140, 40, 30));
  text_layer_set_background_color(current_month_layer, GColorClear);
  text_layer_set_text_color(current_month_layer, GColorWhite);
  text_layer_set_text(current_month_layer, "000");
  text_layer_set_font(current_month_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(current_month_layer, GTextAlignmentCenter);

  // Create prev month TextLayer
  prev_month_layer = text_layer_create(GRect(12, 145, 40, 30));
  text_layer_set_background_color(prev_month_layer, GColorClear);
  text_layer_set_text_color(prev_month_layer, GColorWhite);
  text_layer_set_text(prev_month_layer, "000");
  text_layer_set_font(prev_month_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(prev_month_layer, GTextAlignmentCenter);

  // Create prev month TextLayer
  next_month_layer = text_layer_create(GRect(92, 145, 40, 30));
  text_layer_set_background_color(next_month_layer, GColorClear);
  text_layer_set_text_color(next_month_layer, GColorWhite);
  text_layer_set_text(next_month_layer, "000");
  text_layer_set_font(next_month_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(next_month_layer, GTextAlignmentCenter);
  
}

static void main_window_load(Window *window) {
  //ACTION: Create GBitmap, then set to created BitmapLayer
  clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_slidebar_bg);
  clock_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(clock_layer, clock_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(clock_layer));
  
  //ACTION: Create GBitmap, then set to created BitmapLayer
  batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt100);
  batt_layer = bitmap_layer_create(GRect(124, 18, 18, 40));
  bitmap_layer_set_bitmap(batt_layer, batt_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(batt_layer));

  set_text_to_window();
    
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(prev_date_1_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(prev_date_2_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(prev_date_3_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(current_date_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(next_date_1_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(next_date_2_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(next_date_3_layer));

  layer_add_child(window_get_root_layer(window), text_layer_get_layer(current_month_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(prev_month_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(next_month_layer));

  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(timephase_layer));
  
  // Make sure the time is displayed from the start
  update_time();
}

static void main_window_unload(Window *window) {
  
  //Destroy GBitmap
  gbitmap_destroy(clock_bitmap);
  gbitmap_destroy(batt_bitmap);

  //Destroy BitmapLayer
  bitmap_layer_destroy(clock_layer);
  bitmap_layer_destroy(batt_layer);
  
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(timephase_layer);
  text_layer_destroy(current_date_layer);
  text_layer_destroy(prev_date_1_layer);
  text_layer_destroy(prev_date_2_layer);
  text_layer_destroy(prev_date_3_layer);
  text_layer_destroy(next_date_1_layer);
  text_layer_destroy(next_date_2_layer);
  text_layer_destroy(next_date_3_layer);

  text_layer_destroy(current_month_layer);
  text_layer_destroy(prev_month_layer);
  text_layer_destroy(next_month_layer);

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
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
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

