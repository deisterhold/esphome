#include "neokey_binary_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace neokey {

static const char *const TAG = "neokey.binary_sensor";

void NeoKeyBinarySensor::setup() {
  ESP_LOGCONFIG(TAG, "Setting up NeoKey...");

  if (!neokey.begin(address_)) {
    this->mark_failed();
    return;
  }
}

void NeoKeyBinarySensor::loop() {
}

void NeoKeyBinarySensor::dump_config() {
  ESP_LOGCONFIG(TAG, "NeoKey:");

  LOG_BINARY_SENSOR("  ", "NeoKey Binary Sensor", this);
}

}  // namespace neokey
}  // namespace esphome