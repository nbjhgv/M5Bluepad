# Overview
M5Bluepad library features extension classes for easier use of [Bluepad32](https://github.com/ricardoquesada/bluepad32) with [M5Stack](https://m5stack.com/) units.

# Bluetooth remote control for M5Stack Atom-Lite and Atomic Motion Base
**M5Stack Atomic Motion Base** is kind of Arduino shield for controlling servo motors. It's designed to be used with Atom series controllers from M5 Stack.

**Atom-Lite** is based on ESP32 and has Classic Bluetooth support so it can establish wireless connection with most modern gamepads, including XBox Wireless Controller and Sony DualShock/DualSense. 

This combo makes possible building small size yet powerful and extensible remote control machines like RC-vehicles or robots.

PICTURE!!!

YouTube [demo 1](https://www.youtube.com/watch?v=nqUzDYmt8Ac), [demo 2](https://www.youtube.com/watch?v=292cAnSWLGk)

# Using BluepadHub with other controllers
While this library was initially created for using with Atom-Lite, it is also compatible with other M5Stack controllers based on 
ESP32-PICO-D4, ESP32-PICO-V3 or ESP32-D0WD chips and even with non-M5Stack boards having Bluetooth Classic support, like ESP32-PICO-MINI02 or ESP-WROOM-32. 

The main **BluepadHub** class in this library is M5Stack-independent, so it can be used to implement bluetooth remote control for other type of devices even not listed here.

| Supported M5Stack controllers  | Unsupported controllers   |
| :---           |    :----        |   
| :white_check_mark: Atom-Lite   | :x: AtomS3-Lite|
| :white_check_mark: Atom-Matrix | :x: AtomS3|
| :white_check_mark: Atom-Echo   | :x: Atom EchoS3R|
| :white_check_mark: Core2   | :x: CoreS3|
| :white_check_mark: Core Basic   |  |
| :white_check_mark: StickC-Plus2    |  |
| :white_check_mark: All controllers based on ESP32-PICO-D4, ESP-32-PICO-V3 and ESP32-D0WD   | :x: All controllers based on ESP32-S3, ESP32-C3, and ESP32-C6|

# Supported servo and motor control units
Currently, the only supported servo/motor control unit is M5Stack Atomic Motion Base.

| Unit  | Status   |
| :---           |    :----        |   
| M5Stack Atomic Motion Base   | :white_check_mark: Supported in this library |
| M5Stack Atomic HDriver Base  | Planned |
| M5Stack Unit 8Servos   | Planned |
| M5Stack Unit HBridge   | Planned |
| Other units and devices | Can be impemented with custom code in sketch |

# Using the library

1. Install boards and libraries in Arduino IDE ([wiki](https://github.com/pink0D/M5Bluepad/wiki/Arduino-IDE-configuration))
2. Start with uploading TestBluepad32 example (link!) to test connection between your gamepad and the controller
3. Continue to explore AtomicLiteMotion sketch (link) and learn how BluepadHub handles gamepad's input to control motors and servos  

# Contacts
EMail: [pink0D.github@gmail.com](mailto:pink0d.github@gmail.com)
