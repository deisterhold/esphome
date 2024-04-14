#include "neokey.h"
#include "esphome/core/log.h"

namespace esphome {
namespace neokey {

static const char *const TAG = "neokey";

void NeoKeyComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up NeoKey...");

  if (!neokey_.begin(address_)) {
    this->mark_failed();
    return;
  }
}

void NeoKeyComponent::loop() {
  uint8_t buttons = neokey.read();

  // Check each button
  for (auto *child : this->children_) {
    uint8_t key = child->key_;
    if (buttons & (1 << key)) {
      ESP_LOGD(TAG, "Key %d press" , key);
      child->publish_state(true);
    } else {
      ESP_LOGD(TAG, "Key %d release" , key);
      child->publish_state(false);
    }
  }
}

void NeoKeyComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Config for NeoKey:");
  ESP_LOGCONFIG(TAG, "  Address: %s", address_);
}

NeoKeyBinarySensor::NeoKeyBinarySensor(uint8_t key) : key_(key) {}

}  // namespace neokey
}  // namespace esphome