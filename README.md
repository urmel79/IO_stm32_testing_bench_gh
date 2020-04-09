# STM32 testing bench with PlatformIO

The aim of this project is to build a test environment based on STM32 microcontroller platforms like the **Bluepill F103C8**, the **Nucleo64 F103RB** and the **Maple mini clone** from Baite with the use of PlatformIO and the Arduino framework. So a very important goal is to write the code as portable as possible with many defines.

The onboard LED is flashing like a heartbeat to show all things are "alive". This function is implemented as an finite state machine (FSM) and in an non-blocking manner without delay() functions.

For the purpose of a STM32 test environment, some peripheral sensor modules are planned to integrate like the Bosch BME280 (temperature, humidity and pressure).

STM32 microcontrollers usually don't have any networking interfaces on board like ethernet, wifi or bluetooth. For this reason the most important goal is to integrate a suitable wifi interface. One possibility is to connect a small ESP8266-01 microcontroller via the second hardware serial line Serial2(PA3, PA2) driven with a baud rate of 115200 bps. The challange is, that the vendor fimware of the ESP8266-01 microcontroller communicates over an AT protocol only. Furthermore, some of the AT commands seem not to work like described in the manuals ... (e. g. setting the hostname).

With an established network connection it is possible to implement higher level functionality like a webserver to display the sensor data. An other possibility will be to transfer the sensor data via network protocols like MQTT to a NodeRed server.

## Visual impressions

Coming soon.

## Bill of materials (BOM)

Following parts I have used in this project:

- **Bluepill** board (with STM32 F103C8T6 ARM3 microcontroller)
- ESP8266-01 (as wifi adapter); flashed with original fimware (AT commands are used)
- breadboard adapter for ESP8266-01
- USB-to-serial adapter (chipsets like CP2102 or CH340G are suitable); important: only use the 3.3 V VCC lines!
- USB cable (which one depends on the port of the USB-to-serial adapter)
- BME280 (temperature, humidity and pressure sensor)
- breadboard half+ (400 holes)
- wire jumpers

## USB-to-serial adapters: take a close look

A word of **warning** about some (cheap) USB-to-serial adapters: some of them have VCC pins labelled with **3V3**, but measurements showed something else like **4.4 V**.

These (defective) USB-to-serial adapters look like this:

![cp2102_5v_Breadboard](./fritzing/cp2102_5v_Breadboard_Foto.png)

The 3.3 V external main supply voltage of the Bluepill (STM32 F103C8T6) is **NOT 5 V tolerant** and has maximum ratings of 4.0 V (compare with the [datasheet](https://www.st.com/resource/en/datasheet/stm32f103c8.pdf)). Because of this you can't use this adapter pin for voltage supply of your microcontroller -- otherwise maybe you'll see the *magic blue smoke*.

Instead of this use the +5V pin of the adapter and connect it to the 5V power supply pin of the Bluepill. The voltage regulator of the board will generate the 3.3 V level. Because of these two cases I have created with *Fritzing* (https://fritzing.org) two versions of breadboard layouts and schematics. The UART pins (RxD, TxD) are then also at a level of about 4.4 V  -- but luckily the UART pins (e. g. PA9, PA10) are 5 V tolerant, so it will work :)

## Breadboard layout and schematics (3.3 V version)

Here is the breadboard layout with the 3.3 V capable USB-to-serial adapter:

![Breadboard Layout 3.3V](./fritzing/Bluepill_wifi_cp2102_3v3_Breadboard.png)

The schematics looks like this:

![Schematics 3.3V](./fritzing/Bluepill_wifi_cp2102_3v3_Schematics.png)

## Breadboard layout and schematics (5 V version)

Here is the breadboard layout with the 5 V pin used of the (defective) USB-to-serial adapter:

![Breadboard Layout 5V](./fritzing/Bluepill_wifi_cp2102_5v_Breadboard.png)

The schematics looks like this:

![Schematics 5V](./fritzing/Bluepill_wifi_cp2102_5v_Schematics.png)

## Software libraries and documentation

Go to the source code: I have it supplied with many comments for explanation.

## License

This project is licensed under the terms of "GNU General Public License v3.0". For details see [LICENSE](LICENSE).

## Implemented functionality

- onboard LED is flashing like heartbeat; implemented in an FSM
- sensor module Bosch BME280 (temperature, humidity and pressure) is read via I2C line and written to the serial console
- wifi connection is established with an ESP8266-01 microcontroller via 2. hardware serial line
- webserver shows a very basic html website with the time passed since last reboot

## Todo and known issues

[Todo 2020-03-31] Establishing the wifi connection is very slow and not very reliable. It has to be rewritten in an finite state machine (FSM).

[Todo 2020-03-31] Sensor data of the BME280 should be displayed on the webserver.

[Issue 2020-03-31] Webserver responses are very unstable and unreliable. Elimination of delay() functions may be a solution and rewrite it in an non-blocking manner.


















<!--  -->
