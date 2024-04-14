#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

#include "Adafruit_NeoKey_1x4.h"
#include "seesaw_neopixel.h"

namespace esphome {
namespace neokey {

class NeoKeyBinarySensor;

class NeoKeyComponent : public Component {
 public:
  void register_key(NeoKeyBinarySensor *key) { this->children_.push_back(key); }
  void set_address(uint8_t address) { this->address_ = address; }

  void setup() override;
  void loop() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

 protected:
  std::vector<NeoKeyBinarySensor *> children_;
  uint8_t address_;
  Adafruit_NeoKey_1x4 neokey_;
};

/// Simple helper class to expose a touch pad value as a binary sensor.
class NeoKeyBinarySensor : public binary_sensor::BinarySensor {
 public:
  NeoKeyBinarySensor(uint8_t key);
 protected:
  friend NeoKeyComponent;
  uint8_t key_;
};

}  // namespace neokey
}  // namespace esphome