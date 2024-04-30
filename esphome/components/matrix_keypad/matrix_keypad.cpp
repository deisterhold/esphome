#include "matrix_keypad.h"
#include "esphome/core/log.h"

namespace esphome {
namespace matrix_keypad {

static const char *const TAG = "matrix_keypad";

void MatrixKeypad::setup() {
  for (auto *pin : this->columns_) {
    pin->pin_mode(gpio::FLAG_OUTPUT);
    pin->digital_write(true);
  }
  for (auto *pin : this->rows_) {
    pin->pin_mode(gpio::FLAG_INPUT | gpio::FLAG_PULLUP);
  }
}

void MatrixKeypad::loop() {
  for (auto *pin : this->columns_) {
    pin->digital_write(true);
  }

  static uint32_t active_start = 0;
  static int active_key = -1;
  uint32_t now = millis();
  int key = -1;
  bool error = false;
  int pos = 0, row, col;
  for (auto *col : this->columns_) {
    col->digital_write(false);
    for (auto *row : this->rows_) {
      bool pressed = !row->digital_read();
      if (pressed) {
        if (key != -1) {
          error = true;
        } else {
          key = pos;
        }
      }
      pos++;
    }
    col->digital_write(true);
  }
  if (error)
    return;

  if (key != active_key) {
    if ((active_key != -1) && (this->pressed_key_ == active_key)) {
      row = this->pressed_key_ % this->rows_.size();
      col = this->pressed_key_ / this->rows_.size();
      ESP_LOGD(TAG, "key @ row %d, col %d released", row, col);
      for (auto &listener : this->listeners_)
        listener->button_released(row, col);
      if (!this->keys_.empty()) {
        uint8_t keycode = this->keys_[this->pressed_key_];
        ESP_LOGD(TAG, "key '%c' released", keycode);
        for (auto &listener : this->listeners_)
          listener->key_released(keycode);
      }
      this->pressed_key_ = -1;
    }

    active_key = key;
    if (key == -1)
      return;
    active_start = now;
  }

  if ((this->pressed_key_ == key) || (now - active_start < this->debounce_time_))
    return;

  row = key % this->rows_.size();
  col = key / this->rows_.size();
  ESP_LOGD(TAG, "key @ row %d, col %d pressed", row, col);
  for (auto &listener : this->listeners_)
    listener->button_pressed(row, col);
  if (!this->keys_.empty()) {
    uint8_t keycode = this->keys_[key];
    ESP_LOGD(TAG, "key '%c' pressed", keycode);
    for (auto &listener : this->listeners_)
      listener->key_pressed(keycode);
    this->send_key_(keycode);
  }
  this->pressed_key_ = key;
}

void MatrixKeypad::dump_config() {
  ESP_LOGCONFIG(TAG, "Matrix Keypad:");
  ESP_LOGCONFIG(TAG, " Rows:");
  for (auto &pin : this->rows_) {
    LOG_PIN("  Pin: ", pin);
  }
  ESP_LOGCONFIG(TAG, " Cols:");
  for (auto &pin : this->columns_) {
    LOG_PIN("  Pin: ", pin);
  }
}

void MatrixKeypad::register_listener(MatrixKeypadListener *listener) { this->listeners_.push_back(listener); }

}  // namespace matrix_keypad
}  // namespace esphome
