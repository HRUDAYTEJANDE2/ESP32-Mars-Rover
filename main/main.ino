#define L1 27
#define L2 26
#define R1 33
#define R2 25
// LEDs used for visualising

void translational(String input);
void rotational(String input);
void stop(String input);


void setup() {
  Serial.begin(115200);  // Start serial communication
  ledcAttach(L1, 5000, 8);
  ledcAttach(L2, 5000, 8);
  ledcAttach(R1, 5000, 8);
  ledcAttach(R2, 5000, 8);
}

void translational(String input) {
  // CHANGE THIS CODE TO ACTUAL MOTOR CODE WHEN INSTALLING MOTOR DRIVERS!!!
  if (input == "left") {
    Serial.println("LEFT");
    ledcWrite(L1, 255);
    ledcWrite(L2, 255);
    ledcWrite(R1, 0);
    ledcWrite(R2, 0);
  } else if (input == "right") {
    Serial.println("RIGHT");
    ledcWrite(L1, 0);
    ledcWrite(L2, 0);
    ledcWrite(R1, 255);
    ledcWrite(R2, 255);
  } else if (input == "forward") {
    Serial.println("FORWARD");
    ledcWrite(L1, 255);
    ledcWrite(R1, 255);
    ledcWrite(L2, 0);
    ledcWrite(R2, 0);
  } else if (input == "backward") {
    Serial.println("BACKWARD");
    ledcWrite(L1, 0);
    ledcWrite(R1, 0);
    ledcWrite(L2, 255);
    ledcWrite(R2, 255);
  }
}

void rotational(String input) {
  if (input == "rleft") {
    Serial.println("ROTATE LEFT");
    ledcWrite(L1, 0);
    ledcWrite(R1, 0);
    ledcWrite(L2, 0);
    ledcWrite(R2, 0);

    while (true) {
      if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        cmd.toLowerCase();

        if (cmd != "rleft") {
          stop("stop");
          break;
        }
      }
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
  } else if (input == "rright") {
    Serial.println("ROTATE RIGHT");
    ledcWrite(L1, 0);
    ledcWrite(R1, 0);
    ledcWrite(L2, 0);
    ledcWrite(R2, 0);

    while (true) {
      if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        cmd.toLowerCase();

        if (cmd != "rleft") {
          stop("stop");
          break;
        }
      }
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
}

void stop(String input) {
  if (input == "stop") {
    Serial.println("STOP");
    ledcWrite(L1, 0);
    ledcWrite(R1, 0);
    ledcWrite(L2, 0);
    ledcWrite(R2, 0);
  }
}

void loop() {
  String input = Serial.readString();
  input.trim();
  if (input) {
      translational(input);
      rotational(input);
      stop(input);
  }
}