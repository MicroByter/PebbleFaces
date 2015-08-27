#include <pebble.h>
  
static Window *s_main_window;

static TextLayer *time_layer;
static TextLayer *timephase_layer;

static BitmapLayer *bg_layer;
static BitmapLayer *dogaction_layer;
static BitmapLayer *duck_layer;

static GBitmap *bg_bitmap;
static GBitmap *dogaction_bitmap;
static GBitmap *duck_bitmap;

// Create a long-lived buffer
static char buffer[] = "00:00";
static char current_date_buffer[] = "00";
static char timephase_buffer[] = "00";


static void duck_action() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  int sec = tick_time->tm_sec;
  
  gbitmap_destroy(duck_bitmap);
  //bitmap_layer_destroy(duck_layer);
  
   switch (sec) {
    case 1: 
       duck_layer = bitmap_layer_create(GRect(80, 94, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_du1); break;
    case 2: 
       duck_layer = bitmap_layer_create(GRect(80, 92, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_du2); break;
    case 3: 
       duck_layer = bitmap_layer_create(GRect(80, 90, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_du3); break;
    case 4: 
       duck_layer = bitmap_layer_create(GRect(80, 88, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_du2); break;
    case 5: 
       duck_layer = bitmap_layer_create(GRect(80, 86, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_du1); break;
    case 6: 
       duck_layer = bitmap_layer_create(GRect(78, 86, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_dl1); break;
    case 7: 
       duck_layer = bitmap_layer_create(GRect(76, 86, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_dl2); break;
    case 8: 
       duck_layer = bitmap_layer_create(GRect(74, 86, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_dl3); break;
    case 9: 
       duck_layer = bitmap_layer_create(GRect(72, 86, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_dl2); break;
    case 10: 
       duck_layer = bitmap_layer_create(GRect(72, 84, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_dl1); break;
    case 11: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_du1); break;
    case 12: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_du2); break;
    case 13: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_du3); break;
    case 14: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_du2); break;
    case 15: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_du1); break;
    case 16: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_dr1); break;
    case 17: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_dr2); break;
    case 18: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_dr3); break;
    case 19: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_dr2); break;
    case 20: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_dr1); break;
    case 21: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_du1); break;
    case 22: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_du2); break;
    case 23: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_du3); break;
    case 24: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_du2); break;
    case 25: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_du1); break;
    case 26: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_dl1); break;
    case 27: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_dl2); break;
    case 28: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
      duck_bitmap =  gbitmap_create_with_resource(RESOURCE_ID_dl3); break;
    case 29: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
      duck_bitmap =  gbitmap_create_with_resource(RESOURCE_ID_dl2); break;
    case 30: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_dl1); break;
    case 31: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_dl2); break;
    case 32: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_du1); break;
    case 33: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_du2); break;
    case 34: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_du3); break;
    case 35: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_du2); break;
    case 36: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_du1); break;
    case 37: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_dr1); break;
    case 38: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_dr2); break;
    case 39: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_dr3); break;
    case 40: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_dr2); break;
    case 41: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_dr1); break;
    case 42: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_du1); break;
    case 43: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_du2); break;
    case 44: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_du3); break;
    case 45: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_du2); break;
    case 46: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_du1); break;
    case 47: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_du2); break;
    case 48: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_du3); break;
    case 49: 
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_du2); break;
    default:
       duck_layer = bitmap_layer_create(GRect(80, 95, 40, 40));
       duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_dblank); break;
   }
  bitmap_layer_set_bitmap(duck_layer, duck_bitmap);       
}

static void dog_action(){
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  gbitmap_destroy(dogaction_bitmap);

  if (tick_time->tm_sec >= 50 && tick_time->tm_sec <= 60) 
  {
       if (tick_time->tm_min % 2 == 0) 
       {
           dogaction_bitmap = gbitmap_create_with_resource(RESOURCE_ID_duck1);          
       } 
       else 
       {
           if (tick_time->tm_sec % 2 == 0) 
           {
              dogaction_bitmap = gbitmap_create_with_resource(RESOURCE_ID_doglaugh1);
           } 
           else 
           {
              dogaction_bitmap = gbitmap_create_with_resource(RESOURCE_ID_doglaugh2);      
           }
       }
  }
  else 
  {
      dogaction_bitmap = gbitmap_create_with_resource(RESOURCE_ID_dog_blank);               
  }
    bitmap_layer_set_bitmap(dogaction_layer, dogaction_bitmap);       
}
    
static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  
  strftime(current_date_buffer, sizeof("00"), "%d", tick_time);
  strftime(timephase_buffer, sizeof("00"), "%p", tick_time);

  duck_action();
  dog_action();  
  
  text_layer_set_text(time_layer, buffer);
  text_layer_set_text(timephase_layer, timephase_buffer);
}

static void set_text_to_window() {
   //Time TextLayer 
  time_layer = text_layer_create(GRect(0, 5, 139, 50));
  text_layer_set_background_color(time_layer, GColorClear);
//  #ifdef PBL_COLOR
//    text_layer_set_text_color(s_time_layer, GColorFromHEX(0x00ffff));
//  #else
    text_layer_set_text_color(time_layer, GColorBlack);
//  #endif 
  text_layer_set_text(time_layer, "00:00");
  text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);

  //Time TextLayer 
  timephase_layer = text_layer_create(GRect(110, 20, 25, 25));
  text_layer_set_background_color(timephase_layer, GColorClear);
  text_layer_set_text_color(timephase_layer, GColorBlack);
  text_layer_set_text(timephase_layer, "00");
  text_layer_set_font(timephase_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(timephase_layer, GTextAlignmentCenter);
}

static void main_window_load(Window *window) {
  //ACTION: Create GBitmap, then set to created BitmapLayer -- BACKGROUND
  bg_bitmap = gbitmap_create_with_resource(RESOURCE_ID_duckbg);
  bg_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(bg_layer, bg_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(bg_layer));

  //ACTION: Create GBitmap, then set to created BitmapLayer -- DOG
  dogaction_bitmap = gbitmap_create_with_resource(RESOURCE_ID_dog_blank);
  dogaction_layer = bitmap_layer_create(GRect(52, 94, 58, 42));
  bitmap_layer_set_bitmap(dogaction_layer, dogaction_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(dogaction_layer));
  
  //ACTION: Create GBitmap, then set to created BitmapLayer -- DUCK
  duck_bitmap = gbitmap_create_with_resource(RESOURCE_ID_dblank);
  duck_layer = bitmap_layer_create(GRect(80, 50, 40, 40));
  bitmap_layer_set_bitmap(duck_layer, duck_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(duck_layer));

  // Create time TextLayer
  time_layer = text_layer_create(GRect(5, -7, 139, 50));
  text_layer_set_background_color(time_layer, GColorClear);
#ifdef PBL_COLOR
  text_layer_set_text_color(time_layer, GColorDarkCandyAppleRed);
#else
  text_layer_set_text_color(time_layer, GColorWhite);
#endif 
  text_layer_set_text(time_layer, "00:00");
  
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  
  set_text_to_window();
    
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(timephase_layer));
  
  // Make sure the time is displayed from the start
  update_time();
}

static void main_window_unload(Window *window) {
  
  //Destroy GBitmap
  gbitmap_destroy(bg_bitmap);
  gbitmap_destroy(dogaction_bitmap);
  gbitmap_destroy(duck_bitmap);

  //Destroy BitmapLayer
  bitmap_layer_destroy(bg_layer);
  bitmap_layer_destroy(dogaction_layer);
  bitmap_layer_destroy(duck_layer);
  
  // Destroy TextLayer
  text_layer_destroy(time_layer);
  text_layer_destroy(timephase_layer);
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


