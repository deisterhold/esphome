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

  // set callbacks
  for(int i=0; i<NEOKEY_1X4_KEYS; i++){
    neokey->registerCallback(i, key_callback);
  }

  // for (auto *child : this->children_) {
  //   // Disable interrupt threshold
  //   touch_pad_config(child->get_touch_pad(), 0);
  // }
}

void NeoKeyComponent::loop() {
  // for (auto *child : this->children_) {
  //   child->value_ = this->component_touch_pad_read(child->get_touch_pad());
  //   child->publish_state(child->value_ < child->get_threshold());

  //   ESP_LOGD(TAG, "NeoKey %s: %" child->get_name().c_str(), child->value_);
  // }
}

void NeoKeyComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Config for NeoKey:");
  ESP_LOGCONFIG(TAG, "  Address: %s", address_);
}

NeoKey1x4Callback NeoKeyComponent::key_callback(keyEvent evt) {
  // this->children_->at()
  // for (auto *child : this->children_) {
  //   child->value_ = this->component_touch_pad_read(child->get_touch_pad());
  //   child->publish_state(child->value_ < child->get_threshold());

  //   ESP_LOGD(TAG, "NeoKey %s: %" child->get_name().c_str(), child->value_);
  // }

  uint8_t key = evt.bit.NUM;
  
  if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
    ESP_LOGD(TAG, "Key %d press" , key);
    // neokey.pixels.setPixelColor(key, Wheel(map(key, 0, neokey.pixels.numPixels(), 0, 255)));
  } else if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING) {
    ESP_LOGD(TAG, "Key %d release" , key);
    // neokey.pixels.setPixelColor(key, 0);
  }

  // Turn on/off the neopixels!
  // neokey.pixels.show();
  return 0;
}

NeoKeyBinarySensor::NeoKeyBinarySensor(uint8_t key) : key_(key) {}

}  // namespace neokey
}  // namespace esphome