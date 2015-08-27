#include <pebble.h>
#include <stdio.h>
#include <ctype.h>
  
static Window *s_main_window;
static Layer *s_canvas_layer;
static Layer *car_layer;
static Layer *right_layer;

static GFont clock_font, batt_txt_font, pct_font, date_font;

static TextLayer *s_time_layer;
static TextLayer *month_layer;
static TextLayer *current_date_layer;
static TextLayer *batt_txt_layer;
static TextLayer *pct_layer;

static BitmapLayer *scene_layer;
static BitmapLayer *road_layer;
static BitmapLayer *batt_layer;

static GBitmap *scene_bitmap;
static GBitmap *road_bitmap;
static GBitmap *car_bitmap;
static GBitmap *batt_bitmap;

static int scene_seed, road_seed;

static int max_scenes = 21;
static const uint32_t scenery[] = {
  RESOURCE_ID_beach,    //0
  RESOURCE_ID_day,      //1
  RESOURCE_ID_dusk,     //2
  RESOURCE_ID_night,    //3
  RESOURCE_ID_noon,     //4
  RESOURCE_ID_snow,     //5
  RESOURCE_ID_mountains,//6
  RESOURCE_ID_hills,    //7
  RESOURCE_ID_desert,   //8
  RESOURCE_ID_city_dawn,//9
  RESOURCE_ID_breezy,    //10
  RESOURCE_ID_city_dawn2,//11  
  RESOURCE_ID_city_day,  //12
  RESOURCE_ID_city_dusk, //13
  RESOURCE_ID_city_night,//14
  RESOURCE_ID_city_night2,//15
  RESOURCE_ID_harrier,    //16
  RESOURCE_ID_harrier2,   //17
  RESOURCE_ID_ice,        //18
  RESOURCE_ID_plains,     //19
  RESOURCE_ID_sunset      //20
};

static int max_roads = 8;
static const uint32_t road1[] = {
  RESOURCE_ID_blue_road1,   //0
  RESOURCE_ID_dirt_road1,   //1
  RESOURCE_ID_grass_road1,  //2
  RESOURCE_ID_urban_road1,  //3
  RESOURCE_ID_water_road1,  //4
  RESOURCE_ID_grassy_road1, //5
  RESOURCE_ID_desert_road1,  //6
  RESOURCE_ID_clown_road1  //7
};

static const uint32_t road2[] = {
  RESOURCE_ID_blue_road2,   //0
  RESOURCE_ID_dirt_road2,   //1
  RESOURCE_ID_grass_road2,  //2
  RESOURCE_ID_urban_road2,  //3
  RESOURCE_ID_water_road2,  //4
  RESOURCE_ID_grassy_road2, //5
  RESOURCE_ID_desert_road2,  //6
  RESOURCE_ID_clown_road2  //7
};

static int max_right = 2;
static const uint32_t right1[] = {
  RESOURCE_ID_sign1,   //0
  RESOURCE_ID_bill1   //1
};

static const uint32_t right2[] = {
  RESOURCE_ID_sign2,   //0
  RESOURCE_ID_bill2   //1
};


// Create a long-lived buffer
static char buffer[] = "00:00";
static char month_buffer[] = "SEPTEMBER 00";

static int choose_scene(int rand_value) {
  int seed = rand() % rand_value;
  return seed;
}

static void set_display() {
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
 
  if ( tick_time->tm_min % 10 == 0 && tick_time->tm_sec == 0) {
    scene_seed = choose_scene(max_scenes);
    road_seed = choose_scene(max_roads);
  }

  gbitmap_destroy(scene_bitmap);
  gbitmap_destroy(road_bitmap);
  scene_bitmap = gbitmap_create_with_resource(scenery[scene_seed]);
  //gbitmap_destroy(car_bitmap);

  if ( tick_time->tm_sec % 2 == 0) {
      road_bitmap = gbitmap_create_with_resource(road1[road_seed]);
  //    car_bitmap = gbitmap_create_with_resource(RESOURCE_ID_car1);   
  } else {
      road_bitmap = gbitmap_create_with_resource(road2[road_seed]);
  //    car_bitmap = gbitmap_create_with_resource(RESOURCE_ID_car2);  
  }
  bitmap_layer_set_bitmap(scene_layer, scene_bitmap);          
  bitmap_layer_set_bitmap(road_layer, road_bitmap);   
}

static void set_battery() {
  BatteryChargeState state = battery_state_service_peek();
  
  gbitmap_destroy(batt_bitmap);
  
  switch (state.charge_percent) {
    case 90: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt90); 
       text_layer_set_text(batt_txt_layer, "90"); break;
    case 80: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt80); 
       text_layer_set_text(batt_txt_layer, "80"); break;
    case 70: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt70); 
       text_layer_set_text(batt_txt_layer, "70"); break;
    case 60: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt60); 
       text_layer_set_text(batt_txt_layer, "60"); break;
    case 50: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt50); 
       text_layer_set_text(batt_txt_layer, "50"); break;
    case 40: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt40); 
       text_layer_set_text(batt_txt_layer, "40"); break;
    case 30: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt30); 
       text_layer_set_text(batt_txt_layer, "30"); break;
    case 20: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt20); 
       text_layer_set_text(batt_txt_layer, "20"); break;
    case 10: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt10); 
        text_layer_set_text(batt_txt_layer, "10"); break;
    case 00: 
       batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt00); 
       text_layer_set_text(batt_txt_layer, "0"); break;
    default: 
       text_layer_set_text(batt_txt_layer, "100"); 
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

  strftime(month_buffer, sizeof("SEPTEMBER"), "%B %d", tick_time);
      
  set_display();
  set_battery();
  //set_display_right();
 
  text_layer_set_text(s_time_layer, buffer);
  text_layer_set_text(month_layer, month_buffer);
}

static void set_text_to_window() {

  //Time TextLayer 
  clock_font =  fonts_load_custom_font(resource_get_handle(RESOURCE_ID_arcade_24));
  s_time_layer = text_layer_create(GRect(0, 30, 144, 44));
  text_layer_set_background_color(s_time_layer, GColorClear);
    text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, clock_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Create current date TextLayer 
  date_font =  fonts_load_custom_font(resource_get_handle(RESOURCE_ID_dos_18));
  // Create current date TextLayer 
  month_layer = text_layer_create(GRect(0, 55, 144, 18));
  text_layer_set_background_color(month_layer, GColorClear);
  text_layer_set_text_color(month_layer, GColorWhite);
  text_layer_set_text(month_layer, "SEPTEMBER 00");
  text_layer_set_font(month_layer, date_font);
  text_layer_set_text_alignment(month_layer, GTextAlignmentCenter);

  // Create current date TextLayer 
  batt_txt_font =  fonts_load_custom_font(resource_get_handle(RESOURCE_ID_lcd_10));
  batt_txt_layer = text_layer_create(GRect(10, 152, 20, 14));
  text_layer_set_background_color(batt_txt_layer, GColorClear);
  text_layer_set_text_color(batt_txt_layer, GColorRed);
  text_layer_set_text(batt_txt_layer, "100");
  text_layer_set_font(batt_txt_layer, batt_txt_font);
  text_layer_set_text_alignment(batt_txt_layer, GTextAlignmentRight);

  // Create current date TextLayer 
  pct_font =  fonts_load_custom_font(resource_get_handle(RESOURCE_ID_lcd_12));
  pct_layer = text_layer_create(GRect(19, 150, 20, 14));
  text_layer_set_background_color(pct_layer, GColorClear);
  text_layer_set_text_color(pct_layer, GColorYellow);
  text_layer_set_text(pct_layer, "%");
  text_layer_set_font(pct_layer, pct_font);
  text_layer_set_text_alignment(pct_layer, GTextAlignmentRight);
}

static void car_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_compositing_mode(ctx, GCompOpSet);
  graphics_draw_bitmap_in_rect(ctx, car_bitmap, gbitmap_get_bounds(car_bitmap));
}

static void main_window_load(Window *window) {
  
   Layer *window_layer = window_get_root_layer(window);
  
  //ACTION: Create GBitmap, then set to created BitmapLayer
  scene_bitmap = gbitmap_create_with_resource(RESOURCE_ID_beach);
  scene_layer = bitmap_layer_create(GRect(0, 0, 144, 136));
  bitmap_layer_set_bitmap(scene_layer, scene_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(scene_layer));

  road_bitmap = gbitmap_create_with_resource(RESOURCE_ID_urban_road1);
  road_layer = bitmap_layer_create(GRect(0, 136, 144, 32));
  bitmap_layer_set_bitmap(road_layer, road_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(road_layer));
 
  batt_bitmap = gbitmap_create_with_resource(RESOURCE_ID_batt100);
  batt_layer = bitmap_layer_create(GRect(5, 163, 32, 4));
  bitmap_layer_set_bitmap(batt_layer, batt_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(batt_layer));

  car_bitmap = gbitmap_create_with_resource(RESOURCE_ID_car1);
  car_layer = layer_create(GRect(51, 146, 41, 21));
  layer_set_update_proc(car_layer, car_update_proc);
  layer_add_child(window_layer, car_layer);

  /*
  right_bitmap = gbitmap_create_with_resource(RESOURCE_ID_empty_right);
  right_layer = layer_create(GRect(79, 110, 65, 58));
  layer_set_update_proc(right_layer, right_update_proc);
  layer_add_child(window_layer, right_layer);
 */
  set_text_to_window();
    
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(month_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(batt_txt_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(pct_layer));
  
  // Make sure the time is displayed from the start
  update_time();
}

static void main_window_unload(Window *window) {
  
  fonts_unload_custom_font(batt_txt_font);
  fonts_unload_custom_font(pct_font);
  fonts_unload_custom_font(clock_font);

  //Destroy GBitmap
  gbitmap_destroy(scene_bitmap);
  gbitmap_destroy(road_bitmap);
  gbitmap_destroy(car_bitmap);
  gbitmap_destroy(batt_bitmap);
//  gbitmap_destroy(right_bitmap);

  //Destroy BitmapLayer
  bitmap_layer_destroy(scene_layer);
  bitmap_layer_destroy(road_layer);
  bitmap_layer_destroy(batt_layer);

  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(month_layer);
  text_layer_destroy(batt_txt_layer);
  text_layer_destroy(pct_layer);
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


