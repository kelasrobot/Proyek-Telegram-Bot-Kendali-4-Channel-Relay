#include"settings.h"
#define USE_CLIENTSSL true
#include <AsyncTelegram2.h>
#include <time.h>
#define MYTZ "WIB-7"
#ifdef ESP8266
#include <ESP8266WiFi.h>
BearSSL::WiFiClientSecure client;
BearSSL::Session   session;
BearSSL::X509List  certificate(telegram_cert);
#elif defined(ESP32)
#include <WiFi.h>
#include <WiFiClient.h>
#if USE_CLIENTSSL
#include <SSLClient.h>
#include "tg_certificate.h"
WiFiClient base_client;
SSLClient client(base_client, TAs, (size_t)TAs_NUM, A0, 1, SSLClient::SSL_ERROR);
#else
#include <WiFiClientSecure.h>
WiFiClientSecure client;
#endif
#endif
AsyncTelegram2 myBot(client);



void setup() {
  pinMode(relay1, OUTPUT);
  digitalWrite(relay1, HIGH);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay2, HIGH);
  pinMode(relay3, OUTPUT);
  digitalWrite(relay3, HIGH);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("\nStarting TelegramBot...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  delay(500);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
#ifdef ESP8266
  configTime(MYTZ, "time.google.com", "time.windows.com", "pool.ntp.org");
  client.setSession(&session);
  client.setTrustAnchors(&certificate);
  client.setBufferSizes(1024, 1024);
#elif defined(ESP32)
  configTzTime(MYTZ, "time.google.com", "time.windows.com", "pool.ntp.org");
#if USE_CLIENTSSL == false
  client.setCACert(telegram_cert);
#endif
#endif
  myBot.setUpdateTime(2000);
  myBot.setTelegramToken(token);
  Serial.print("\nTest Telegram connection... ");
  myBot.begin() ? Serial.println("OK") : Serial.println("NOK");

  //  char welcome_msg[128];
  //  snprintf(welcome_msg, 128, "BOT @%s online\n/help all commands avalaible.", myBot.getBotName());
  //  int64_t userid = 123456789;
  //  myBot.sendTo(userid, welcome_msg);

}

void loop() {
  static uint32_t ledTime = millis();
  if (millis() - ledTime > 150) {
    ledTime = millis();
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
  TBMessage msg;
  if (myBot.getNewMessage(msg)) {
    String message ;
    message += "Message from @";
    message += myBot.getBotName();
    message += ":\n";
    message += msg.text;
    Serial.println(message);

    if (msg.text == "/start") {
      String pesan = "Selamat datang di Telegram Bot Informatika UNSIA\n";
      pesan += "Silakan gunakan perintah berikut:\n\n";
      pesan += "/lampu1ON untuk menyalakan lampu 1\n";
      pesan += "/lampu1OFF untuk mematikan lampu 1\n";
      pesan += "/lampu2ON untuk menyalakan lampu 2\n";
      pesan += "/lampu2OFF untuk mematikan lampu 2\n";
      pesan += "/lampu3ON untuk menyalakan lampu 3\n";
      pesan += "/lampu3OFF untuk mematikan lampu 3\n";
      myBot.sendMessage(msg, pesan);
    }
    if (msg.text == "/lampu1ON") {
      digitalWrite(relay1, LOW);
      delay(1000);
      myBot.sendMessage(msg, "Lampu 1 Telah Dinyalakan!");
    }
    if (msg.text == "/lampu1OFF") {
      digitalWrite(relay1, HIGH);
      delay(1000);
      myBot.sendMessage(msg, "Lampu 1 Telah Dimatikan!");
    }
    if (msg.text == "/lampu2ON") {
      digitalWrite(relay2, LOW);
      delay(1000);
      myBot.sendMessage(msg, "Lampu 2 Telah Dinyalakan!");
    }
    if (msg.text == "/lampu2OFF") {
      digitalWrite(relay2, HIGH);
      delay(1000);
      myBot.sendMessage(msg, "Lampu 2 Telah Dimatikan!");
    }
    if (msg.text == "/lampu3ON") {
      digitalWrite(relay3, LOW);
      delay(1000);
      myBot.sendMessage(msg, "Lampu 3 Telah Dinyalakan!");
    }
    if (msg.text == "/lampu3OFF") {
      digitalWrite(relay3, HIGH);
      delay(1000);
      myBot.sendMessage(msg, "Lampu 3 Telah Dimatikan!");
    }

  }
}
