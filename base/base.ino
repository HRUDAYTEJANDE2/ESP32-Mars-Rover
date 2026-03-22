#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = { 0x94, 0xE6, 0x86, 0x10, 0xE5, 0xB8 };
// Structure example to send cmd
// Must match the receiver structure
typedef struct struct_message {
  int a;
  float h;
  float tc;
  float tf;
  float hic;
  float hif;
} struct_message;

struct_message cmd;

esp_now_peer_info_t peerInfo;

// callback when cmd is sent
void OncmdSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&cmd, incomingData, sizeof(cmd));
  Serial.println("Command: ");
  Serial.println(cmd.a);
  Serial.println("INCOMING TEMP READINGS");
  Serial.print("Temperature: ");
  Serial.print(cmd.tc);
  Serial.println(" ºC");
  Serial.print(cmd.tf);
  Serial.println(" ºF");
  Serial.print("Humidity: ");
  Serial.print(cmd.h);
  Serial.println(" %");
  Serial.print(F("°F  Heat index: "));
  Serial.print(cmd.hic);
  Serial.print(F("°C "));
  Serial.print(cmd.hif);
  Serial.println(F("°F"));
}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Transmitted packet
  esp_now_register_send_cb(OncmdSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop() {
  // Set values to send
  String string = Serial.readStringUntil('\n');
  string.trim();
  string.toLowerCase();
  if (string == "left") {
    cmd.a = 1;
  } else if (string == "right") {
    cmd.a = 2;
  } else if (string == "forward") {
    cmd.a = 3;
  } else if (string == "backward") {
    cmd.a = 4;
  } else if (string == "rleft") {
    cmd.a = 5;
  } else if (string == "rright") {
    cmd.a = 6;
  } else if (string == "stop") {
    cmd.a = 7;
  }

  if (cmd.a) {
    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&cmd, sizeof(cmd));

    if (result == ESP_OK) {
      Serial.println("Sent with success");
      Serial.println(cmd.a);
    } else {
      Serial.println("Error sending the cmd");
    }
    delay(200);
    
  }
}
