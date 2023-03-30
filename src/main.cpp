#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>

#include "html.h"

#define INPUT_BUTTON D4

const char *ssid = "MTS81231FT_460C";
const char *password = "20101462";

AsyncWebServer server(80); // Webserver Object
WiFiClient wifiClient;

String tickets = "";
byte tries = 10; // Попыток подключения к точке доступа

void setup()
{
  Serial.begin(9600); // Открыть последовательное соединение
  WiFi.begin(ssid, password);
  pinMode(INPUT_BUTTON, INPUT_PULLUP);
  while (--tries && WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Non Connecting to WiFi..");
  }
  else
  {
    // Иначе удалось подключиться отправляем сообщение
    // о подключении и выводим адрес IP
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }

  delay(200);

  Serial.println("Waiting to connect…");

  server.on(
      "/",
      HTTP_GET,
      [](AsyncWebServerRequest *request)
      {
        request->send(200, "text/html", htmlMessage);
      });

  server.on(
      "/post",
      HTTP_POST,
      [](AsyncWebServerRequest *request) {},
      NULL,
      [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
      {
        for (size_t i = 0; i < len; i++)
        {
          tickets.concat(String(data[i]));
        }

        Serial.println(tickets);
        request->send(200);
      });

  server.begin();
  Serial.println("Server listening");
}

void loop()
{
  /*delay(100);
  if (digitalRead(INPUT_BUTTON)==LOW){
    delay(5);
     Serial.println(Ticket);
     delay(1000);
  }
  */
  delay(20000);
}
