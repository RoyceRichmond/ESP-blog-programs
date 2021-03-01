/* RMT example -- RGB LED Strip

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/rmt.h"
#include "led_strip.h"

static const char *TAG = "showdown";

#define RMT_TX_CHANNEL RMT_CHANNEL_0

#define EXAMPLE_CHASE_SPEED_MS (10)

void app_main(void){

    rmt_config_t config = RMT_DEFAULT_CONFIG_TX(8, RMT_TX_CHANNEL);
    // set counter clock to 40MHz
    config.clk_div = 2;

    ESP_ERROR_CHECK(rmt_config(&config));
    ESP_ERROR_CHECK(rmt_driver_install(config.channel, 0, 0));

    // install ws2812 driver
    led_strip_config_t strip_config = LED_STRIP_DEFAULT_CONFIG(1, (led_strip_dev_t)config.channel);
    led_strip_t *strip = led_strip_new_rmt_ws2812(&strip_config);
    if (!strip) {
        ESP_LOGE(TAG, "install WS2812 driver failed");
    }
    // Clear LED strip (turn off all LEDs)
    ESP_ERROR_CHECK(strip->clear(strip, 100));
    // Show simple rainbow chasing pattern
    ESP_LOGI(TAG, "Modified program start");
    while (true) {
        for (int a=0; a<=2;a++){
            for (int i = 0; i <= 255; i+=3) {
                // Write RGB values to strip driver
                ESP_ERROR_CHECK(strip->set_pixel(strip, 0, 0, 0, i));
                // Flush RGB values to LEDs
                ESP_ERROR_CHECK(strip->refresh(strip, 100));
                vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
            }
            for (int i = 0; i <= 255; i+=3) {
                ESP_ERROR_CHECK(strip->set_pixel(strip, 0, 0, i, 255));
                ESP_ERROR_CHECK(strip->refresh(strip, 100));
                vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
            }
            for (int i = 255; i != 0; i-=3) {
                ESP_ERROR_CHECK(strip->set_pixel(strip, 0, 0, 255, i));
                ESP_ERROR_CHECK(strip->refresh(strip, 100));
                vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
            }
            for (int i = 0; i <= 255; i+=3) {
                ESP_ERROR_CHECK(strip->set_pixel(strip, 0, i, 255, 0));
                ESP_ERROR_CHECK(strip->refresh(strip, 100));
                vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
            }
            for (int i = 255; i != 0; i-=3) {
                ESP_ERROR_CHECK(strip->set_pixel(strip, 0, 255, i, 0));
                ESP_ERROR_CHECK(strip->refresh(strip, 100));
                vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
            }
            for (int i = 0; i <= 255; i+=3) {
                ESP_ERROR_CHECK(strip->set_pixel(strip, 0, 255, 0, i));
                ESP_ERROR_CHECK(strip->refresh(strip, 100));
                vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
            }
            for (int i = 255; i != 0; i-=3) {
                ESP_ERROR_CHECK(strip->set_pixel(strip, 0, i, 0, 255));
                ESP_ERROR_CHECK(strip->refresh(strip, 100));
                vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
            }
            for (int i = 255; i != 0; i-=3) {
                ESP_ERROR_CHECK(strip->set_pixel(strip, 0, 0, 0, i));
                ESP_ERROR_CHECK(strip->refresh(strip, 100));
                vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
            }

            strip->clear(strip, 50);
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }
}
