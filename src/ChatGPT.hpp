/*
 * Project ChatGPT Client
 * Description: The official method using API Key for communication with ChatGPT
 * Author: Eric Nam
 * Date: 07-17-2024
 */

#ifndef __CHATGPT__
#define __CHATGPT__

template <class T>
class ChatGPT
{
public:
  /*
   * Description:
   * Constructor for the ChatGPT class.
   *
   * Parameters:
   * - _client: Pointer to the HTTP client object.
   * - api_version_in: The API version to use.
   * - api_key_in: The API key for authentication.
   * - timeout_in: The timeout duration for requests.
   */

  ChatGPT(T *_client,
          const char *api_version_in,
          const char *api_key_in,
          unsigned long timeout_in)
      : client(_client), api_version(api_version_in), api_key(api_key_in), timeout(timeout_in)
  {
    timeout = (timeout_in < 5000) ? 5000 : timeout_in;
  }

  ~ChatGPT() = default;

  /*
   * Description:
   * Sends a vision question to the ChatGPT API.
   *
   * Parameters:
   * - model: The model to use for generating the response.
   * - role: The role of the user (e.g., "user", "assistant").
   * - type: The type of the content (e.g., "text", "image_url").
   * - text: The text content of the message.
   * - image_type: The type of the image content.
   * - image_url: The URL of the image.
   * - detail: The detail level of the image (e.g., "low", "high").
   * - max_tokens: The maximum number of tokens to generate.
   * - content_only: If true, extracts and returns only the content from the response.
   * - result: Stores the response from the API.
   * - mem_dynamic: Select Dynamic/Static Memory, (Default: Dyanmic Memory)
   *
   * Returns:
   * - True if the request is successful, false otherwise.
   */
  bool vision_question(
      const char *model,
      const char *role,
      const char *type,
      const char *text,
      const char *image_type,
      const char *image_url,
      const char *detail,
      int max_tokens,
      bool content_only,
      String &result,
      bool mem_dynamic = true)
  {
    char *post_body = nullptr; // Initialize post_body pointer
    int post_body_size = 0;
    if (mem_dynamic)
    {
      // Calculate the required size for dynamic allocation
      post_body_size = snprintf(nullptr, 0,
                                "{\"model\": \"%s\", \"messages\": [{\"role\": \"%s\", \"content\": [{\"type\": \"%s\", \"text\": \"%s\"}, {\"type\": \"%s\", \"image_url\": {\"url\": \"%s\", \"detail\": \"%s\"}}]}], \"max_tokens\": %d}",
                                model, role, type, text, image_type, image_url, detail, max_tokens) + 1;
      post_body = new char[post_body_size];
      if (post_body == nullptr)
      {
        result = "[ERR] Memory allocation failed!";
        return false;
      }
    }
    else
    {
      // Use a static buffer with a fixed size
      static const int static_buffer_size = 512;
      char static_post_body[static_buffer_size];
      post_body_size = static_buffer_size;
      post_body = static_post_body;
    }

    // Format the post_body string
    snprintf(post_body, post_body_size,
             "{\"model\": \"%s\", \"messages\": [{\"role\": \"%s\", \"content\": [{\"type\": \"%s\", \"text\": \"%s\"}, {\"type\": \"%s\", \"image_url\": {\"url\": \"%s\", \"detail\": \"%s\"}}]}], \"max_tokens\": %d}",
             model, role, type, text, image_type, image_url, detail, max_tokens);

    // Call the _post function
    bool success = _postStream(post_body, content_only, result);

    // Free dynamic memory if allocated
    if (mem_dynamic)
    {
      delete[] post_body;
    }
    return success;
  }

  /*
   * Description:
   * Sends a chat message to the ChatGPT API.
   *
   * Parameters:
   * - model: The model to use for generating the response.
   * - role: The role of the user (e.g., "user", "assistant").
   * - content: The content of the message.
   * - max_tokens: The maximum number of tokens to generate.
   * - content_only: If true, extracts and returns only the content from the response.
   * - result: Stores the response from the API.
   * - mem_dynamic: Select Dynamic/Static Memory, (Default: Dyanmic Memory)
   *
   * Returns:
   * - True if the request is successful, false otherwise.
   */
  bool chat_message(
      const char *model,
      const char *role,
      const char *content,
      int max_tokens,
      bool content_only,
      String &result,
      bool mem_dynamic = true)
  {
    char *post_body = nullptr; // Initialize post_body pointer
    int post_body_size = 0;

    if (mem_dynamic)
    {
      // Calculate the required size for dynamic allocation
      post_body_size = snprintf(nullptr, 0, "{\"model\": \"%s\", \"max_tokens\": %d, \"messages\": [{\"role\": \"%s\", \"content\": \"%s\"}]}", model, max_tokens, role, content) + 1;
      post_body = new char[post_body_size];
      if (post_body == nullptr)
      {
        result = "[ERR] Memory allocation failed!";
        return false;
      }
    }
    else
    {
      // Use a static buffer with a fixed size
      static const int static_buffer_size = 256;
      char static_post_body[static_buffer_size];
      post_body_size = static_buffer_size;
      post_body = static_post_body;
    }

    // Format the post_body string
    snprintf(post_body, post_body_size, "{\"model\": \"%s\", \"max_tokens\": %d, \"messages\": [{\"role\": \"%s\", \"content\": \"%s\"}]}", model, max_tokens, role, content);

    // Call the _post function
    bool success = _postStream(post_body, content_only, result);

    // Free dynamic memory if allocated
    if (mem_dynamic)
    {
      delete[] post_body;
    }
    return success;
  }

private:
  static constexpr const char *host = "api.openai.com";
  static constexpr const int httpsPort = 443;

  T *client = nullptr;
  String api_version;
  String api_key;
  unsigned long timeout;

  /*
   * Description:
   * Sends an HTTP POST request to the ChatGPT API in streaming mode.
   *
   * Parameters:
   * - post_body: The body of the POST request.
   * - content_only: If true, extracts and returns only the content from the response.
   * - result: Stores the response from the API.
   *
   * Returns:
   * - True if the request is successful, false otherwise.
   */
  bool _postStream(
      const char *post_body,
      bool content_only,
      String &result)
  {
    if (!client->connect(host, httpsPort))
    {
      result = "[ERR] Connection failed!";
      return false;
    }

    size_t payload_length = strlen(post_body);
    String auth_header = _get_auth_header(api_key);
    String http_request = "POST /" + api_version + "/chat/completions HTTP/1.1\r\n" + auth_header + "\r\n" + "Host: " + host + "\r\n" + "Cache-control: no-cache\r\n" + "User-Agent: ESP32 ChatGPT\r\n" + "Content-Type: application/json\r\n" + "Content-Length: " + String(payload_length) + "\r\n" + "Connection: close\r\n" + "\r\n";

    // Send the HTTP request headers
    client->print(http_request);

    // Send the HTTP request body in chunks
    size_t bytes_sent = 0;
    while (bytes_sent < payload_length)
    {
      size_t chunk_size = minimum(payload_length - bytes_sent, static_cast<size_t>(1024)); // Adjust chunk size as needed
      size_t bytes_written = client->write((const uint8_t *)post_body + bytes_sent, chunk_size);
      if (bytes_written != chunk_size)
      {
        result = "[ERR] Failed to send data";
        client->stop();
        return false;
      }
      bytes_sent += bytes_written;
    }

    // Wait for the response
    String responseStr;
    unsigned long startTime = millis();
    while (client->connected() && millis() - startTime < timeout)
    {
      if (client->available())
      {
        responseStr += client->readStringUntil('\n');
        responseStr += "\r\n";
        startTime = millis(); // Reset the timeout timer whenever new data is received
      }
    }

    // Handle timeout
    if (millis() - startTime >= timeout)
    {
      result = "[ERR] Timeout";
      client->stop();
      return false;
    }

    client->stop();

    // Check for valid response
    if (responseStr.length() == 0)
    {
      result = "[ERR] Empty response";
      return false;
    }

    // Extract response code
    int responseCode = responseStr.substring(responseStr.indexOf(" ") + 1, responseStr.indexOf(" ") + 4).toInt();

    if (responseCode != 200)
    {
      result = responseStr;
      return false;
    }

    // Extract JSON body
    int start = responseStr.indexOf("{");
    int end = responseStr.lastIndexOf("}");
    String jsonBody = responseStr.substring(start, end + 1);

    if (jsonBody.length() == 0)
    {
      result = "[ERR] Couldn't read JSON data";
      return false;
    }

    if (content_only)
    {
      return getContent(jsonBody.c_str(), result);
    }
    else
    {
      result = jsonBody;
      return true;
    }
  }

  String _get_auth_header(const String &api)
  {
    return "Authorization: Bearer " + api;
  }

  bool getContent(const char *jsonString, String &result)
  {
    JsonDocument doc;

    DeserializationError error = deserializeJson(doc, jsonString);
    if (error)
    {
      result = "deserializeJson() failed: ";
      result += error.f_str();
      return false;
    }

    // Check if the "choices" array exists and contains at least one element
    if (doc.containsKey("choices") && doc["choices"].is<JsonArray>() && doc["choices"].size() > 0)
    {
      // Access the first element of the "choices" array
      JsonObject choice = doc["choices"][0];

      // Check if the "message" object exists
      if (choice.containsKey("message"))
      {
        // Access the "message" object
        JsonObject message = choice["message"];

        // Check if the "content" field exists
        if (message.containsKey("content"))
        {
          // Access and store the value of the "content" field
          const char *content = message["content"];
          result = content;
          return true;
        }
        else
        {
          result = "No 'content' field found";
          return false;
        }
      }
      else
      {
        result = "No 'message' object found";
        return false;
      }
    }
    else
    {
      result = "No 'choices' array found or it is empty";
      return false;
    }
  }

  size_t minimum(size_t a, size_t b)
  {
    return (a < b) ? a : b;
  }
};

#endif
