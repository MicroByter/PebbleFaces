#include <pebble.h>
#include <stdio.h>
#include <ctype.h>
  
static Window *s_main_window;

static TextLayer *time_layer, *seconds_layer, *month_layer, *year_layer, *day_layer, *temperature_layer, *desc_layer, *batt_txt_layer;

static GFont time_font, seconds_font, month_font, year_font, day_font, temperature_font, desc_font, batt_txt_font;

static BitmapLayer *clock_layer, *batt_layer;

static GBitmap *clock_bitmap, *batt_bitmap;
 
// Create a long-lived buffer
static char buffer[] = "00:00";
static char sec_buffer[] = "00";
static char month_buffer[] = "00";
static char year_buffer[] = "00";
static char day_buffer[] = "XXX";
static char batt_txt_buffer[] = "100%";
static bool initial_load = true;
static AppSync s_sync;
static uint8_t s_sync_buffer[64];

enum WeatherKey {
  WEATHER_TEMPERATURE_KEY = 0x0,
  WEATHER_DESCRIPTION = 0x1             // TUPLE_CSTRING
};  

#define CONFIG_KEY_BACKGROUND   (3)

void update_configuration(void)
{
//    bool inv = 0;    /* default to not inverted */

//    if (persist_exists(CONFIG_KEY_BACKGROUND))
//    {
//        inv = persist_read_bool(CONFIG_KEY_BACKGROUND);
//    }

    //layer_set_hidden(inverter_layer_get_layer(inverter_layer), !inv);
}


void in_received_handler(DictionaryIterator *received, void *context)
{
   APP_LOG(APP_LOG_LEVEL_DEBUG, "recieved config?");
  Tuple *background_tuple = dict_find(received, CONFIG_KEY_BACKGROUND);

    if (background_tuple)
    {
        app_log(APP_LOG_LEVEL_DEBUG,
                __FILE__,
                __LINE__,
                "bg=%s",
                background_tuple->value->cstring);

       APP_LOG(APP_LOG_LEVEL_DEBUG, "background_tuple->value->cstring");  
       if (strcmp(background_tuple->value->cstring, "F") == 0)
        {
            persist_write_bool(CONFIG_KEY_BACKGROUND, false);
        }
        else
        {
            persist_write_bool(CONFIG_KEY_BACKGROUND, true);
        }
    }

    update_configuration();
}

void in_dropped_handler(AppMessageResult reason, void *ctx)
{
    app_log(APP_LOG_LEVEL_WARNING,
            __FILE__,
            __LINE__,
            "Message dropped, reason code %d",
            reason);
}


static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", app_message_error);
}

static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
 
  APP_LOG(APP_LOG_LEVEL_DEBUG, "sync_tuple_changed_callback");
  switch (key) {
    case WEATHER_TEMPERATURE_KEY:       
      text_layer_set_text(temperature_layer, new_tuple->value->cstring);
      break;
    case WEATHER_DESCRIPTION:       
      text_layer_set_text(desc_layer, new_tuple->value->cstring);
      break;
    
      default: break;
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
  request_weather(); 
}

static void set_battery() {
  
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  BatteryChargeState state = battery_state_service_peek();
  
  gbitmap_destroy(batt_bitmap);
  
  switch (state.charge_percent) {
    case 90: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt90); 
       text_layer_set_text(batt_txt_layer, "90%"); break;
    case 80: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt80); 
       text_layer_set_text(batt_txt_layer, "80%"); break;
    case 70: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt70); 
       text_layer_set_text(batt_txt_layer, "70%"); break;
    case 60: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt60); 
       text_layer_set_text(batt_txt_layer, "60%"); break;
    case 50: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt50); 
       text_layer_set_text(batt_txt_layer, "50%"); break;
    case 40: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt40); 
       text_layer_set_text(batt_txt_layer, "40%"); break;
    case 30: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt30); 
       text_layer_set_text(batt_txt_layer, "30%"); break;
    case 20: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt20); 
       text_layer_set_text(batt_txt_layer, "20%"); break;
    case 10: 
      if ( tick_time->tm_sec % 2 == 0)
         batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt00); 
      else
         batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt10); 
       text_layer_set_text(batt_txt_layer, "10%"); break;
    case 00: 
      if ( tick_time->tm_sec % 2 == 0)
         batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt00); 
      else
         batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt10); 
       text_layer_set_text(batt_txt_layer, "0%"); break;
    
    default: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt100); 
       text_layer_set_text(batt_txt_layer, "100%"); break;
  }
  bitmap_layer_set_bitmap(batt_layer, batt_bitmap); 
}


static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

 if (initial_load) {
    update_temp();
    initial_load = false;
  }
  
  if ((tick_time->tm_min == 0 || tick_time->tm_min == 30) && tick_time->tm_sec == 0)
      update_temp();

  if(clock_is_24h_style() == true) {
    //Use 2h hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    //Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  } 
  strftime(sec_buffer, sizeof("00"), "%S", tick_time);
  strftime(month_buffer, sizeof("00"), "%m", tick_time);
  strftime(year_buffer, sizeof("00"), "%d", tick_time);
  strftime(day_buffer, sizeof("000"), "%a", tick_time);  
  set_battery();
  text_layer_set_text(time_layer, buffer);
  text_layer_set_text(seconds_layer, sec_buffer);
  text_layer_set_text(month_layer, month_buffer);
  text_layer_set_text(year_layer, year_buffer);
  text_layer_set_text(day_layer, day_buffer);
}

static void set_text_to_window() {

  //Time TextLayer 
  time_font =  fonts_load_custom_font(resource_get_handle(RESOURCE_ID_jarvis_24));
  time_layer = text_layer_create(GRect(4, 105, 120, 24));
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_text(time_layer, "00:00");
  text_layer_set_font(time_layer, time_font);
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);

  seconds_font =  fonts_load_custom_font(resource_get_handle(RESOURCE_ID_jarvis_10));
  seconds_layer = text_layer_create(GRect(113, 109, 25, 12));
  text_layer_set_background_color(seconds_layer, GColorClear);
  text_layer_set_text(seconds_layer, "00");
  text_layer_set_font(seconds_layer, seconds_font);
  text_layer_set_text_alignment(seconds_layer, GTextAlignmentRight);

  month_font =  fonts_load_custom_font(resource_get_handle(RESOURCE_ID_jarvis_12));
  month_layer = text_layer_create(GRect(100, 38, 25, 12));
  text_layer_set_background_color(month_layer, GColorClear);
  text_layer_set_text(month_layer, "00");
  text_layer_set_font(month_layer, month_font);
  text_layer_set_text_alignment(month_layer, GTextAlignmentCenter);

  year_font =  fonts_load_custom_font(resource_get_handle(RESOURCE_ID_jarvis_12));
  year_layer = text_layer_create(GRect(100, 54, 25, 12));
  text_layer_set_background_color(year_layer, GColorClear);
  text_layer_set_text(year_layer, "00");
  text_layer_set_font(year_layer, year_font);
  text_layer_set_text_alignment(year_layer, GTextAlignmentCenter);

  day_font =  fonts_load_custom_font(resource_get_handle(RESOURCE_ID_jarvis_08));
  day_layer = text_layer_create(GRect(5, 90, 50, 12));
  text_layer_set_background_color(day_layer, GColorClear);
  text_layer_set_text(day_layer, "000");
  text_layer_set_font(day_layer, day_font);
  text_layer_set_text_alignment(day_layer, GTextAlignmentLeft);
  
  temperature_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_jarvis_09));
  temperature_layer = text_layer_create(GRect(38, 52, 40, 10));
  text_layer_set_background_color(temperature_layer, GColorClear);
  text_layer_set_text(temperature_layer, "100");
  text_layer_set_font(temperature_layer, temperature_font);
  text_layer_set_text_alignment(temperature_layer, GTextAlignmentCenter);
  
  desc_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_jarvis_08));
  desc_layer = text_layer_create(GRect(35, 90, 103, 10));
  text_layer_set_background_color(desc_layer, GColorClear);
  text_layer_set_text(desc_layer, "100");
  text_layer_set_font(desc_layer, desc_font);
  text_layer_set_text_alignment(desc_layer, GTextAlignmentRight);

  batt_txt_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_jarvis_06));
  batt_txt_layer = text_layer_create(GRect(27, 77, 30, 10));
  text_layer_set_background_color(batt_txt_layer, GColorClear);
  text_layer_set_text(batt_txt_layer, "100%");
  text_layer_set_font(batt_txt_layer, batt_txt_font);
  text_layer_set_text_alignment(batt_txt_layer, GTextAlignmentLeft);
  
  
#ifdef PBL_COLOR
  text_layer_set_text_color(year_layer, GColorCyan);
  text_layer_set_text_color(seconds_layer, GColorCyan);
  text_layer_set_text_color(month_layer, GColorCyan);
  text_layer_set_text_color(day_layer, GColorCyan);
  text_layer_set_text_color(time_layer, GColorCyan);
  text_layer_set_text_color(temperature_layer, GColorCyan);
  text_layer_set_text_color(desc_layer, GColorCyan);
  text_layer_set_text_color(batt_txt_layer, GColorWhite);
#else
  text_layer_set_text_color(year_layer, GColorWhite);
  text_layer_set_text_color(seconds_layer, GColorWhite);
  text_layer_set_text_color(month_layer, GColorWhite);
  text_layer_set_text_color(day_layer, GColorWhite);
  text_layer_set_text_color(time_layer, GColorWhite);
  text_layer_set_text_color(temperature_layer, GColorWhite);
  text_layer_set_text_color(desc_layer, GColorWhite);
  text_layer_set_text_color(batt_txt_layer, GColorWhite);
#endif   
}

static void main_window_load(Window *window) {
  //ACTION: Create GBitmap, then set to created BitmapLayer
  clock_bitmap = gbitmap_create_with_resource(RESOURCE_ID_bg_image);
  clock_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(clock_layer, clock_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(clock_layer));

  batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt100);
  batt_layer = bitmap_layer_create(GRect(6 ,25, 25, 58));
  bitmap_layer_set_bitmap(batt_layer, batt_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(batt_layer));
  
  set_text_to_window();
    
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(temperature_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(desc_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(seconds_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(month_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(year_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(day_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(batt_txt_layer));
  
  // Make sure the time is displayed from the start
  update_time();
}

static void main_window_unload(Window *window) {
    

  fonts_unload_custom_font(time_font);
  fonts_unload_custom_font(seconds_font);
  fonts_unload_custom_font(month_font);
  fonts_unload_custom_font(year_font);
  fonts_unload_custom_font(day_font);
  fonts_unload_custom_font(temperature_font);
  fonts_unload_custom_font(desc_font);
  fonts_unload_custom_font(batt_txt_font);

  //Destroy GBitmap
  gbitmap_destroy(clock_bitmap);
  gbitmap_destroy(batt_bitmap);

  //Destroy BitmapLayer
  bitmap_layer_destroy(clock_layer);
  bitmap_layer_destroy(batt_layer);
  
  // Destroy TextLayer
  text_layer_destroy(time_layer);
  text_layer_destroy(seconds_layer);
  text_layer_destroy(month_layer);
  text_layer_destroy(year_layer);
  text_layer_destroy(day_layer);
  text_layer_destroy(temperature_layer);
  text_layer_destroy(desc_layer);
  text_layer_destroy(batt_txt_layer);
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
  
  Tuplet initial_values[] = {
    TupletCString(WEATHER_TEMPERATURE_KEY, "123 F"),
    TupletCString(WEATHER_DESCRIPTION, "XXX"),
  };

  app_sync_init(&s_sync, s_sync_buffer, sizeof(s_sync_buffer), 
      initial_values, ARRAY_LENGTH(initial_values),
      sync_tuple_changed_callback, sync_error_callback, NULL
  );

  request_weather();
  
  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  bluetooth_connection_service_subscribe(bt_handler);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  app_message_register_inbox_received(in_received_handler);
  app_message_register_inbox_dropped(in_dropped_handler);
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
