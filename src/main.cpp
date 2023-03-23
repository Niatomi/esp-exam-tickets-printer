#include <Arduino.h>

#include <WiFi.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80); // Webserver Object
const char *ssid = "GD-2";
const char *password = "9171016278";

WiFiClient wifiClient;

byte tries = 10; // Попыток подключения к точке доступа
int input5 = 5;
unsigned long lastDebounceTime = 0; // время, когда состояние пина изменилось последний раз
unsigned long debounceDelay = 50;   // задержка для защиты от дребезга контактов;

#include "html.h"
const char *PARAM_MESSAGE = "message";
void setup()
{
  Serial.begin(9600); // Открыть последовательное соединение
  WiFi.begin(ssid, password);
  pinMode(input5, INPUT);
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

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/html", htmlMessage); });

  server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request)
            {
        String message;
        if (request->hasParam(PARAM_MESSAGE, true)) {
            message = request->getParam(PARAM_MESSAGE, true)->value();
        } else {
            message = "No message sent";
        }
        Serial.println(message);
        request->send(200, "text/html", "Hello, POST: " + message); });

  server.begin();
  Serial.println("Server listening");
}

void loop()
{

  /*// считываем состояние переключателя в локальную переменную:
  int reading = digitalRead(input5);
  // подождите немного и проверьте не изменился ли сигнал
  // (с LOW на HIGH) с момента последнего нажатия чтобы исключить дребезг:
  // Если состояние изменилось из-за дребезга или случайного нажатия:
  if (reading != lastButtonState) {
  // сбрасываем таймер
  lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
  // вне зависимости от действительного состояния,
  //если оно длится больше задержки, то принимаем его за текущее:
  // если состояние кнопки изменилось:
  if (reading != buttonState) {
  buttonState = reading;
  //Печатаем билет
  if (buttonState == HIGH) {
  ....
  }
  }*/
}
