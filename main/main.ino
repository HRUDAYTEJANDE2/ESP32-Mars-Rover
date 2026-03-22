#define L1 27
#define L2 26
#define R1 33
#define R2 25
// LEDs used for visualising
#define DHTPIN 4

#include <DHT.h>
#include <esp_now.h>
#include <WiFi.h>

#define DHTTYPE DHT11
uint8_t broadcastAddress[] = { 0x54, 0x43, 0xB2, 0xC3, 0x3D, 0x40 };

// Data structure
// 1 = left, 2 = right, 3 = forward, 4 = backward, 5 = rleft, 6 = rright, 7 = stop
DHT dht(DHTPIN, DHTTYPE);
typedef struct struct_message {
  int a;
  float h;
  float tc;
  float tf;
  float hic;
  float hif;
  String dhtstatus;
} struct_message;
struct_message cmd;
esp_now_peer_info_t peerInfo;
char *commands[] = { "none", "left", "right", "forward", "backward", "rleft", "rright", "stop" };

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&cmd, incomingData, sizeof(cmd));
  Serial.println("Command: ");
  Serial.println(cmd.a);
  Serial.println(commands[cmd.a]);
}

void setup() {
  // RECIEVING DATA FROM THE BASE STATION
  Serial.begin(115200);
  Serial.println(F("DHTxx test!"));
  dht.begin();
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Resetting the LEDs at start
  ledcAttach(L1, 5000, 8);
  ledcAttach(L2, 5000, 8);
  ledcAttach(R1, 5000, 8);
  ledcAttach(R2, 5000, 8);
}

void stop() {
  ledcWrite(L1, 0);
  ledcWrite(R1, 0);
  ledcWrite(L2, 0);
  ledcWrite(R2, 0);
}

void output(int input) {
  // CHANGE THIS CODE TO ACTUAL MOTOR CODE WHEN INSTALLING
  // THIS IS LED CODE ONLY
  if (input) {
    if (input == 1) {
      ledcWrite(L1, 255);
      ledcWrite(L2, 255);
      ledcWrite(R1, 0);
      ledcWrite(R2, 0);
    } else if (input == 2) {
      ledcWrite(L1, 0);
      ledcWrite(L2, 0);
      ledcWrite(R1, 255);
      ledcWrite(R2, 255);
    } else if (input == 3) {
      ledcWrite(L1, 255);
      ledcWrite(R1, 255);
      ledcWrite(L2, 0);
      ledcWrite(R2, 0);
    } else if (input == 4) {
      ledcWrite(L1, 0);
      ledcWrite(R1, 0);
      ledcWrite(L2, 255);
      ledcWrite(R2, 255);
    } else if (input == 7) {
      stop();
    }
  }
}

void rleft() {
  ledcWrite(L1, 0);
  ledcWrite(R1, 0);
  ledcWrite(L2, 0);
  ledcWrite(R2, 0);

  for (int i = 0; i < 127; i++) {
    ledcWrite(L1, i);
    ledcWrite(R2, i);
    delay(2);
  }
  for (int i = 127; i < 255; i++) {
    ledcWrite(L1, i);
    ledcWrite(R2, i);
    ledcWrite(L2, i - 127);
    ledcWrite(R1, i - 127);
    delay(2);
  }
  for (int i = 127; i < 255; i++) {
    ledcWrite(L2, i);
    ledcWrite(R1, i);
    delay(2);
  }
  for (int i = 255; i > 127; i--) {
    ledcWrite(L1, i);
    ledcWrite(R2, i);
    delay(2);
  }
  for (int i = 127; i > 0; i--) {
    ledcWrite(L1, i);
    ledcWrite(R2, i);
    ledcWrite(L2, i + 128);
    ledcWrite(R1, i - 128);
    delay(2);
  }
  for (int i = 127; i > 0; i--) {
    ledcWrite(L2, i);
    ledcWrite(R1, i);
    delay(2);
  }
}

void rright() {
  ledcWrite(L1, 0);
  ledcWrite(R1, 0);
  ledcWrite(L2, 0);
  ledcWrite(R2, 0);

  for (int i = 0; i < 127; i++) {
    ledcWrite(L2, i);
    ledcWrite(R1, i);
    delay(2);
  }
  for (int i = 127; i < 255; i++) {
    ledcWrite(L2, i);
    ledcWrite(R1, i);
    ledcWrite(L1, i - 127);
    ledcWrite(R2, i - 127);
    delay(2);
  }
  for (int i = 127; i < 255; i++) {
    ledcWrite(L1, i);
    ledcWrite(R2, i);
    delay(2);
  }
  for (int i = 255; i > 127; i--) {
    ledcWrite(L2, i);
    ledcWrite(R1, i);
    delay(2);
  }
  for (int i = 127; i > 0; i--) {
    ledcWrite(L2, i);
    ledcWrite(R1, i);
    ledcWrite(L1, i + 128);
    ledcWrite(R2, i - 128);
    delay(2);
  }
  for (int i = 127; i > 0; i--) {
    ledcWrite(L1, i);
    ledcWrite(R2, i);
    delay(2);
  }
}

void getDHTdata() {
  float h = dht.readHumidity();
  float tc = dht.readTemperature();
  float tf = dht.readTemperature(true);
  float hif = dht.computeHeatIndex(tf, h);
  float hic = dht.computeHeatIndex(tc, h, false);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(tc) || isnan(tf)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  cmd.h = h;
  cmd.tc = tc;
  cmd.tf = tf;
  cmd.hif = hif;
  cmd.hic = hic;
}

void loop() {
  int input = cmd.a;
  if (cmd.a == 1) output(1);
  else if (cmd.a == 2) output(2);
  else if (cmd.a == 3) output(3);
  else if (cmd.a == 4) output(4);
  else if (cmd.a == 5) rleft();
  else if (cmd.a == 6) rright();
  else if (cmd.a == 7) stop();

  static unsigned long lastRead = 0;

  if (millis() - lastRead > 2000) {
    lastRead = millis();
    getDHTdata();
  }
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&cmd, sizeof(cmd));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
    Serial.println(cmd.a);
    Serial.println(cmd.tc);
    Serial.println(cmd.tf);
    Serial.println(cmd.hif);
    Serial.println(cmd.hic);
  } else {
    Serial.println("Error sending the cmd");
  }
  delay(200);
}
