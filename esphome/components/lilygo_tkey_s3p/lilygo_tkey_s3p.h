#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/gpio_expander/cached_gpio.h"

namespace esphome::lilygo_tkey_s3p {

enum LilyGoTKeyboardS3ProGPIORegisters {
  LilyGoTKeyboardS3Pro_LCD_CS = 0x01,
  LilyGoTKeyboardS3Pro_KEY_TRGR = 0x02,
  LilyGoTKeyboardS3Pro_LED_MODE = 0x03,
  LilyGoTKeyboardS3Pro_LED_BRGHT = 0x04,
  LilyGoTKeyboardS3Pro_LED_HUEH = 0x05,
  LilyGoTKeyboardS3Pro_LED_HUEL = 0x06,
  LilyGoTKeyboardS3Pro_LED_SAT = 0x07,
  LilyGoTKeyboardS3Pro_LED_CTRL1 = 0x08,
  LilyGoTKeyboardS3Pro_LED_CTRL2 = 0x09,
  LilyGoTKeyboardS3Pro_FRMWR_VER = 0x0A
};

class LilyGoTKeyboardS3Pro : public Component, public i2c::I2CDevice, public gpio_expander::CachedGpioExpander<uint16_t, 16> {
 public:
  MCP23016() = default;

  void setup() override;
  void loop() override;
  void pin_mode(uint8_t pin, gpio::Flags flags);

  float get_setup_priority() const override;

  void set_interrupt_pin(InternalGPIOPin *pin) { this->interrupt_pin_ = pin; }

 protected:
  static void IRAM_ATTR gpio_intr(MCP23016 *arg);
  // Virtual methods from CachedGpioExpander
  bool digital_read_hw(uint8_t pin) override;
  bool digital_read_cache(uint8_t pin) override;
  void digital_write_hw(uint8_t pin, bool value) override;

  // read a given register
  bool read_reg_(uint8_t reg, uint16_t *value);
  // write a value to a given register
  bool write_reg_(uint8_t reg, uint16_t value);
  // update registers with given pin value.
  void update_reg_(uint8_t pin, bool pin_value, uint8_t reg_a);

  uint16_t olat_{0x0000};
  // Cache for input values (16-bit combined for both banks)
  uint16_t input_mask_{0x0000};
  InternalGPIOPin *interrupt_pin_{nullptr};
};

class LilyGoTKeyboardS3ProGPIOPin : public GPIOPin {
 public:
  void setup() override;
  void pin_mode(gpio::Flags flags) override;
  bool digital_read() override;
  void digital_write(bool value) override;
  size_t dump_summary(char *buffer, size_t len) const override;

  void set_parent(MCP23016 *parent) { parent_ = parent; }
  void set_pin(uint8_t pin) { pin_ = pin; }
  void set_inverted(bool inverted) { inverted_ = inverted; }
  void set_flags(gpio::Flags flags) { flags_ = flags; }

  gpio::Flags get_flags() const override { return this->flags_; }

 protected:
  MCP23016 *parent_;
  uint8_t pin_;
  bool inverted_;
  gpio::Flags flags_;
};

}  // namespace esphome::lilygo_tkey_s3p
