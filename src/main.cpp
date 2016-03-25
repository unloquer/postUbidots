#include "constants.h"
#include "Arduino.h"
#include <ESP8266WiFi.h>
// https://developer.ibm.com/recipes/tutorials/use-http-to-send-data-to-the-ibm-iot-foundation-from-an-esp8266/
#include <ESP8266HTTPClient.h>
//
#define errorPin 2
WiFiClient client;
HTTPClient http;

/*
  # Request.
  $ curl -XPOST -H 'Content-Type: application/json;' -H 'X-Auth-Token: AUTH_TOKEN' -d '{"value": 23}' http://things.ubidots.com/api/v1.6/variables/ID_VARIABLE/values

  # Response.
  {
  "url": "http:///things.ubidots.com/api/v1.6/values/ID_VARIABLE",
  "value": 23.0,
  "timestamp": 1369847742302,
  "created_at": "2013-08-29T17:15:42.302"
  }
*/

/*
  ¿Cómo obtener el string de respuesta?
  // http://stackoverflow.com/questions/34565090/esp8266-display-received-http-response
  // http://stackoverflow.com/questions/35949834/url-string-not-submitting-in-arduino-ide-with-esp8266

*/

void setup(){

  Serial.begin(115200);
  pinMode(errorPin, OUTPUT);
  const char* ssid = ESSID;
  const char* password = WIFI_PASSWORD;

  for (int i=0;i<4; i++){   // let know we are working
    digitalWrite(errorPin ,HIGH);
    delay(200);
    digitalWrite(errorPin ,LOW);
    delay(200);
  }

  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");
}

////////////////////////////////////////////////////////////////////////////////

void loop() {
  String url = "http://things.ubidots.com/api/v1.6/variables/" ID_VARIABLE "/values";
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-Auth-Token", AUTH_TOKEN);
  int content_length =0;
  String payload = String("{ \"value\": " + String(millis()/1000) + "}");
  //content_length = payload.length();
  //http.addHeader("Content-Length: ", String(content_length));
  int httpCode = http.POST(payload);
  if(httpCode > 0) {
    String payload = http.getString();
    Serial.println(payload);
  }
  else {
    Serial.print("[HTTP] failed, error: ");Serial.println(http.errorToString(httpCode).c_str());
  }
  http.end();
  delay(20000);
}
