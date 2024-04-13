#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

#include "Adafruit_NeoKey_1x4.h"
#include "seesaw_neopixel.h"

namespace esphome {
namespace neokey {

class NeoKeyBinarySensor : public binary_sensor::BinarySensor, public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;
//   float get_setup_priority() const override { return setup_priority::DATA; }
  void set_address(uint8_t address) { address_ =  address; }
 protected:
  uint8_t address_;
  Adafruit_NeoKey_1x4 neokey_;
};

}  // namespace neokey
}  // namespace esphome