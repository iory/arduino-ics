# Arduino Library for ICS (Kondo Servo)

[![Build Status](https://github.com/iory/arduino-ics/actions/workflows/test.yml/badge.svg?branch=main)](https://github.com/iory/arduino-ics/actions)

This is a library for Kondo servos that supports Arduino and ESP with PlatformIO.
You can easily perform ICS communication and control Kondo servos with it.

## Quick Example

### Prerequirements

Install udev to give permission to the device.

```
curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core/develop/platformio/assets/system/99-platformio-udev.rules | sudo tee /etc/udev/rules.d/99-platformio-udev.rules
sudo udevadm control --reload-rules
sudo udevadm trigger
```

Ubuntu/Debian users may need to add own “username” to the “dialout” group if they are not “root”, doing this issuing

```
sudo usermod -a -G dialout $USER
sudo usermod -a -G plugdev $USER
```

```
pip3 install platformio -U
```

### Compile and write firmware to arduino

Connect the servo as shown in the diagram below. Ensure that a 2.2kΩ resistor is placed between the RX and TX pins for proper operation.

For more details on the serial servo configuration, please refer to the official documentation:
[Kondo Serial Servo Wiring](https://kondo-robot.com/faq/serial-servo-method-tech)
![Wiring Example](./docs/arduino_nano_every.jpg)

```
pio run -e example_rotate_nano_every -t upload
```

You should be able to confirm that the servo is rotating.

## Contributing

### Clang-Format
To install `clang-format`, you can use the following command:
```
pip install clang-format
```

Once installed, you can format your files using a `.clang-format` configuration file. To format a file, run:
```
clang-format -i <filename>
```

Replace `<filename>` with the name of the file you want to format. The `-i` option tells `clang-format` to edit the file in place.
