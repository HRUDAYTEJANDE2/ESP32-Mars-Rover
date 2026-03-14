#define L1 27
#define L2 26
#define R1 33
#define R2 25
// LEDs used for visualising

#include <esp_now.h>
#include <WiFi.h>

// Data structure
// 1 = left, 2 = right, 3 = forward, 4 = backward, 5 = rleft, 6 = rright, 7 = stop
typedef struct struct_message {
  int a;
} struct_message;
struct_message cmd;

char *commands[] = { "none", "left", "right", "forward", "backward", "rleft", "rright", "stop" };

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&cmd, incomingData, sizeof(cmd));
  Serial.println("Command: ");
  Serial.println(cmd.a);
  Serial.println(commands[cmd.a]);
}

void setup() {

  // RECIEVING DATA FROM THE BASE STATION

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

  // Resetting the LEDs at start

  Serial.begin(115200);
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

  while (cmd.a == 5) {
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
}

void rright() {
  ledcWrite(L1, 0);
  ledcWrite(R1, 0);
  ledcWrite(L2, 0);
  ledcWrite(R2, 0);

  while (cmd.a == 6) {
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
}
