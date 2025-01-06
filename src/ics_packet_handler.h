#ifndef ICS_PACKET_HANDLER_H
#define ICS_PACKET_HANDLER_H

#include <cstdint>
#include <cstddef>

class ICSPacketHandler {
public:
  explicit ICSPacketHandler(uint8_t device_id)
    : buffer_pos_(0),
      packet_ready_(false),
      id_changed_(false) {
    setId(device_id);
    id_changed_ = false;
  }

  virtual ~ICSPacketHandler() = default;

  uint8_t getId() const {
    return id_;
  }

  uint8_t setId(uint8_t target_id) {
    id_ = target_id;
    id_changed_ = true;
    return id_;
  }

  bool idChanged() {
    bool ret = id_changed_;
    id_changed_ = false;
    return ret;
  }

  static uint8_t extractTargetId(const uint8_t* packet) {
    return (packet != nullptr) ? (packet[0] & 0x1F) : 0xFF;
  }

  bool handleIdRequest(uint8_t* response, size_t& response_length) {
    if (!buffer_) return false;

    if (buffer_[1] == 0x00 && buffer_[2] == 0x00 && buffer_[3] == 0x00) {
      response[0] = getId();
      response_length = 1;
      return true;
    } else if (buffer_[1] == 0x01 && buffer_[2] == 0x01 && buffer_[3] == 0x01) {
      response[0] = setId(buffer_[0] & 0x1F);
      response_length = 1;
      return true;
    }
    return false;
  }

  bool processByte(uint8_t byte) {
    if (buffer_pos_ == 0) {
      if (!isValidHeader(byte)
          || (((byte & 0xE0) != 0xE0) && (byte & 0x1F) != getId())) {
        return false;
      }
    }
    if (buffer_pos_ >= MAX_PACKET_SIZE) {
      resetBuffer();
    }
    buffer_[buffer_pos_++] = byte;

    if (buffer_pos_ == 1 && !isValidHeader(buffer_[0])) {
      resetBuffer();
    }

    if (buffer_pos_ > 1) {
      size_t expected_length = determinePacketLength(buffer_);
      if (expected_length > 0 && buffer_pos_ >= expected_length) {
        packet_ready_ = true;
        buffer_pos_ = 0;
      }
    }

    return packet_ready_;
  }

  void resetBuffer() {
    buffer_pos_ = 0;
    packet_ready_ = false;
  }

protected:
  virtual bool isValidHeader(uint8_t byte) const {
    return (byte & 0xE0) != 0;
  }

  virtual size_t determinePacketLength(const uint8_t* header) const = 0;
  virtual bool handleActuatorRequest(uint8_t* response, size_t& responseLength) = 0;
  virtual bool handleDataRequest(uint8_t* response, size_t& responseLength) = 0;
  virtual bool handleParameterSetting(uint8_t* response, size_t& responseLength) = 0;

protected:
  uint8_t id_;
  static constexpr size_t MAX_PACKET_SIZE = 66;
  uint8_t buffer_[MAX_PACKET_SIZE];
  size_t buffer_pos_;
  bool packet_ready_;
  bool id_changed_;
};

#endif // ICS_PACKET_HANDLER_H
