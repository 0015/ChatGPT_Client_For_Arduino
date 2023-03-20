/*
 * Project ChatGPT Client For Arduino
 * Description: For HTTPS connection using ArduinoBearSSL
 * Author: Eric Nam
 * Date: 03-08-2023
 */

//#define ARDUINO_DISABLE_ECCX08
#include <ArduinoBearSSL.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <ChatGPT.hpp>

static const char *ssid = "<WIFI_SSID>";
static const char *password = "<WIFI_PW>";

WiFiClient client;
BearSSLClient sslClient(client);
ChatGPT<BearSSLClient> chat_gpt(&sslClient, "v1", "<OpenAI_API_KEY>");

int status = WL_IDLE_STATUS;

unsigned long getTime() {
  return WiFi.getTime();
}

void setup() {
  Serial.begin(115200);
  if (WiFi.status() == WL_NO_SHIELD)
    return;

  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    status = WiFi.begin(ssid, password);
    delay(1000);
  }
  Serial.println("Connected!");

  ArduinoBearSSL.onGetTime(getTime);

  // Disable Server Name Indication:
  // for testing purposes only
  // DO NOT USE IN PRODUCTION
  sslClient.setInsecure(BearSSLClient::SNI::Insecure);

  String result;

  Serial.println("[ChatGPT] Only print a content message");
  if (chat_gpt.simple_message("gpt-3.5-turbo-0301", "user", "Planning a 3-day trip to San Diego", result)) {
    Serial.println("===OK===");
    Serial.println(result);
  } else {
    Serial.println("===ERROR===");
    Serial.println(result);
  }

  Serial.println("\n\n[ChatGPT] Print full message(JSON Type)");
  if (chat_gpt.full_message("gpt-3.5-turbo", "user", "What is the OpenAI mission?", result)) {
    Serial.println("===OK===");
    Serial.println(result);
  } else {
    Serial.println("===ERROR===");
    Serial.println(result);
  }
}

void loop() {}