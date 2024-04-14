#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/i2c/i2c.h"

#include "Adafruit_NeoKey_1x4.h"
#include "seesaw_neopixel.h"

namespace esphome {
namespace neokey2 {

class NeoKey2Component : public Component, public i2c::I2CDevice {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void set_key_1_sensor(binary_sensor::BinarySensor *sensor) { key_1_sensor_ = sensor; }
  void set_key_2_sensor(binary_sensor::BinarySensor *sensor) { key_2_sensor_ = sensor; }
  void set_key_3_sensor(binary_sensor::BinarySensor *sensor) { key_3_sensor_ = sensor; }
  void set_key_4_sensor(binary_sensor::BinarySensor *sensor) { key_4_sensor_ = sensor; }

 protected:
  Adafruit_NeoKey_1x4 neokey_;
  binary_sensor::BinarySensor *key_1_sensor_{nullptr};
  binary_sensor::BinarySensor *key_2_sensor_{nullptr};
  binary_sensor::BinarySensor *key_3_sensor_{nullptr};
  binary_sensor::BinarySensor *key_4_sensor_{nullptr};
};

}  // namespace neokey2
}  // namespace esphome