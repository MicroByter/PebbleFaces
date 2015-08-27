#include <pebble.h>
  
static Window *s_main_window;

static TextLayer *time_layer;

static BitmapLayer *bg_layer;
static BitmapLayer *clock1_layer;
static BitmapLayer *clock2_layer;
static BitmapLayer *clock3_layer;
static BitmapLayer *clock4_layer;

static GBitmap *bg_bitmap;
static GBitmap *clock1_bitmap;
static GBitmap *clock2_bitmap;
static GBitmap *clock3_bitmap;
static GBitmap *clock4_bitmap;
  
// Create a long-lived buffer
static char buffer[] = "00:00";

static void set_clock1_image(char time_number) {

  switch (time_number) {
    case '0':   clock1_bitmap = gbitmap_create_with_resource(RESOURCE_ID_0); break;
    case '1':   clock1_bitmap = gbitmap_create_with_resource(RESOURCE_ID_1); break;
    case '2':   clock1_bitmap = gbitmap_create_with_resource(RESOURCE_ID_2); break;
    case '3':   clock1_bitmap = gbitmap_create_with_resource(RESOURCE_ID_3); break;
    case '4':   clock1_bitmap = gbitmap_create_with_resource(RESOURCE_ID_4); break;
    case '5':   clock1_bitmap = gbitmap_create_with_resource(RESOURCE_ID_5); break;
    case '6':   clock1_bitmap = gbitmap_create_with_resource(RESOURCE_ID_6); break;
    case '7':   clock1_bitmap = gbitmap_create_with_resource(RESOURCE_ID_7); break;
    case '8':   clock1_bitmap = gbitmap_create_with_resource(RESOURCE_ID_8); break;
    case '9':   clock1_bitmap = gbitmap_create_with_resource(RESOURCE_ID_9); break;
    default: clock1_bitmap = gbitmap_create_with_resource(RESOURCE_ID_0); break;
  }
  
}

static void set_clock2_image(char time_number) {

  switch (time_number) {
    case '0':   clock2_bitmap = gbitmap_create_with_resource(RESOURCE_ID_0); break;
    case '1':   clock2_bitmap = gbitmap_create_with_resource(RESOURCE_ID_1); break;
    case '2':   clock2_bitmap = gbitmap_create_with_resource(RESOURCE_ID_2); break;
    case '3':   clock2_bitmap = gbitmap_create_with_resource(RESOURCE_ID_3); break;
    case '4':   clock2_bitmap = gbitmap_create_with_resource(RESOURCE_ID_4); break;
    case '5':   clock2_bitmap = gbitmap_create_with_resource(RESOURCE_ID_5); break;
    case '6':   clock2_bitmap = gbitmap_create_with_resource(RESOURCE_ID_6); break;
    case '7':   clock2_bitmap = gbitmap_create_with_resource(RESOURCE_ID_7); break;
    case '8':   clock2_bitmap = gbitmap_create_with_resource(RESOURCE_ID_8); break;
    case '9':   clock2_bitmap = gbitmap_create_with_resource(RESOURCE_ID_9); break;
    default: clock2_bitmap = gbitmap_create_with_resource(RESOURCE_ID_0); break;
  }
  
}

static void set_clock3_image(char time_number) {

  switch (time_number) {
    case '0':   clock3_bitmap = gbitmap_create_with_resource(RESOURCE_ID_0); break;
    case '1':   clock3_bitmap = gbitmap_create_with_resource(RESOURCE_ID_1); break;
    case '2':   clock3_bitmap = gbitmap_create_with_resource(RESOURCE_ID_2); break;
    case '3':   clock3_bitmap = gbitmap_create_with_resource(RESOURCE_ID_3); break;
    case '4':   clock3_bitmap = gbitmap_create_with_resource(RESOURCE_ID_4); break;
    case '5':   clock3_bitmap = gbitmap_create_with_resource(RESOURCE_ID_5); break;
    case '6':   clock3_bitmap = gbitmap_create_with_resource(RESOURCE_ID_6); break;
    case '7':   clock3_bitmap = gbitmap_create_with_resource(RESOURCE_ID_7); break;
    case '8':   clock3_bitmap = gbitmap_create_with_resource(RESOURCE_ID_8); break;
    case '9':   clock3_bitmap = gbitmap_create_with_resource(RESOURCE_ID_9); break;
    default: clock3_bitmap = gbitmap_create_with_resource(RESOURCE_ID_0); break;
  }
  
}

static void set_clock4_image(char time_number) {

  switch (time_number) {
    case '0':   clock4_bitmap = gbitmap_create_with_resource(RESOURCE_ID_0); break;
    case '1':   clock4_bitmap = gbitmap_create_with_resource(RESOURCE_ID_1); break;
    case '2':   clock4_bitmap = gbitmap_create_with_resource(RESOURCE_ID_2); break;
    case '3':   clock4_bitmap = gbitmap_create_with_resource(RESOURCE_ID_3); break;
    case '4':   clock4_bitmap = gbitmap_create_with_resource(RESOURCE_ID_4); break;
    case '5':   clock4_bitmap = gbitmap_create_with_resource(RESOURCE_ID_5); break;
    case '6':   clock4_bitmap = gbitmap_create_with_resource(RESOURCE_ID_6); break;
    case '7':   clock4_bitmap = gbitmap_create_with_resource(RESOURCE_ID_7); break;
    case '8':   clock4_bitmap = gbitmap_create_with_resource(RESOURCE_ID_8); break;
    case '9':   clock4_bitmap = gbitmap_create_with_resource(RESOURCE_ID_9); break;
    default: clock4_bitmap = gbitmap_create_with_resource(RESOURCE_ID_0); break;
  }
  
}

static void set_clock1_image_red(char time_number) {

  switch (time_number) {
    case '0':   clock1_bitmap = gbitmap_create_with_resource(RESOURCE_ID_0red); break;
    case '1':   clock1_bitmap = gbitmap_create_with_resource(RESOURCE_ID_1red); break;
    case '2':   clock1_bitmap = gbitmap_create_with_resource(RESOURCE_ID_2red); break;
    case '3':   clock1_bitmap = gbitmap_create_with_resource(RESOURCE_ID_3red); break;
    case '4':   clock1_bitmap = gbitmap_create_with_resource(RESOURCE_ID_4red); break;
    case '5':   clock1_bitmap = gbitmap_create_with_resource(RESOURCE_ID_5red); break;
    case '6':   clock1_bitmap = gbitmap_create_with_resource(RESOURCE_ID_6red); break;
    case '7':   clock1_bitmap = gbitmap_create_with_resource(RESOURCE_ID_7red); break;
    case '8':   clock1_bitmap = gbitmap_create_with_resource(RESOURCE_ID_8red); break;
    case '9':   clock1_bitmap = gbitmap_create_with_resource(RESOURCE_ID_9red); break;
    default: clock1_bitmap = gbitmap_create_with_resource(RESOURCE_ID_0red); break;
  }
 
}
static void set_clock2_image_red(char time_number) {

  switch (time_number) {
    case '0':   clock2_bitmap = gbitmap_create_with_resource(RESOURCE_ID_0red); break;
    case '1':   clock2_bitmap = gbitmap_create_with_resource(RESOURCE_ID_1red); break;
    case '2':   clock2_bitmap = gbitmap_create_with_resource(RESOURCE_ID_2red); break;
    case '3':   clock2_bitmap = gbitmap_create_with_resource(RESOURCE_ID_3red); break;
    case '4':   clock2_bitmap = gbitmap_create_with_resource(RESOURCE_ID_4red); break;
    case '5':   clock2_bitmap = gbitmap_create_with_resource(RESOURCE_ID_5red); break;
    case '6':   clock2_bitmap = gbitmap_create_with_resource(RESOURCE_ID_6red); break;
    case '7':   clock2_bitmap = gbitmap_create_with_resource(RESOURCE_ID_7red); break;
    case '8':   clock2_bitmap = gbitmap_create_with_resource(RESOURCE_ID_8red); break;
    case '9':   clock2_bitmap = gbitmap_create_with_resource(RESOURCE_ID_9red); break;
    default: clock2_bitmap = gbitmap_create_with_resource(RESOURCE_ID_0red); break;
  }
 
}
static void set_clock3_image_red(char time_number) {

  switch (time_number) {
    case '0':   clock3_bitmap = gbitmap_create_with_resource(RESOURCE_ID_0red); break;
    case '1':   clock3_bitmap = gbitmap_create_with_resource(RESOURCE_ID_1red); break;
    case '2':   clock3_bitmap = gbitmap_create_with_resource(RESOURCE_ID_2red); break;
    case '3':   clock3_bitmap = gbitmap_create_with_resource(RESOURCE_ID_3red); break;
    case '4':   clock3_bitmap = gbitmap_create_with_resource(RESOURCE_ID_4red); break;
    case '5':   clock3_bitmap = gbitmap_create_with_resource(RESOURCE_ID_5red); break;
    case '6':   clock3_bitmap = gbitmap_create_with_resource(RESOURCE_ID_6red); break;
    case '7':   clock3_bitmap = gbitmap_create_with_resource(RESOURCE_ID_7red); break;
    case '8':   clock3_bitmap = gbitmap_create_with_resource(RESOURCE_ID_8red); break;
    case '9':   clock3_bitmap = gbitmap_create_with_resource(RESOURCE_ID_9red); break;
    default: clock3_bitmap = gbitmap_create_with_resource(RESOURCE_ID_0red); break;
  }
 
}
static void set_clock4_image_red(char time_number) {

  switch (time_number) {
    case '0':   clock4_bitmap = gbitmap_create_with_resource(RESOURCE_ID_0red); break;
    case '1':   clock4_bitmap = gbitmap_create_with_resource(RESOURCE_ID_1red); break;
    case '2':   clock4_bitmap = gbitmap_create_with_resource(RESOURCE_ID_2red); break;
    case '3':   clock4_bitmap = gbitmap_create_with_resource(RESOURCE_ID_3red); break;
    case '4':   clock4_bitmap = gbitmap_create_with_resource(RESOURCE_ID_4red); break;
    case '5':   clock4_bitmap = gbitmap_create_with_resource(RESOURCE_ID_5red); break;
    case '6':   clock4_bitmap = gbitmap_create_with_resource(RESOURCE_ID_6red); break;
    case '7':   clock4_bitmap = gbitmap_create_with_resource(RESOURCE_ID_7red); break;
    case '8':   clock4_bitmap = gbitmap_create_with_resource(RESOURCE_ID_8red); break;
    case '9':   clock4_bitmap = gbitmap_create_with_resource(RESOURCE_ID_9red); break;
    default: clock4_bitmap = gbitmap_create_with_resource(RESOURCE_ID_0red); break;
  }
 
}



static void update_time() {
  
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
 
  BatteryChargeState state = battery_state_service_peek();
  
  // Write the current hours and minutes into the buffer
  strftime(buffer, sizeof("00:00"), "%H:%M", tick_time); //24h
  //strftime(buffer, sizeof("00:00"), "%I:%M", tick_time); //12h
  
  gbitmap_destroy(clock1_bitmap);
  if (state.charge_percent <= 20)
    set_clock1_image_red(buffer[0]);
  else
    set_clock1_image(buffer[0]);
  bitmap_layer_set_bitmap(clock1_layer, clock1_bitmap);
  
  gbitmap_destroy(clock2_bitmap);
  if (state.charge_percent <= 20)
    set_clock2_image_red(buffer[1]);
  else
    set_clock2_image(buffer[1]);
  bitmap_layer_set_bitmap(clock2_layer, clock2_bitmap);

  gbitmap_destroy(clock3_bitmap);
  if (state.charge_percent <= 20)
    set_clock3_image_red(buffer[3]);
  else
    set_clock3_image(buffer[3]);
  bitmap_layer_set_bitmap(clock3_layer, clock3_bitmap);
  
  gbitmap_destroy(clock4_bitmap);
  if (state.charge_percent <= 20)
    set_clock4_image_red(buffer[4]);
  else
    set_clock4_image(buffer[4]);
  bitmap_layer_set_bitmap(clock4_layer, clock4_bitmap);
  
  text_layer_set_text(time_layer, buffer);
}

static void main_window_load(Window *window) {

   //Background
  bg_bitmap = gbitmap_create_with_resource(RESOURCE_ID_base_bg);
  bg_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(bg_layer, bg_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(bg_layer));
  
  //clock1 (top left)
  clock1_bitmap = gbitmap_create_with_resource(RESOURCE_ID_0);
  clock1_layer = bitmap_layer_create(GRect(31, 3, 40, 81));
  bitmap_layer_set_bitmap(clock1_layer, clock1_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(clock1_layer));

  //clock2 (top right)
  clock2_bitmap = gbitmap_create_with_resource(RESOURCE_ID_0);
  clock2_layer = bitmap_layer_create(GRect(73, 3, 40, 81));
  bitmap_layer_set_bitmap(clock2_layer, clock2_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(clock2_layer));

  //clock1 (bottom left)
  clock3_bitmap = gbitmap_create_with_resource(RESOURCE_ID_0);
  clock3_layer = bitmap_layer_create(GRect(31, 84, 40, 81));
  bitmap_layer_set_bitmap(clock3_layer, clock3_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(clock3_layer));

  //clock1 (bottome right)
  clock4_bitmap = gbitmap_create_with_resource(RESOURCE_ID_0);
  clock4_layer = bitmap_layer_create(GRect(73, 84, 40, 81));
  bitmap_layer_set_bitmap(clock4_layer, clock4_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(clock4_layer));

  
    // Create time TextLayer
  time_layer = text_layer_create(GRect(0, 0, 0, 0));
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_text_color(time_layer, GColorClear);
  text_layer_set_text(time_layer, "00:00");
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer)); 

  // Make sure the time is displayed from the start
  update_time();
}

static void main_window_unload(Window *window) {

  gbitmap_destroy(clock1_bitmap);
  gbitmap_destroy(clock2_bitmap);
  gbitmap_destroy(clock3_bitmap);
  gbitmap_destroy(clock4_bitmap);

  bitmap_layer_destroy(clock1_layer);
  bitmap_layer_destroy(clock2_layer);
  bitmap_layer_destroy(clock3_layer);
  bitmap_layer_destroy(clock4_layer);

  text_layer_destroy(time_layer);
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


