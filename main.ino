// Définition of the libraries
#include <SoftwareSerial.h>
#include <math.h>
// Définition of a global variable to debug
#define DEBUG true

// Init of the serial in the way that : RX->TX et TX->RX
SoftwareSerial ESP8266(2,3); // RX,TX

// Wifi informations
String ssid = "your_wifi_ssid";
String password = "your_wifi_password";

// Data lists 
byte moisture_list[5];
float temperature_list[5];
unsigned long lum_list[5];

// Variables to keep track of which measure to take
byte trackMeasure = 0;
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(74880);
  ESP8266.begin(74880);
  initESP8266();
  display_freeram();
}

void loop() {
  if (millis() - previousMillis >= 15000) { // Change duration between each measure if needed
    previousMillis = millis();

    if(trackMeasure <= 4) {
      // Read analog values and store it in the lists
      moisture_list[trackMeasure] = analogRead(A0)/7.15;
      temperature_list[trackMeasure] = 1.0/(log(1023.0/analogRead(A1)-1.0)/4275+1/298.15)-273.15;
      lum_list[trackMeasure] = analogRead(A2)/7.7;

      // Show the values retrivied 
      Serial.println(moisture_list[trackMeasure]);
      Serial.println(temperature_list[trackMeasure]);
      Serial.println(lum_list[trackMeasure]);

      // Move the cursor by 1
      trackMeasure += 1;
    }
    else {
      // Reset the cursor
      trackMeasure = 0;
    }
  }

  if(ESP8266.available())
  {
    display_freeram();
    String response = ESP8266.readStringUntil('H'); // Cut the response string to free up ram
    Serial.println(response);
    if (response.indexOf(F("+IPD,")) >= 0)
    {
      // Send command to prepare to the webpage send
      String cipSend = F("AT+CIPSEND=");
      cipSend += response.substring(0, 1);
      cipSend += F(",");
      cipSend += 1150;
      cipSend += F("\r\n");
      sendData(cipSend,2000,DEBUG);

      // Send webpage
      String test = F("<!doctype html><html lang=fr><head><meta content='text/html;charset=utf-8' http-equiv=Content-Type><style>table{border-collapse:collapse;border:2px solid #000;font-family:'Franklin Gothic Medium'}caption{padding:10px}tfoot,thead{background-color:#9eeab9}td,th{border:1px solid rgb(160 160 160);padding:8px 10px}td:last-of-type{text-align:center}</style></head><body><main><Table><caption>Rapports des trois dernieres heures</caption><thead><tr id=colonnes><th scope=col>Temperature</th><th scope=col>Humidite</th><th scope=col>Luminosite</th></tr></thead><tbody><tr><th scope=row>");
      test += temperature_list[0];
      test += F(" °C (-1h)</th><td>");
      test += moisture_list[0];
      test += F(" % (-1h)</td><td>");
      test += lum_list[0];
      test += F(" % (-1h)</td></tr><tr><th scope=row>");
      test += temperature_list[1];
      test += F(" °C (-2h)</th><td>");
      test += moisture_list[1]; 
      test += F(" % (-2h)</td><td>");
      test += lum_list[1];
      test += F("  % (-2h)</td></tr><tr><th scope=row>");
      test += temperature_list[2];
      test += F(" °C (-3h)</th><td>");
      test += moisture_list[2];
      test += F(" % (-3h)</td><td>");
      test += lum_list[2];
      test += F(" % (-3h)</td></tr><tr><th scope=row>");
      test += temperature_list[3];
      test += F(" °C (-4h)</th><td>");
      test += moisture_list[3]; 
      test += F(" % (-4h)</td><td>");
      test += lum_list[3];
      test += F(" % (-4h)</td></tr><tr><th scope=row>");
      test += temperature_list[4];
      test += F(" °C (-5h)</th><td>");
      test += moisture_list[4]; 
      test += F(" % (-5h)</td><td>");
      test += lum_list[4];
      test += F(" % (-5h)</td></tr></tbody><tfoot><tr><th scope=row colspan=2>Temperature Moyenne</th><td>");
      test += (temperature_list[0] + temperature_list[1] + temperature_list[2] + temperature_list[3] + temperature_list[4])/5;
      test += F(" °C</td></tr><tr><th scope=row colspan=2>Luminosite moyenne</th><td>");
      test += (lum_list[0] + lum_list[1] + lum_list[2] + lum_list[3] + lum_list[4])/5;
      test += F(" %</td></tr></tfoot></Table></main></body></html>");
      ESP8266.print(test);
    }
    display_freeram();
  }
}

void initESP8266() {
  display_freeram();
  sendData(F("AT\r\n"),3000,DEBUG); // init module
  sendData(F("AT+CWMODE=3\r\n"),3000,DEBUG); // configure as access point and client
  sendData("AT+CWJAP=\""+ ssid + "\",\""+ password +"\"\r\n",10000,DEBUG); // connect to Wifi with credentials
  sendData(F("AT+CIPMUX=1\r\n"),3000,DEBUG); // configure for multiple connections
  sendData(F("AT+CIPSERVER=1,80\r\n"),3000,DEBUG); // turn on server on port 80
  sendData(F("AT+CIFSR\r\n"),3000,DEBUG); // get ip address
}

void sendData(String command, const int timeout, boolean debug)
{
  ESP8266.print(command); // send the command to the esp8266
  
  long int time = millis();
  while( (time+timeout) > millis())
  {
    if(debug && ESP8266.available())
    {
      Serial.println(ESP8266.readString());
    }
  }
}


// Small functions to measure the freeram at a given time
void display_freeram() {
  Serial.print(F("- SRAM left: "));
  Serial.println(freeRam());
}

int freeRam() {
  extern int __heap_start,*__brkval;
  int v;
  return (int)&v - (__brkval == 0  
    ? (int)&__heap_start : (int) __brkval);  
}
