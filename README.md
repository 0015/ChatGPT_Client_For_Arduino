[![arduino-library-badge](https://www.ardu-badge.com/badge/ChatGPT_Client.svg)](https://www.ardu-badge.com/ChatGPT_Client)

## Overview
The ChatGPT Arduino Library provides a convenient way to interact with the OpenAI GPT models from Arduino environments, such as ESP32 devices. With this library, you can easily send text and vision queries to the ChatGPT API and receive responses directly in your Arduino projects.

## Features
- **Text Generation**: Generate text-based responses to user queries using the ChatGPT models.
- **Vision Question Answering**: Utilize the vision capabilities of ChatGPT to answer questions about images.
- **Simple API Integration**: Simple and intuitive API for seamless integration into your Arduino projects.

## Authentication
The OpenAI API uses API keys for authentication. Visit your API Keys page to retrieve the API key you'll use in your requests.

First, get your SECRET KEY issued here.
https://platform.openai.com/api-keys
![API Keys](misc/openai_API-Keys.png)

## Installation

### Using Arduino Library Manager

From Arduino IDE, go to menu *Sketch -> Include Library -> Manage Libraries...*
In Library Manager Window, search *"ChatGPT"* in the search form then select *"ChatGPT_Client"*
Click *"Install"* button.

### Manual installation

Download zip file from this repository by selecting the green *"Code"* dropdown at the top of repository, select *"Download ZIP"*
From Arduino IDE, select menu *Sketch -> Include Library -> Add .ZIP Library...*

Or use git:
 ```
 cd ~/arduino/libraries/
 gh repo clone 0015/ChatGPT_Client_For_Arduino
 ```

Then you should see the examples and be able to include the library in your projects with:

```
 #include <ChatGPT.hpp>
```

## Usage
1. Include the `ChatGPT.hpp` header file in your Arduino sketch.
2. Create a `ChatGPT` object and provide your API key and other required parameters.
3. Use the provided methods to send queries to the ChatGPT API and receive responses.

### Example:
```cpp
#include <ChatGPT.hpp>

// Initialize ChatGPT client
WiFiSSLClient wifiClient;
ChatGPT<WiFiSSLClient> chatGPT(&wifiClient, "v1", "YOUR_API_KEY", 10000);

void exampleTextQuestion() {
  /*
  model: Model to use for generating the response (e.g., "gpt-4o", "gpt-3.5-turbo").
  role: Role of the message sender (e.g., "user" or "assistant").
  content: Content of the message to send.
  max_tokens: Maximum number of tokens to generate in the response.
  content_only: Flag indicating whether to extract only the content of the response. (e.g., true - answer only, false - full response)
  result: Reference to a String variable to store the result of the API call.
  */

  String result;
  Serial.println("\n\n[ChatGPT] - Asking a Text Question");
  if (chatGPT_Client.chat_message("gpt-3.5-turbo", "user", "What is the best feature of GPT-4o?", 100, false, result)) {
    Serial.print("[ChatGPT] Response: ");
    Serial.println(result);
  } else {
    Serial.print("[ChatGPT] Error: ");
    Serial.println(result);
  }
}

void exampleVisionQuestionWithURL() {
  /*
  model: Model to use for generating the response (e.g., "gpt-4o").
  role: Role of the message sender (e.g., "user" or "assistant").
  type: Type of content (e.g., "text").
  text: Text content of the message.
  image_type: Type of the image (e.g., "image_url").
  image_url: URL of the image.
  detail: Detail level of the image (e.g., "high", "low", "auto").
  max_tokens: Maximum number of tokens to generate in the response.
  content_only: Flag indicating whether to extract only the content of the response. (e.g., true - answer only, false - full response)
  result: Reference to a String variable to store the result of the API call.
  */
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
```

### Result:

```json
[ChatGPT] - Asking a Text Question
[ChatGPT] Response: {
  "id": "chatcmpl-9QjdlCkqkB4dsVXBzUTRTBYafI7f3",
  "object": "chat.completion",
  "created": 1716158997,
  "model": "gpt-3.5-turbo-0125",
  "choices": [
    {
      "index": 0,
      "message": {
        "role": "assistant",
        "content": "One of the best features of GPT-4o is its advanced natural language processing capabilities. GPT-4o has a deeper understanding of context, nuance, and complex language structures, allowing it to generate more accurate and coherent responses to a wide range of prompts. Its improved language modeling capabilities make it more versatile and effective in various tasks such as text generation, language translation, and question answering. Overall, the enhanced natural language processing capabilities of GPT-4o make it a powerful tool"
      },
      "logprobs": null,
      "finish_reason": "length"
    }
  ],
  "usage": {
    "prompt_tokens": 19,
    "completion_tokens": 100,
    "total_tokens": 119
  },
  "system_fingerprint": null
}
```
```json
[ChatGPT] - Asking a Vision Question
[ChatGPT] Response: {
  "id": "chatcmpl-9Qjdp5B4wyE5cVbcLOucvm9uoT6rH",
  "object": "chat.completion",
  "created": 1716159001,
  "model": "gpt-4o-2024-05-13",
  "choices": [
    {
      "index": 0,
      "message": {
        "role": "assistant",
        "content": "The image shows a peaceful lakeside pathway. The path is made of bricks and runs alongside a body of water. There is greenery on both sides of the path with trees providing shade on the right side. Off in the distance, you can see some buildings, possibly residential apartments, and more greenery. The atmosphere appears to be calm and serene."
      },
      "logprobs": null,
      "finish_reason": "stop"
    }
  ],
  "usage": {
    "prompt_tokens": 268,
    "completion_tokens": 69,
    "total_tokens": 337
  },
  "system_fingerprint": "fp_927397958d"
}
```

### Limitation

AVR and ESP8266 may not operate smoothly due to insufficient memory allocation. It looks like this part will need to be updated again soon.

# Updates 

 - v0.2.0 
    - Support Vision Question (GPT-4o)
    - Support larger payloads
    - ArduinoJson 7

 - v0.1.2
    - Demo Bugs Fixed

 - v0.1.1
    - Added ESP8266 Example

 - v0.1.0
    - Only Support Chat completion yet
    - Model, only gpt-3.5-turbo and gpt-3.5-turbo-0301 are supported

# License

 This software is written by Eric Nam and is licensed under The MIT License. Check License file for more information.    