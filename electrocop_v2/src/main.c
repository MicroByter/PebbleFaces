#include <pebble.h>
#include <stdio.h>
#include <ctype.h>
  
static Window *s_main_window;

static TextLayer *s_time_layer, *month_layer, *year_layer, *date_layer, *batt_layer;
static TextLayer *s_temperature_layer, *temp_min_layer, *temp_max_layer;

static GFont s_time_font, month_font, year_font, date_font, batt_font, temperature_font, temp_min_font, temp_max_font;

static BitmapLayer *clock_layer;
static BitmapLayer *weather_layer;

static GBitmap *clock_bitmap;
static GBitmap *weather_bitmap;
static bool initial_load = true;
static AppSync s_sync;
static uint8_t s_sync_buffer[64];

static const uint32_t WEATHER_ICONS[] = {
  RESOURCE_ID_IMAGE_SUN, //0
  RESOURCE_ID_IMAGE_CLOUD, //1
  RESOURCE_ID_IMAGE_RAIN, //2
  RESOURCE_ID_IMAGE_SNOW //3
};

enum WeatherKey {
  WEATHER_ICON_KEY = 0x0,         // TUPLE_INT
  WEATHER_TEMPERATURE_KEY = 0x1,  // TUPLE_CSTRING
  WEATHER_CITY_KEY = 0x2,         // TUPLE_CSTRING
  WEATHER_TEMP_MIN = 0x3,
  WEATHER_TEMP_MAX = 0x4
};

// Create a long-lived buffer
static char buffer[] = "00:00";
static char month_buffer[] = "XXX";
static char year_buffer[] = "0000";
static char date_buffer[] = "00";

static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", app_message_error);
}

static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
 
  APP_LOG(APP_LOG_LEVEL_DEBUG, "sync_tuple_changed_callback");
  switch (key) {
    case WEATHER_ICON_KEY: 
      if (weather_bitmap) {
        gbitmap_destroy(weather_bitmap);
      }
      weather_bitmap = gbitmap_create_with_resource(WEATHER_ICONS[new_tuple->value->uint8]);

      #ifdef PBL_SDK_3
         bitmap_layer_set_compositing_mode(weather_layer, GCompOpSet);
      #endif
      
      bitmap_layer_set_bitmap(weather_layer, weather_bitmap);
    break;
    
    case WEATHER_TEMPERATURE_KEY:
//      temp[] =  new_tuple->value->cstring"
//      if (new_tuple->value->cstring == " ") {
//      } else {      
        text_layer_set_text(s_temperature_layer, new_tuple->value->cstring);
//      }  
      break;
    
    case WEATHER_TEMP_MIN: 
//      if (new_tuple->value->cstring == " ") {
//      } else {        
        text_layer_set_text(temp_min_layer, new_tuple->value->cstring); 
//      }
      break;
    
    case WEATHER_TEMP_MAX:
//      if (new_tuple->value->cstring == " ") {
//      } else {        
        text_layer_set_text(temp_max_layer, new_tuple->value->cstring); 
//        text_layer_set_text(temp_max_layer, "100"); 
//      }
      break;

    case WEATHER_CITY_KEY:
      break;
  }
}

static void request_weather(void) {
APP_LOG(APP_LOG_LEVEL_DEBUG, "request_weather");
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  if (!iter) {
    // Error creating outbound message
    return;
  }

  int value = 1;
  dict_write_int(iter, 1, &value, sizeof(int), true);
  dict_write_end(iter);

  app_message_outbox_send();
}

static void update_temp() {
 
  APP_LOG(APP_LOG_LEVEL_DEBUG, "update_temp");
/* Tuplet initial_values[] = {
    TupletInteger(WEATHER_ICON_KEY, (uint8_t) 1),
    TupletCString(WEATHER_TEMPERATURE_KEY, "123"),
    TupletCString(WEATHER_CITY_KEY, "St Pebblesburg"),
    TupletCString(WEATHER_TEMP_MIN, "00"),
    TupletCString(WEATHER_TEMP_MAX, "99"),
  };

  app_sync_init(&s_sync, s_sync_buffer, sizeof(s_sync_buffer), 
      initial_values, ARRAY_LENGTH(initial_values),
      sync_tuple_changed_callback, sync_error_callback, NULL
  ); */
  request_weather(); 
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
  strftime(month_buffer, sizeof("XXX"), "%b", tick_time);
  strftime(year_buffer, sizeof("0000"), "%G", tick_time);
  strftime(date_buffer, sizeof("00"), "%d", tick_time);

  if (initial_load) {
    update_temp();
    initial_load = false;
  }
  
  if (tick_time->tm_min == 0 || tick_time->tm_min == 30)
      update_temp();

  BatteryChargeState state = battery_state_service_peek();
  switch (state.charge_percent) {
    case 90: text_layer_set_text(batt_layer, "90"); break;
    case 80: text_layer_set_text(batt_layer, "80"); break;
    case 70: text_layer_set_text(batt_layer, "70"); break;
    case 60: text_layer_set_text(batt_layer, "60"); break;
    case 50: text_layer_set_text(batt_layer, "50"); break;
    case 40: text_layer_set_text(batt_layer, "40"); break;
    case 30: text_layer_set_text(batt_layer, "30"); break;
    case 20: text_layer_set_text(batt_layer, "20"); break;
    case 10: text_layer_set_text(batt_layer, "10"); break;
    default: text_layer_set_text(batt_layer,"100"); break;
  }
    
  text_layer_set_text(s_time_layer, buffer);
  text_layer_set_text(month_layer, month_buffer);
  text_layer_set_text(year_layer, year_buffer);
  text_layer_set_text(date_layer, date_buffer);
}

static void set_text_to_window() {

  //Time TextLayer 
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_lcd_48));
  s_time_layer = text_layer_create(GRect(10, 15, 130, 60));
  text_layer_set_background_color(s_time_layer, GColorClear);
  #ifdef PBL_COLOR
    text_layer_set_text_color(s_time_layer, GColorArmyGreen);
  #else
    text_layer_set_text_color(s_time_layer, GColorBlack);
  #endif 
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  //Month TextLayer 
  month_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_lcd_18));
  month_layer = text_layer_create(GRect(97, 69, 40, 18));
  text_layer_set_background_color(month_layer, GColorClear);
  #ifdef PBL_COLOR
    text_layer_set_text_color(month_layer, GColorArmyGreen);
  #else
    text_layer_set_text_color(month_layer, GColorBlack);
  #endif 
  text_layer_set_text(month_layer, "000");
  text_layer_set_font(month_layer, month_font);
  text_layer_set_text_alignment(month_layer, GTextAlignmentCenter);

  //Year TextLayer 
  year_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_lcd_12));
  year_layer = text_layer_create(GRect(97, 89, 40, 12));
  text_layer_set_background_color(year_layer, GColorClear);
  #ifdef PBL_COLOR
    text_layer_set_text_color(year_layer, GColorArmyGreen);
  #else
    text_layer_set_text_color(year_layer, GColorBlack);
  #endif 
  text_layer_set_text(year_layer, "00:00");
  text_layer_set_font(year_layer, year_font);
  text_layer_set_text_alignment(year_layer, GTextAlignmentCenter);

  //Date TextLayer 
  date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_lcd_36));
  date_layer = text_layer_create(GRect(63, 65, 40, 36));
  text_layer_set_background_color(date_layer, GColorClear);
  #ifdef PBL_COLOR
    text_layer_set_text_color(date_layer, GColorArmyGreen);
  #else
    text_layer_set_text_color(date_layer, GColorBlack);
  #endif 
  text_layer_set_text(date_layer, "00:00");
  text_layer_set_font(date_layer, date_font);
  text_layer_set_text_alignment(date_layer, GTextAlignmentLeft);
  
  //batt TextLayer 
  batt_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_lcd_24));
  batt_layer = text_layer_create(GRect(56, 110, 50, 24));
  text_layer_set_background_color(batt_layer, GColorClear);
  #ifdef PBL_COLOR
    text_layer_set_text_color(batt_layer, GColorArmyGreen);
  #else
    text_layer_set_text_color(batt_layer, GColorBlack);
  #endif 
  text_layer_set_text(batt_layer, "100");
  text_layer_set_font(batt_layer, batt_font);
  text_layer_set_text_alignment(batt_layer, GTextAlignmentRight);

  temp_min_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_lcd_12));
  temp_min_layer = text_layer_create(GRect(13, 128, 18, 12));
  text_layer_set_background_color(temp_min_layer, GColorClear);
  #ifdef PBL_COLOR
    text_layer_set_text_color(temp_min_layer, GColorArmyGreen);
  #else
    text_layer_set_text_color(temp_min_layer, GColorBlack);
  #endif 
  text_layer_set_text(temp_min_layer, "00");
  text_layer_set_font(temp_min_layer, temp_min_font);
  text_layer_set_text_alignment(temp_min_layer, GTextAlignmentRight);

  temp_max_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_lcd_12));
  temp_max_layer = text_layer_create(GRect(41, 128, 25, 12));
  text_layer_set_background_color(temp_max_layer, GColorClear);
  #ifdef PBL_COLOR
    text_layer_set_text_color(temp_max_layer, GColorArmyGreen);
  #else
    text_layer_set_text_color(temp_max_layer, GColorBlack);
  #endif 
  text_layer_set_text(temp_max_layer, "99");
  text_layer_set_font(temp_max_layer, temp_max_font);
  text_layer_set_text_alignment(temp_max_layer, GTextAlignmentLeft);
  
  temperature_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_lcd_24));
  s_temperature_layer = text_layer_create(GRect(0, 104, 50, 24));
  text_layer_set_background_color(s_temperature_layer, GColorClear);
  #ifdef PBL_COLOR
    text_layer_set_text_color(s_temperature_layer, GColorArmyGreen);
  #else
    text_layer_set_text_color(s_temperature_layer, GColorBlack);
  #endif 
  text_layer_set_text(s_temperature_layer, "108");
  text_layer_set_font(s_temperature_layer, temperature_font);
  text_layer_set_text_alignment(s_temperature_layer, GTextAlignmentRight);
}


static void main_window_load(Window *window) {
  //ACTION: Create GBitmap, then set to created BitmapLayer
  clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_bg_image);
  clock_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(clock_layer, clock_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(clock_layer));

  //ACTION: Create GBitmap, then set to created BitmapLayer
  weather_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_RAIN);
  weather_layer = bitmap_layer_create(GRect(13, 69, 48, 35));
  bitmap_layer_set_bitmap(weather_layer, weather_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(weather_layer));
  

  
  set_text_to_window();
    
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(month_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(year_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(batt_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_temperature_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(temp_min_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(temp_max_layer));
  
  // Make sure the time is displayed from the start
  update_time();
}

static void main_window_unload(Window *window) {
    
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(month_font);
  fonts_unload_custom_font(year_font);
  fonts_unload_custom_font(date_font);
  fonts_unload_custom_font(temperature_font);
  fonts_unload_custom_font(temp_min_font);
  fonts_unload_custom_font(temp_max_font);

  //Destroy GBitmap
  gbitmap_destroy(clock_bitmap);
  gbitmap_destroy(weather_bitmap);

  //Destroy BitmapLayer
  bitmap_layer_destroy(clock_layer);
  bitmap_layer_destroy(weather_layer);
  
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(month_layer);
  text_layer_destroy(year_layer);
  text_layer_destroy(date_layer);
  text_layer_destroy(batt_layer);
  text_layer_destroy(temp_min_layer);
  text_layer_destroy(temp_max_layer);
  text_layer_destroy(s_temperature_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void min_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_temp();
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

  Tuplet initial_values[] = {
    TupletInteger(WEATHER_ICON_KEY, (uint8_t) 1),
    TupletCString(WEATHER_TEMPERATURE_KEY, "123"),
    TupletCString(WEATHER_CITY_KEY, "St Pebblesburg"),
    TupletCString(WEATHER_TEMP_MIN, "00"),
    TupletCString(WEATHER_TEMP_MAX, "99"),
  };

  app_sync_init(&s_sync, s_sync_buffer, sizeof(s_sync_buffer), 
      initial_values, ARRAY_LENGTH(initial_values),
      sync_tuple_changed_callback, sync_error_callback, NULL
  );

  request_weather();
  
  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  //bluetooth_connection_service_subscribe(bt_handler);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  //tick_timer_service_subscribe(MINUTE_UNIT, min_handler);
  app_message_open(64, 64);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
  tick_timer_service_unsubscribe();
  app_sync_deinit(&s_sync);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}



