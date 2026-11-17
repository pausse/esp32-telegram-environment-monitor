#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <time.h>
#include "DHT.h"
#include "secrets.h"

#define DHTPIN 4
#define DHTTYPE DHT22

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
DHT dht(DHTPIN, DHTTYPE);

unsigned long lastTimeBotRan = 0;
const unsigned long botRequestDelay = 1000;

String getStatusText(float temperature, float humidity) {
  if (temperature > 28) {
    return "🔥 High temperature detected";
  } else if (humidity > 70) {
    return "💦 High humidity detected";
  }
  return "✅ Normal conditions";
}

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chatId = bot.messages[i].chat_id;
    String command = bot.messages[i].text;

    if (chatId != CHAT_ID) {
      bot.sendMessage(chatId, "⛔ Unauthorized user", "");
      continue;
    }

    if (command == "/start") {
      String welcome = "👋 *Welcome to your ESP32 Bot*\n\n";
      welcome += "📊 /status - Get current sensor values\n";
      welcome += "🏠 /start - Show this menu\n";
      bot.sendMessage(chatId, welcome, "Markdown");
    } else if (command == "/status") {
      float humidity = dht.readHumidity();
      float temperature = dht.readTemperature();

      if (isnan(humidity) || isnan(temperature)) {
        bot.sendMessage(chatId, "⚠️ Error reading sensor.", "");
        continue;
      }

      String message = "🤖 *ESP32 Environmental Monitor*\n\n";
      message += "🌡️ *Temperature:* " + String(temperature, 1) + " °C\n";
      message += "💧 *Humidity:* " + String(humidity, 1) + " %\n";
      message += "📌 *Status:* " + getStatusText(temperature, humidity);
      bot.sendMessage(chatId, message, "Markdown");
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi connected");
  Serial.println(WiFi.localIP());

  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  while (time(nullptr) < 24 * 3600) {
    delay(100);
  }

  bot.sendMessage(CHAT_ID, "🚀 *ESP32 Telegram bot is online*", "Markdown");
}

void loop() {
  if (millis() - lastTimeBotRan >= botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages > 0) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    lastTimeBotRan = millis();
  }
}
