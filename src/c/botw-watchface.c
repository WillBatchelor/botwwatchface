#include <pebble.h>
#include <stdio.h>
#include <time.h>
//#include "botw-watchface.h"

static Window *s_main_window;

static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static GFont s_time_font;
static GFont s_date_font;

static TextLayer *s_time_hex_layer;
static TextLayer *s_date_hex_layer;
static GFont s_hex_font;

static TextLayer *s_weather_layer;
static TextLayer *s_weather_hex_layer;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);

	unsigned int h;
	
	h=(unsigned int)time(NULL); 
	
  static char s_hext_buffer[20];
  snprintf(s_hext_buffer, 20, "%X", h); 

  static char s_date_buffer[20];
	strftime(s_date_buffer, sizeof(s_date_buffer), "%a %m/%d" , tick_time);
	
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
	text_layer_set_text(s_time_hex_layer, s_hext_buffer);
	text_layer_set_text(s_date_layer, s_date_buffer);
	text_layer_set_text(s_date_hex_layer, s_date_buffer);
}


static void main_window_load(Window *window) {
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);	
	
	s_time_layer = text_layer_create(
	GRect(PBL_IF_ROUND_ELSE(30,0), PBL_IF_ROUND_ELSE(2,2), bounds.size.w, 44));

	s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_INRIA_SANS_REGULAR_38));	
	
	s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_INRIA_SANS_REGULAR_33));
	
	text_layer_set_background_color(s_time_layer, GColorClear);
	text_layer_set_text_color(s_time_layer,GColorPictonBlue);
	text_layer_set_font(s_time_layer, s_time_font);
	text_layer_set_text_alignment(s_time_layer, GTextAlignmentLeft);

	s_hex_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SC_BOLD_12));
	
	s_time_hex_layer = text_layer_create(
	GRect(0, 48, bounds.size.w, 14));
	
	text_layer_set_background_color(s_time_hex_layer, GColorClear);
	text_layer_set_text_color(s_time_hex_layer, GColorCobaltBlue);
	text_layer_set_font(s_time_hex_layer, s_hex_font);
	text_layer_set_text_alignment(s_time_hex_layer, GTextAlignmentLeft);

	s_date_layer = text_layer_create(
		GRect(0, 56, bounds.size.w, 44));
	
	
	text_layer_set_background_color(s_date_layer, GColorClear);
	text_layer_set_text_color(s_date_layer,GColorPictonBlue);
	text_layer_set_font(s_date_layer, s_date_font);
	text_layer_set_text_alignment(s_date_layer, GTextAlignmentLeft);
	
	s_date_hex_layer = text_layer_create(
	GRect(0, 97, bounds.size.w, 14));
	
	text_layer_set_background_color(s_date_hex_layer, GColorClear);
	text_layer_set_text_color(s_date_hex_layer, GColorCobaltBlue);
	text_layer_set_font(s_date_hex_layer, s_hex_font);
	text_layer_set_text_alignment(s_date_hex_layer, GTextAlignmentLeft);
	
	// Create temperature Layer
	s_weather_layer = text_layer_create(
    GRect(0, 105, bounds.size.w, 44));

	// Style the text
	text_layer_set_background_color(s_weather_layer, GColorClear);
	text_layer_set_text_color(s_weather_layer, GColorPictonBlue);
	text_layer_set_text_alignment(s_weather_layer, GTextAlignmentLeft);
	text_layer_set_font(s_weather_layer, s_date_font);
	text_layer_set_text(s_weather_layer, "Loading...");
	
	// Create temperature Sheikah Layer
	s_weather_hex_layer = text_layer_create(
    GRect(0, 146, bounds.size.w, 14));

	// Style the text
	text_layer_set_background_color(s_weather_hex_layer, GColorClear);
	text_layer_set_text_color(s_weather_hex_layer, GColorCobaltBlue);
	text_layer_set_text_alignment(s_weather_hex_layer, GTextAlignmentLeft);
	text_layer_set_font(s_weather_hex_layer, s_hex_font);
	text_layer_set_text(s_weather_hex_layer, "Weather");

	
	layer_add_child(window_layer, text_layer_get_layer(s_time_layer));

	layer_add_child(window_layer, text_layer_get_layer(s_time_hex_layer));

	layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
	
	layer_add_child(window_layer, text_layer_get_layer(s_date_hex_layer));
	
	layer_add_child(window_layer,text_layer_get_layer(s_weather_layer));
	
	layer_add_child(window_layer,text_layer_get_layer(s_weather_hex_layer));

	update_time();
	
}

static void main_window_unload(Window *window) {
	text_layer_destroy(s_time_layer);
	fonts_unload_custom_font(s_hex_font);
	text_layer_destroy(s_time_hex_layer);
	text_layer_destroy(s_date_layer);
	text_layer_destroy(s_date_hex_layer);
	text_layer_destroy(s_weather_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	update_time();	
}


static void init() {
	s_main_window = window_create();
	window_set_background_color(s_main_window, GColorBlack);
	
	window_set_window_handlers(s_main_window, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload 
	});
	
	window_stack_push(s_main_window, false);

	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);	
}

static void deinit() {
	window_destroy(s_main_window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}
