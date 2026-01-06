#include "pn532_uart.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
// #include "esphome/core/hal.h"

// Based on:
// - https://cdn-shop.adafruit.com/datasheets/PN532C106_Application+Note_v1.2.pdf
// - https://www.nxp.com/docs/en/nxp/application-notes/AN133910.pdf
// - https://www.nxp.com/docs/en/nxp/application-notes/153710.pdf

namespace esphome {
namespace pn532_uart {

static const char *const TAG = "pn532_uart";

// Maximum bytes to log in verbose hex output
static constexpr size_t PN532_MAX_LOG_BYTES = 64;

void PN532UART::setup() {
  // clear out anything in read buffer
  while (this->available())
    this->read();

  delay(10);

  // wakeup device
  this->write_array({0x55, 0x00, 0x00});
  delay(2);

  PN532::setup();
}

void PN532UART::dump_config() {
  PN532::dump_config();
  ESP_LOGCONFIG(TAG, "  Baud rate: %d", this->parent_->get_baud_rate());
}

bool PN532UART::is_read_ready() { return (this->available() != 0); }

bool PN532UART::write_data(const std::vector<uint8_t> &data) {
  this->write_array(data.data(), data.size());
  return true;
}

bool PN532UART::read_data(std::vector<uint8_t> &data, uint8_t len) {
  delay(1);

  if (this->read_ready_(true) != pn532::PN532ReadReady::READY) {
    return false;
  }

  data.resize(len + 1);
  this->read_array(data.data(), len + 1);
  // std::reverse(data.begin(), data.end());
#if ESPHOME_LOG_LEVEL >= ESPHOME_LOG_LEVEL_VERBOSE
  char hex_buf[format_hex_pretty_size(PN532_MAX_LOG_BYTES)];
#endif
  ESP_LOGV(TAG, "Read data: %s", format_hex_pretty_to(hex_buf, sizeof(hex_buf), data.data(), data.size()));
  return true;
}

bool PN532UART::read_response(uint8_t command, std::vector<uint8_t> &data) {
  ESP_LOGV(TAG, "Reading response");
  uint8_t len = this->read_response_length_();
  if (len == 0) {
    return false;
  }

  ESP_LOGV(TAG, "Reading response of length %d", len);
  if (!this->read_data(data, 6 + len + 2)) {
    ESP_LOGD(TAG, "No response data");
    return false;
  }

  if (data[1] != 0x00 && data[2] != 0x00 && data[3] != 0xFF) {
    // invalid packet
    ESP_LOGV(TAG, "read data invalid preamble!");
    return false;
  }

  bool valid_header = (static_cast<uint8_t>(data[4] + data[5]) == 0 &&  // LCS, len + lcs = 0
                       data[6] == 0xD5 &&                               // TFI - frame from PN532 to system controller
                       data[7] == command + 1);                         // Correct command response

  if (!valid_header) {
    ESP_LOGV(TAG, "read data invalid header!");
    return false;
  }

  data.erase(data.begin(), data.begin() + 6);  // Remove headers

  uint8_t checksum = 0;
  for (int i = 0; i < len + 1; i++) {
    uint8_t dat = data[i];
    checksum += dat;
  }
  checksum = ~checksum + 1;

  if (data[len + 1] != checksum) {
    ESP_LOGV(TAG, "read data invalid checksum! %02X != %02X", data[len], checksum);
    return false;
  }

  if (data[len + 2] != 0x00) {
    ESP_LOGV(TAG, "read data invalid postamble!");
    return false;
  }

  data.erase(data.begin(), data.begin() + 2);  // Remove TFI and command code
  data.erase(data.end() - 2, data.end());      // Remove checksum and postamble

  return true;
}

uint8_t PN532UART::read_response_length_() {
  std::vector<uint8_t> data;
  if (!this->read_data(data, 6)) {
    return 0;
  }

  if (data[1] != 0x00 && data[2] != 0x00 && data[3] != 0xFF) {
    // invalid packet
    ESP_LOGV(TAG, "read data invalid preamble!");
    return 0;
  }

  bool valid_header = (static_cast<uint8_t>(data[4] + data[5]) == 0 &&  // LCS, len + lcs = 0
                       data[6] == 0xD5);                                // TFI - frame from PN532 to system controller

  if (!valid_header) {
    ESP_LOGV(TAG, "read data invalid header!");
    return 0;
  }

  this->send_nack_();

  // full length of message, including TFI
  uint8_t full_len = data[4];
  // length of data, excluding TFI
  uint8_t len = full_len - 1;
  if (full_len == 0)
    len = 0;
  return len;
}

}  // namespace pn532_uart
}  // namespace esphome
