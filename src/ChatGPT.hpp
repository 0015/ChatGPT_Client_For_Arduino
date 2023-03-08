/*
 * Project ChatGPT Client
 * Description: The official method using API Key for communication with ChatGPT
 * Author: Eric Nam
 * Date: 03-08-2023
 */

#ifndef __CHATGPT__
#define __CHATGPT__

template<class T>
class ChatGPT {
public:
  ChatGPT() {}
  ChatGPT(T* _client,
          const char* api_version_in,
          const char* api_key_in)
    : client(_client), api_version(api_version_in), api_key(api_key_in) {
  }

  ~ChatGPT() = default;

  bool simple_message(
    const String& model,
    const String& role,
    const String& content,
    String& result) {

    if (_send_message(model, role, content, result)) {
      DynamicJsonDocument doc(result.length() + 200);
      DeserializationError error = deserializeJson(doc, result.c_str());
      if (error) {
        result = "[ERR] deserializeJson() failed: " + String(error.f_str());
        return false;
      }
      const char* _content = doc["choices"][0]["message"]["content"];
      result = String(_content);
      return true;
    }
    return false;
  }

  bool full_message(const String& model,
                    const String& role,
                    const String& content,
                    String& result) {
    return _send_message(model, role, content, result);
  }

private:
  static constexpr const char* host = "api.openai.com";
  static constexpr const int httpsPort = 443;
  T* client = NULL;
  String api_version;
  String api_key;
  bool _send_message(
    const String& model,
    const String& role,
    const String& content,
    String& result) {

    if (!client->connect(host, httpsPort)) {
      result = "[ERR] Connection failed!";
      return false;
    }

    String post_body = "{\"model\": \"";
    post_body += model;
    post_body += "\", \"messages\": [{\"role\": \"";
    post_body += role;
    post_body += "\", \"content\": \"";
    post_body += content;
    post_body += "\"}]}";

    String auth_header = _get_auth_header(api_key);
    String http_request = "POST /" + api_version + "/chat/completions HTTP/1.1\r\n" + auth_header + "\r\n" + "Host: " + host + "\r\n" + "Cache-control: no-cache\r\n" + "User-Agent: ESP32 ChatGPT\r\n" + "Content-Type: application/json\r\n" + "Content-Length: " + post_body.length() + "\r\n" + "Connection: close\r\n" + "\r\n" + post_body + "\r\n";

    client->println(http_request);

    String responseStr;

    while (client->connected()) {
      if (client->available()) {
        responseStr += client->readStringUntil('\n');
        responseStr += String("\r\n");
      }
    }
    client->stop();

    int responseCode = 0;
    if (responseStr.indexOf(" ") != -1) {                                                                        // If the first space is found
      responseCode = responseStr.substring(responseStr.indexOf(" ") + 1, responseStr.indexOf(" ") + 4).toInt();  // Get the characters following the first space and convert to integer
    }

    if (responseCode != 200) {
      result = responseStr;
      return false;
    } else {

      int start = responseStr.indexOf("{");
      int end = responseStr.lastIndexOf("}");
      String jsonBody = responseStr.substring(start, end + 1);

      if (jsonBody.length() > 0) {
        result = jsonBody;
        return true;
      }

      result = "[ERR] Couldn't read data";
      return false;
    }
  }


  String _get_auth_header(
    const String& api) {
    return "Authorization: Bearer " + api;
  }
};
#endif
