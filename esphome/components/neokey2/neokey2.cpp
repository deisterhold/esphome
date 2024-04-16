#include "neokey2.h"
#include "esphome/core/log.h"

namespace esphome {
namespace neokey2 {

static const char *const TAG = "neokey2";

void NeoKey2Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up NeoKey...");

  this->effect_data_ = new uint8_t[this->size()];

  if (!this->neokey_.begin(this->address_)) {
    this->mark_failed();
    return;
  }

  // Pulse all the LEDs on to show we're working
  for (uint16_t i = 0; i < this->neokey_.pixels.numPixels(); i++) {
    this->neokey_.pixels.setPixelColor(i, 0x808080);  // make each LED white
    this->neokey_.pixels.show();
    delay(50);
  }

  for (uint16_t i = 0; i < this->neokey_.pixels.numPixels(); i++) {
    this->neokey_.pixels.setPixelColor(i, 0x000000);
    this->neokey_.pixels.show();
    delay(50);
  }
}

void NeoKey2Component::loop() {
  uint8_t buttons = this->neokey_.read();
  ESP_LOGD(TAG, "Buttons: 0b" BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(buttons));
  
  // Check each button
  this->key_1_sensor_->publish_state(buttons & (1 << 0));
  this->key_2_sensor_->publish_state(buttons & (1 << 1));
  this->key_3_sensor_->publish_state(buttons & (1 << 2));
  this->key_4_sensor_->publish_state(buttons & (1 << 3));
}

void NeoKey2Component::dump_config() {
  ESP_LOGCONFIG(TAG, "Config for NeoKey:");
  LOG_I2C_DEVICE(this);

  LOG_BINARY_SENSOR("  ", "Key 1", this->key_1_sensor_);
  LOG_BINARY_SENSOR("  ", "Key 2", this->key_2_sensor_);
  LOG_BINARY_SENSOR("  ", "Key 3", this->key_3_sensor_);
  LOG_BINARY_SENSOR("  ", "Key 4", this->key_4_sensor_);
}

void NeoKey2Component::write_state(light::LightState *state) {
  ESP_LOGVV(TAG, "Writing state...");
}

void NeoKey2Component::log_sensor(binary_sensor::BinarySensor *sensor) {
  ESP_LOGD(TAG, "%s %s", sensor->get_name().c_str(), sensor->state ? "pressed" : "released");
}

}  // namespace neokey2
}  // namespace esphome