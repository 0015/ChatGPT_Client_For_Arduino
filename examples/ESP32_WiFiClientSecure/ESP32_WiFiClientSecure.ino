/*
 * Project ChatGPT Client For ESP32
 * Description: For HTTPS connection using WiFiClientSecure
 * Author: Eric Nam
 * Date: 04-02-2023
 */

#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <ChatGPT.hpp>

static const char *ssid = "<WIFI_SSID>";
static const char *password = "<WIFI_PW>";

WiFiClientSecure client;
ChatGPT<WiFiClientSecure> chat_gpt(&client, "v1", "<OpenAI_API_KEY>");

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to WiFi network: ");
  Serial.print(ssid);
  Serial.println("'...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting...");
    delay(500);
  }
  Serial.println("Connected!");

  // Ignore SSL certificate validation
  client.setInsecure();

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
