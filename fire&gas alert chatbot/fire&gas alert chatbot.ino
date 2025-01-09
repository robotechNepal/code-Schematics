#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "Aneep";
const char* password = "Anip5656";

// Initialize Telegram BOT
#define BOTtoken "7668012899:AAGNVp75-McWget7w4_r0cF8mMepK2_rCMo"  // your Bot Token (Get from Botfather)
#define CHAT_ID "6410055583"    // get from ID Bot

const int flamesensor = 27;
const int gasSensor = 32; // Pin connected to the MQ5 gas sensor
bool flameDetected = false;
bool gasLeakDetected = false; 

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

void IRAM_ATTR detectsFlame() {
  flameDetected = true;
}

void setup() {
  Serial.begin(115200);

  pinMode(flamesensor, INPUT_PULLUP);
  pinMode(gasSensor, INPUT); // Set the gas sensor pin as input

  attachInterrupt(digitalPinToInterrupt(flamesensor), detectsFlame, RISING);

  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  bot.sendMessage(CHAT_ID, "Device Activated and initiating the process.", "");
}

void loop() {
  if (flameDetected) {
    bot.sendMessage(CHAT_ID, "Fire Alert!!!", "");
    Serial.println("Fire Alert!!!");
    flameDetected = false;
  }

  // Check for gas leakage
  if (digitalRead(gasSensor) == HIGH) { // Assuming HIGH indicates gas leakage
    if (!gasLeakDetected) {
      bot.sendMessage(CHAT_ID, "Gas Leakage Detected!!!", "");
      Serial.println("Gas Leakage Detected!!!");
      gasLeakDetected = true;
    }
  } else {
    gasLeakDetected = false;
  }
}