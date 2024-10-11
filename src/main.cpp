#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <WiFiManager.h>   // Бібліотека для управління підключенням до Wi-Fi

WiFiClientSecure client; // WiFi-клієнт для HTTPS-запитів

// Статична IP-адреса, шлюз і маска підмережі
IPAddress staticIP(192, 168, 68, 68);   // Фіксована IP-адреса ESP8266
IPAddress gateway(192, 168, 68, 1);     // IP-адреса маршрутизатора (шлюз)
IPAddress subnet(255, 255, 255, 0);     // Маска підмережі

const char* host = "raw.githubusercontent.com";        // Хост GitHub
const uint16_t port = 443;                             // Порт для HTTPS
const char* uri = "/stigmat4j/maestro-programmer/main/.pio/build/esp12e/firmware.bin"; // Шлях до файлу прошивки
const String currentVersion = "0.1";                   // Поточна версія прошивки


void checkForUpdates() {
  Serial.println("Перевірка наявності оновлень...");

  // Додавання сертифікатів (можна використовувати без перевірки сертифікату для спрощення тестування)
  client.setInsecure(); // Відключає перевірку сертифікату для HTTPS-з'єднання

  // Оновлення по HTTPS
  t_httpUpdate_return ret = ESPhttpUpdate.update(client, host, port, uri, currentVersion);

  switch (ret) {
    case HTTP_UPDATE_FAILED:
      Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", 
                    ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
      break;

    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("Немає нових оновлень");
      break;

    case HTTP_UPDATE_OK:
      Serial.println("Оновлення успішно встановлено!");
      break;
  }
}

void setup() {
  Serial.begin(115200);

  // Налаштування статичної IP-адреси
  WiFi.config(staticIP, gateway, subnet);

  // Перевірка збереженої мережі
  WiFiManager wifiManager;
  
  // Якщо немає збереженої мережі, відкриваємо точку доступу
  if (!wifiManager.autoConnect("TerikonAP")) {
    Serial.println("Не вдалось підключитись до WiFi. Перезавантаження...");
    delay(3000);
    ESP.restart();
  }

  // Підключення до WiFi
  Serial.println("WiFi підключено з фіксованою IP-адресою 192.168.68.68");

  // Перевірка оновлень прошивки
  checkForUpdates();
}

void loop() {
  // Основний цикл програми
}
