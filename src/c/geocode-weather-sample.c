#include <pebble.h>
#include <pebble-events/pebble-events.h>
#include "logging.h"
#include "enamel.h"
#include "weather.h"

static Window *s_window;
static TextLayer *s_temperature_layer;
static TextLayer *s_conditions_layer;
static TextLayer *s_location_layer;

static EventHandle s_weather_event_handle;

static void weather_handler(GenericWeatherInfo *info, GenericWeatherStatus status, void *context) {
    log_func();
    static char temperature[8];
    if (status == GenericWeatherStatusAvailable) {
        int unit = atoi(enamel_get_WEATHER_UNIT());
        snprintf(temperature, sizeof(temperature), "%d°", unit == 1 ? info->temp_f : info->temp_c);
        text_layer_set_text(s_temperature_layer, temperature);
        text_layer_set_text(s_conditions_layer, info->description);
        text_layer_set_text(s_location_layer, info->name);
    }
}

static void window_load(Window *window) {
    log_func();
    Layer *root_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(root_layer);
#ifdef PBL_ROUND
    bounds.origin.y += 50;
#endif

    s_temperature_layer = text_layer_create(bounds);
    layer_add_child(root_layer, text_layer_get_layer(s_temperature_layer));
    bounds.origin.y += 20;

    s_conditions_layer = text_layer_create(bounds);
    layer_add_child(root_layer, text_layer_get_layer(s_conditions_layer));
    bounds.origin.y += 20;

    s_location_layer = text_layer_create(bounds);
    layer_add_child(root_layer, text_layer_get_layer(s_location_layer));

#ifdef PBL_ROUND
    text_layer_set_text_alignment(s_temperature_layer, GTextAlignmentCenter);
    text_layer_set_text_alignment(s_conditions_layer, GTextAlignmentCenter);
    text_layer_set_text_alignment(s_location_layer, GTextAlignmentCenter);
#endif

    weather_handler(weather_peek(), GenericWeatherStatusAvailable, NULL);
    s_weather_event_handle = events_weather_subscribe(weather_handler, NULL);
}

static void window_unload(Window *window) {
    log_func();
    events_weather_unsubscribe(s_weather_event_handle);

    text_layer_destroy(s_location_layer);
    text_layer_destroy(s_conditions_layer);
    text_layer_destroy(s_temperature_layer);
}

static void init(void) {
    log_func();
    enamel_init();
    weather_init();

    events_app_message_open();

    s_window = window_create();
    window_set_window_handlers(s_window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload
    });
    window_stack_push(s_window, true);
}

static void deinit(void) {
    log_func();
    window_destroy(s_window);

    weather_deinit();
    enamel_deinit();
}

int main(void) {
    log_func();
    init();
    app_event_loop();
    deinit();
}
