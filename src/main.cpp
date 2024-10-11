#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <WiFiManager.h>
#include <BearSSLHelpers.h> // Бібліотека для роботи з WiFiClientSecure

WiFiClientSecure client; // WiFi-клієнт для HTTPS-запитів

// Статична IP-адреса, шлюз і маска підмережі
IPAddress staticIP(192, 168, 68, 68);   // Фіксована IP-адреса ESP8266
IPAddress gateway(192, 168, 68, 1);     // IP-адреса маршрутизатора (шлюз)
IPAddress subnet(255, 255, 255, 0);     // Маска підмережі
IPAddress primaryDNS(8, 8, 8, 8);       // Google DNS
IPAddress secondaryDNS(8, 8, 4, 4);     // Альтернативний Google DNS

// Пряме посилання для завантаження файлу з GitHub
const char* firmware_url = "https://github.com/stigmat4j/maestro-programmer/releases/download/untagged-c6f6b30c800ec7b513c2/firmware.bin"; 
const String currentVersion = "0.1";  // Поточна версія прошивки

void checkWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi не підключено! Спробуйте знову.");
    return;
  }

  Serial.println("WiFi підключено:");
  Serial.print("IP-адреса: ");
  Serial.println(WiFi.localIP());
  Serial.print("Шлюз: ");
  Serial.println(WiFi.gatewayIP());

  HTTPClient http;
  http.begin(client, "http://example.com");
  int httpCode = http.GET();

  if (httpCode > 0) {
    Serial.printf("З'єднання з example.com успішне, код відповіді: %d\n", httpCode);
  } else {
    Serial.printf("З'єднання з example.com не вдалося, помилка: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

void checkForUpdates() {
  Serial.println("Перевірка наявності оновлень...");
  Serial.println(firmware_url);

  client.setInsecure(); // Відключаємо перевірку сертифікатів, що може допомогти з HTTPS

  // Оновлення по HTTPS
  t_httpUpdate_return ret = ESPhttpUpdate.update(client, firmware_url);

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

  // Налаштування статичної IP-адреси і DNS-серверів
  if (!WiFi.config(staticIP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("Помилка налаштування статичної IP або DNS!");
  }

  WiFiManager wifiManager;

  if (!wifiManager.autoConnect("TerikonAP")) {
    Serial.println("Не вдалось підключитись до WiFi. Перезавантаження...");
    delay(3000);
    ESP.restart();
  }

  Serial.println("WiFi підключено з фіксованою IP-адресою 192.168.68.68");

  checkWiFiConnection();
  checkForUpdates();
}

void loop() {
  // Основний цикл програми
}
