# 🪴 TSErre - An all in One Module to Monitor Your Plant !

## Overview
This project involves creating a data logger using an ESP8266 WiFi module interfaced with an Arduino. The logger measures soil moisture, temperature, and luminosity at regular intervals and serves the collected data via a web server. The data is displayed in a tabular format on a webpage, showing the measurements taken over the last five intervals.

## Hardware Requirements
- Arduino (Uno or similar)
- ESP8266 WiFi module
- Soil moisture sensor
- Temperature sensor
- Luminosity sensor
- Connecting wires
- Breadboard (optional)

## Software Requirements
- Arduino IDE
- ESP8266 library for Arduino
- Math library for Arduino

## Libraries Used
- `SoftwareSerial.h` for communication with the ESP8266
- `math.h` for mathematical calculations

## Global Constants and Variables
- `DEBUG`: A boolean flag for enabling/disabling debug messages.
- `SoftwareSerial ESP8266(2, 3)`: Initializes the software serial communication on pins 2 (RX) and 3 (TX).
- WiFi credentials: `String ssid` and `String password`.
- Data arrays: `moisture_list`, `temperature_list`, `lum_list` for storing sensor readings.
- `trackMeasure`: A counter to keep track of the current measurement index.
- `previousMillis`: Stores the last time a measurement was taken.

## Setup Function
1. Initializes serial communication for debugging and ESP8266 communication.
2. Calls `initESP8266` to configure the ESP8266 module.
3. Displays the initial free RAM.

## Loop Function
1. Takes sensor readings every intervals.
2. Stores the readings in the respective arrays.
3. Sends the data to the ESP8266 to be served as a webpage.
4. Resets the `trackMeasure` counter after five readings.

## Helper Functions
### `initESP8266()`
- Initializes the ESP8266 and configures it as both an access point and a client.
- Connects to the specified WiFi network.
- Sets up a web server on port 80.

### `sendData(String command, const int timeout, boolean debug)`
- Sends AT commands to the ESP8266.
- Waits for the specified timeout duration for a response.
- Optionally prints debug messages.

### `display_freeram()`
- Prints the amount of free SRAM available.

### `freeRam()`
- Returns the amount of free SRAM available.

## Web Server Functionality
- The ESP8266 serves a webpage displaying the last five sets of sensor readings.
- The webpage is styled using basic HTML and CSS.
- Calculates and displays average temperature and luminosity over the last five readings.

## Usage
1. **Hardware Connections:**
   - Connect the sensors to the appropriate analog pins on the Arduino.
   - Connect the ESP8266 to the Arduino as per the `SoftwareSerial` pin configuration (2, 3).
2. **Upload Code:**
   - Update the `ssid` and `password` with your WiFi credentials.
   - Upload the provided code to the Arduino using the Arduino IDE.
3. **Operation:**
   - Open the Serial Monitor at 74880 baud rate to observe debug messages.
   - Access the ESP8266's IP address on a web browser to view the sensor data.

## Notes
- Adjust the `millis()` interval in the `loop` function to change the measurement frequency.
- Ensure the ESP8266 baud rate matches the one defined in the code (74880).
- The `freeRam` function is used to monitor memory usage, which is crucial for debugging and ensuring stability.

## Troubleshooting
- If the ESP8266 doesn't connect to WiFi, check the `ssid` and `password`.
- Ensure proper sensor connections and that they are functioning correctly.
- Use the Serial Monitor to view debug messages for diagnosing issues.

## License
This project is open-source and available for use and modification under the MIT License.
