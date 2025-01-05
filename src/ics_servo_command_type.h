#ifndef ICS_SERVO_COMMAND_TYPE_H
#define ICS_SERVO_COMMAND_TYPE_H

enum ICSServoCommandType {
  ACTUATOR_CONTROL = 0x80,
  READ_PARAMETER = 0xA0,
  WRITE_PARAMETER = 0xC0,
  ID_REQUEST = 0xE0
};

#endif // ICS_SERVO_COMMAND_TYPE_H
