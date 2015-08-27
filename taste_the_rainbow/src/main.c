#include <pebble.h>
#include <stdio.h>
#include <ctype.h>

static char dots10[] = ". . . . . . . . . .";
static char dots9[] = ". . . . . . . . .";
static char dots8[] = ". . . . . . . .";
static char dots7[] = ". . . . . . .";
static char dots6[] = ". . . . . .";
static char dots5[] = ". . . . .";
static char dots4[] = ". . . .";
static char dots3[] = ". . .";
static char dots2[] = ". .";
static char dots1[] = ".";
static char dots0[] = " ";
static Window *s_main_window;

static TextLayer *s_bg_layer;
static TextLayer *s_time_layer;
static TextLayer *dots_layer;
static TextLayer *date_layer;

// Create a long-lived buffer
static char dummy[] = " ";
static char buffer[] = "00 : 00 : 00";
static char buffer2[] = "00";
static char buffer3[] = "00";
static char buffer4[] = "00";
static char hexColor[] = "000000";
static char dots[] = "..........................";
static char date[] = "SEPTEMBER 00";

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);


  if(clock_is_24h_style() == true) {
    //Use 24h hour format
  strftime(buffer, sizeof("00 : 00 : 00"), "%H : %M : %S", tick_time);
 } else {
    //Use 12 hour format
  strftime(buffer, sizeof("00 : 00 : 00"), "%I : %M : %S", tick_time);
  }      
  
  strftime(hexColor, sizeof("000000"), "%H%M%S", tick_time);
  strftime(buffer2, sizeof("00"), "%H", tick_time);
  strftime(buffer3, sizeof("00"), "%M", tick_time);
  strftime(buffer4, sizeof("00"), "%S", tick_time);
  strftime(date, sizeof(date), "%A %d", tick_time);
  
  int hour = atoi(buffer2);
  int min = atoi(buffer3);
  int sec = atoi(buffer4);
    
  if ((min < 5) && (sec < 20)) {
      text_layer_set_text_color(s_time_layer, GColorWhite);
      text_layer_set_text_color(dots_layer, GColorWhite);
      text_layer_set_text_color(date_layer, GColorWhite);
  } else {
      text_layer_set_text_color(s_time_layer, GColorBlack);
      text_layer_set_text_color(dots_layer, GColorBlack);
      text_layer_set_text_color(date_layer, GColorBlack);
  }
  
  //Randomize! 
  hour*=10;
  min*=4;
  sec*=4;

  BatteryChargeState state = battery_state_service_peek();
  switch (state.charge_percent) {
    case 100: 
       text_layer_set_text(dots_layer, dots10);
       break;
    case 90: 
       text_layer_set_text(dots_layer, dots9);
       break;
    case 80: 
       text_layer_set_text(dots_layer, dots8);
    break;
    case 70: 
       text_layer_set_text(dots_layer, dots7);
       break;
    case 60: 
       text_layer_set_text(dots_layer, dots6);
       break;
    case 50: 
       text_layer_set_text(dots_layer, dots5);
       break;
    case 40: 
       text_layer_set_text(dots_layer, dots4);
       break;
    case 30: 
       text_layer_set_text(dots_layer, dots3);
       break;
    case 20: 
       text_layer_set_text(dots_layer, dots2);
       break;
    case 10: 
       text_layer_set_text(dots_layer, dots1);
       break;
    case 0: 
       text_layer_set_text(dots_layer, dots0);
       break;
    default: 
       text_layer_set_text(dots_layer, dots10);
    break;
  }
 

    
  
  
//  text_layer_set_background_color(s_bg_layer, GColorFromHEX(result));
  text_layer_set_background_color(s_bg_layer, GColorFromRGB(hour, min, sec));
  text_layer_set_text(s_time_layer, buffer);
  text_layer_set_text(date_layer, date);
}

static void set_text_to_window() {

    //Time TextLayer 
  s_bg_layer = text_layer_create(GRect(0, 0, 144, 168));
  text_layer_set_background_color(s_bg_layer, GColorFromHEX(0xAA0055));
  text_layer_set_text_color(s_bg_layer, GColorClear);
  text_layer_set_text(s_bg_layer, " ");
  text_layer_set_font(s_bg_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_bg_layer, GTextAlignmentCenter);

  //Time TextLayer 
  s_time_layer = text_layer_create(GRect(0, 45, 144, 40));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00 : 00 : 00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  //Time TextLayer 
  dots_layer = text_layer_create(GRect(0, 60, 144, 40));
  text_layer_set_background_color(dots_layer, GColorClear);
  text_layer_set_text_color(dots_layer,  GColorBlack);
  text_layer_set_text(dots_layer,  "........................");
  text_layer_set_font(dots_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(dots_layer, GTextAlignmentCenter);

  //Time TextLayer 
  date_layer = text_layer_create(GRect(0, 88, 144, 40));
  text_layer_set_background_color(date_layer, GColorClear);
  text_layer_set_text_color(date_layer, GColorBlack);
  text_layer_set_text(date_layer,  "SEPTEMBER 00");
  text_layer_set_font(date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);


}

static void main_window_load(Window *window) {
  set_text_to_window();
    
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_bg_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(dots_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));
  
  // Make sure the time is displayed from the start
  update_time();
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_bg_layer);
  text_layer_destroy(s_time_layer);
  text_layer_destroy(dots_layer);
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

