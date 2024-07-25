#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "YOUR SSID";
const char* password = "12345678";

// Initialize Telegram BOT
#define BOTtoken "Your telegram bot code"  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "your chat id"

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const int ledPin = 2;       // GPIO pin for the LED
const int motorPin = 4;     // GPIO pin for the motor relay
const int fanPin = 5;       // GPIO pin for the fan relay

bool ledState = LOW;
bool motorState = LOW;
bool fanState = LOW;

// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use the following commands to control your outputs.\n\n";
      welcome += "/led_on to turn LED ON \n";
      welcome += "/led_off to turn LED OFF \n";
      welcome += "/motor_on to turn Motor ON \n";
      welcome += "/motor_off to turn Motor OFF \n";
      welcome += "/fan_on to turn Fan ON \n";
      welcome += "/fan_off to turn Fan OFF \n";
      welcome += "/state to request current state of all outputs \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/led_on") {
      bot.sendMessage(chat_id, "LED state set to ON", "");
      ledState = HIGH;
      digitalWrite(ledPin, ledState);
    }
    
    if (text == "/led_off") {
      bot.sendMessage(chat_id, "LED state set to OFF", "");
      ledState = LOW;
      digitalWrite(ledPin, ledState);
    }
    
    if (text == "/motor_on") {
      bot.sendMessage(chat_id, "Motor state set to ON", "");
      motorState = HIGH;
      digitalWrite(motorPin, motorState);
    }
    
    if (text == "/motor_off") {
      bot.sendMessage(chat_id, "Motor state set to OFF", "");
      motorState = LOW;
      digitalWrite(motorPin, motorState);
    }
    
    if (text == "/fan_on") {
      bot.sendMessage(chat_id, "Fan state set to ON", "");
      fanState = HIGH;
      digitalWrite(fanPin, fanState);
    }
    
    if (text == "/fan_off") {
      bot.sendMessage(chat_id, "Fan state set to OFF", "");
      fanState = LOW;
      digitalWrite(fanPin, fanState);
    }
    
    if (text == "/state") {
      String stateMsg = "Current states:\n";
      stateMsg += "LED is " + String(ledState ? "ON" : "OFF") + "\n";
      stateMsg += "Motor is " + String(motorState ? "ON" : "OFF") + "\n";
      stateMsg += "Fan is " + String(fanState ? "ON" : "OFF") + "\n";
      bot.sendMessage(chat_id, stateMsg, "");
    }
  }
}

void setup() {
  Serial.begin(115200);

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
    client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  #endif

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);

  pinMode(motorPin, OUTPUT);
  digitalWrite(motorPin, motorState);

  pinMode(fanPin, OUTPUT);
  digitalWrite(fanPin, fanState);
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
