//Khai báo thư viện và định nghĩa các hằng số và thông tin cấu hình cho thư viện Blynk.
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6b-sFxKGV"
#define BLYNK_TEMPLATE_NAME "IoT Gr2"
#define BLYNK_AUTH_TOKEN "4OI51YaQ6oEXigz4q99qjwhKHRzWsWxl"

char auth[] = "";//Auth token
char ssid[] = "";//WIFI name
char pass[] = "";//WIFI password

BlynkTimer timer;
int relayState = 0; // Trạng thái relay: 0 - Tắt, 1 - Bật
int automaticMode = 1; // Chế độ tự động: 0 - Thủ công, 1 - Tự động
int sensorValue = 0; // Giá trị độ ẩm đất
// Định nghĩa và đặt tên cho các chân và hằng số trong mạch 
#define sensorPin A0
#define relayPin D3
#define moistureThreshold 30

void setup() {
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Tắt relay ban đầu
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  timer.setInterval(1000L, checkMoistureLevel);
}
BLYNK_WRITE(V1) {
  automaticMode = param.asInt(); // Chuyển đổi giữa chế độ tự động và thủ công
  if (automaticMode == 0) {
    digitalWrite(relayPin, HIGH); // Tắt relay ở chế độ thủ công
  }
}
BLYNK_WRITE(V2) {
  int buttonState = param.asInt();
  if (automaticMode == 0) { // Chỉ hoạt động khi ở chế độ thủ công
    if (buttonState == 1) {
      digitalWrite(relayPin, LOW); 
    } else {
      digitalWrite(relayPin, HIGH); 
    }
  }
}
void checkMoistureLevel() {
  sensorValue = analogRead(sensorPin);
  sensorValue = map(sensorValue, 0, 1023, 0, 100);
  sensorValue = (sensorValue - 100) * -1;
Blynk.virtualWrite(V0, sensorValue); // Gửi giá trị độ ẩm lên ứng dụng Blynk
  if (automaticMode == 1) {
    if (sensorValue > moistureThreshold) {
      digitalWrite(relayPin, LOW); 
    } else {
      digitalWrite(relayPin, HIGH);
    }
  }
}
void loop() {
  Blynk.run();
  timer.run();
}
