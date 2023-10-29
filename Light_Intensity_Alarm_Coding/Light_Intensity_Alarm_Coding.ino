#include <Keypad.h>
#define SIZE 4

const byte LIGHT_PIN = A0;
const byte BUTTON_PIN = 2;
const byte LASER_PIN = 3;
const byte BUZZER_PIN = 13;
char TEXT[SIZE][SIZE] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
const byte COLS[SIZE] = {8, 7, 6, 5};
const byte ROWS[SIZE] = {12, 11, 10, 9};
char pass[SIZE] = {'A', '6', '8', '#'};
bool toggle = false;
bool detect = false;
short checkPass = 0;
Keypad myKey = Keypad(makeKeymap(TEXT), ROWS, COLS, SIZE, SIZE);

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LASER_PIN, OUTPUT);
  noTone(BUZZER_PIN);
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(BUTTON_PIN) == HIGH && !toggle) {
    toggle = !toggle;
    Serial.println("Switch On");
    digitalWrite(LASER_PIN, HIGH);
  } else if (digitalRead(BUTTON_PIN) == HIGH && toggle) {
    toggle = !toggle;
    digitalWrite(LASER_PIN, LOW);
    Serial.println("Switch Off");
  }
  delay(150);
  Serial.println(analogRead(LIGHT_PIN));
  if (analogRead(LIGHT_PIN) > 550) {
    detect = true;
  }
  if (detect) {
    tone(BUZZER_PIN, 2000);
    delay(50);
    tone(BUZZER_PIN, 1000);
    delay(50);
    if (myKey.getKey() == '*') {
      tone(BUZZER_PIN, 500);
      for (short i = 0; i < SIZE; i++) {
        tone(BUZZER_PIN, 500);
        if (myKey.waitForKey() == pass[i]) {
          checkPass++;
          tone(BUZZER_PIN, 700);
        } else {
          checkPass = 0;
          tone(BUZZER_PIN, 2000);
          i = 0;
        }
      }
      if (checkPass == SIZE) {
        noTone(BUZZER_PIN);
        checkPass = 0;
        detect = false;
      }
    }
  }
}
