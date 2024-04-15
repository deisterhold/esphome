#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/light/addressable_light.h"
#include "esphome/components/light/light_output.h"

#include "Adafruit_NeoKey_1x4.h"
#include "seesaw_neopixel.h"

namespace esphome {
namespace neokey2 {

class NeoKey2Component : public PollingComponent, public i2c::I2CDevice, public light::AddressableLight {
 public:
  void setup() override;
  void update() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void set_key_1_sensor(binary_sensor::BinarySensor *sensor) { key_1_sensor_ = sensor; }
  void set_key_2_sensor(binary_sensor::BinarySensor *sensor) { key_2_sensor_ = sensor; }
  void set_key_3_sensor(binary_sensor::BinarySensor *sensor) { key_3_sensor_ = sensor; }
  void set_key_4_sensor(binary_sensor::BinarySensor *sensor) { key_4_sensor_ = sensor; }

  int32_t size() const override { return (int_32t)this->neokey_.pixels.numPixels(); }
  void write_state(light::LightState *state) override;
  void clear_effect_data() override {
    for (int i = 0; i < this->size(); i++)
      this->effect_data_[i] = 0;
  }
  light::LightTraits get_traits() override {
    auto traits = light::LightTraits();
    traits.set_supported_color_modes({light::ColorMode::RGB});
    return traits;
  }
 protected:
  Adafruit_NeoKey_1x4 neokey_;
  binary_sensor::BinarySensor *key_1_sensor_{nullptr};
  binary_sensor::BinarySensor *key_2_sensor_{nullptr};
  binary_sensor::BinarySensor *key_3_sensor_{nullptr};
  binary_sensor::BinarySensor *key_4_sensor_{nullptr};

  uint8_t *effect_data_{nullptr};
  ESPColorView get_view_internal(int32_t index) {
    auto view = light::ESPColorView();

    return view;
  }

  void log_sensor(binary_sensor::BinarySensor *sensor);
};

}  // namespace neokey2
}  // namespace esphome