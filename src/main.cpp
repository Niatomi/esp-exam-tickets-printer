#include <Arduino.h>

#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#include "AsyncJson.h"
#include "ArduinoJson.h"

#include "html.h"
#include "config.h"

#define INPUT_BUTTON 4

AsyncWebServer server(80); // Webserver Object
WiFiClient wifiClient;
DynamicJsonDocument doc(16000);

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
        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", htmlMessage);
        response->addHeader("Server", "WebServer");
        request->send(response);
      });

  AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler(
      "/post-tickets",
      [](AsyncWebServerRequest *request, JsonVariant &json)
      {
        doc.clear();
        doc = json.as<JsonArray>();
        request->send(200, "application/text", "Tickets saved");
      });
  server.addHandler(handler);

  server.begin();
  Serial.println("Server listening");
}

int current_ticket_index = 0;
void loop()
{
  //  Вот сюда надо добавить обработчик на кнопку
  //  Сейчас он просто работает на то, пустой документ, или нет
  if (doc.isNull() == false)
  {
    JsonArray array = doc.as<JsonArray>();
    JsonVariant v = array[current_ticket_index];
    Serial.println(v["ticket"].as<String>());
    current_ticket_index++;
    if (current_ticket_index == array.size())
    {
      current_ticket_index = 0;
    }
  }
  delay(5);
}