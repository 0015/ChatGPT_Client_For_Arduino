/*
 * Project ChatGPT Client For Arduino (Arduino Giga R1, Arduino Portenta H7)
 * Description: For HTTPS connection using Arduino WiFiSSLClient
 * Author: Eric Nam
 * Date: 07-17-2024
 */

#include <WiFi.h>
#include <WiFiSSLClient.h>
#include <ArduinoJson.h>
#include <ChatGPT.hpp>
#include "Base64ImageData.h"

static const char *ssid = "<WIFI_SSID>";
static const char *pass = "<WIFI_PW>";
int status = WL_IDLE_STATUS;

/*
  client: Pointer to the HTTP client object used for making API requests.
  api_version: API version to use for communication (e.g., "v1").
  api_key: API key for authentication with the ChatGPT API.
  timeout: Timeout duration for API requests, in milliseconds.
*/

WiFiSSLClient client;
ChatGPT<WiFiSSLClient> chatGPT_Client(&client, "v1", "<OpenAI_API_KEY>", 60000);

void exampleTextQuestion() {
  /*
  model: Model to use for generating the response (e.g., "gpt-4o", "gpt-3.5-turbo").
  role: Role of the message sender (e.g., "user" or "assistant").
  content: Content of the message to send.
  max_tokens: Maximum number of tokens to generate in the response.
  content_only: Flag indicating whether to extract only the content of the response. (e.g., true - answer only, false - full response)
  result: Reference to a String variable to store the result of the API call.
  mem_dynamic: Select whether to use dynamic memory or static memory when configuring the post body (default: dynamic memory)
  */

  String result;
  Serial.println("\n\n[ChatGPT] - Asking a Text Question");
  if (chatGPT_Client.chat_message("gpt-3.5-turbo", "user", "What is the best feature of GPT-4o?", 100, false, result, false)) {
    Serial.print("[ChatGPT] Response: ");
    Serial.println(result);
  } else {
    Serial.print("[ChatGPT] Error: ");
    Serial.println(result);
  }
}

void exampleVisionQuestionBase64() {
  /*
  model: Model to use for generating the response (e.g., "gpt-4o").
  role: Role of the message sender (e.g., "user" or "assistant").
  type: Type of content (e.g., "text").
  text: Text content of the message.
  image_type: Type of the image (e.g., "image_url").
  image_url: URL of the image or Base64 Image Data
  detail: Detail level of the image (e.g., "high", "low", "auto").
  max_tokens: Maximum number of tokens to generate in the response.
  content_only: Flag indicating whether to extract only the content of the response. (e.g., true - answer only, false - full response)
  result: Reference to a String variable to store the result of the API call.
  mem_dynamic: Select whether to use dynamic memory or static memory when configuring the post body (default: dynamic memory)
  */

  String result;
  Serial.println("\n\n[ChatGPT] - Asking a Vision Question");
  if (chatGPT_Client.vision_question("gpt-4o", "user", "text", "What’s in this image?", "image_url", base64ImageData, "auto", 500, true, result)) {
    Serial.print("[ChatGPT] Response: ");
    Serial.println(result);
  } else {
    Serial.print("[ChatGPT] Error: ");
    Serial.println(result);
  }
}

void exampleVisionQuestionWithURL() {
  String result;
  Serial.println("\n\n[ChatGPT] - Asking a Vision Question");
  if (chatGPT_Client.vision_question("gpt-4o", "user", "text", "What’s in this image?", "image_url", "https://samplelib.com/lib/preview/jpeg/sample-city-park-400x300.jpg", "auto", 400, false, result)) {
    Serial.print("[ChatGPT] Response: ");
    Serial.println(result);
  } else {
    Serial.print("[ChatGPT] Error: ");
    Serial.println(result);
  }
}

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true)
      ;
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to WiFi");

  Serial.println("[ChatGPT] - Examples");
  delay(1000);
  exampleTextQuestion(); // Post Body in Static Memory

  delay(1000);
  exampleVisionQuestionWithURL(); // Post Body in Dynamic memory
}

void loop() {}
