#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cassert>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <driver/gpio.h>
#include <esp_err.h>

#include "Application.hpp"
#include "Output.hpp"
#include "Input.hpp"
#include "Sensor.hpp"
#include "Server.h"
#include "ultrasonic.h"
#include "debug_def.h"
#include "common_data.h"

extern "C" {
  void app_main (void);
}

#define MAX_DISTANCE 500

static ultrasonic_sensor_t ultrasonic_1 = {
.trigger_pin = GPIO_NUM_5,
.echo_pin = GPIO_NUM_17
};

esp_err_t cb_config_sensor_1 ()
{
  esp_err_t error = ultrasonic_init (&ultrasonic_1);
  return error;
}

float get_data_sensor_1 ()
{
  float distance;
  esp_err_t resp = ultrasonic_measure (&ultrasonic_1, MAX_DISTANCE, &distance);

  if ( ESP_OK != resp )
  {
    switch ( resp )
    {
      case ESP_ERR_ULTRASONIC_PING:
        debug_error ("ESP_ERR_ULTRASONIC_PING");
        break;

      case ESP_ERR_ULTRASONIC_ECHO_TIMEOUT:
        debug_error ("ESP_ERR_ULTRASONIC_ECHO_TIMEOUT");
        break;

      case ESP_ERR_ULTRASONIC_PING_TIMEOUT:
        debug_error ("ESP_ERR_ULTRASONIC_PING_TIMEOUT");
        break;

      default:
        break;
    }
  }
  return distance * 100;
}

void app_main (void)
{
  // Create application
  Application* app = Application::get_instance ("New_device");

  // Sensors
  Sensor* sensor_1 = SensorFactory::create_sensor ("Ultrasonic_1", cb_config_sensor_1, get_data_sensor_1, 50);
  app->add_sensor (sensor_1);

  // Network
  app->add_network (Network::get_instance ("Danilo_tech", "danilo_tech", "ssid_name", "password_1234"));
  app->start_network (NetworkType::AP);

  if ( ESP_OK == app->network_exist () )
  {
    init_end_points (); // API server
  }

  app->print_device_details ();

  //Add a minimum delay of 100 ms to prevent the watchdog......
  while ( 1 )
  {
    server_data.distance = app->get_sensor_data ("Ultrasonic_1", SensorFilterType::KALMAN);
    debug_normal ("Clients connected to Access Point %d \t distance = %0.2f cm", app->get_clients_connected (), server_data.distance);
    DELAY (ONE_SECOND);
  }
}