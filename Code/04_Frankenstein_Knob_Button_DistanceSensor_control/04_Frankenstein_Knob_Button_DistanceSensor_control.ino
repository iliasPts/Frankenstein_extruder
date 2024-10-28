/// Controlled flow extruder ///
/// Speed is regulated by potentiometer (knob) ///
/// The extrusion starts only when the first button is pressed and the sensor is at the correct distance (below the defined value) ///
/// Avoid Serial.Print type commands in the void loop to take full advantage of the motor speed ///

const int StepsPerRev = 400;
const int Knob = A0;
const int ButtonA = 9;
const int LedA = 8;
const int LedB = 10;
const int Echo = 3;
const int Trigger = 2;
const int ExtrusionDistance = 15;

int ButtonAState = LOW;
bool programOn = false;

long duration;
int distance;

#include <Stepper.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(23, 22, 25, 24, 27, 26);
Stepper stepper_NEMA17(StepsPerRev, 50, 51, 52, 53);

void setup() {
  pinMode(ButtonA, INPUT);
  pinMode(LedA, OUTPUT);
  pinMode(LedB, OUTPUT);
  pinMode(Trigger, OUTPUT); 
  pinMode(Echo, INPUT);
  Serial.begin(9600);
  Serial.println("Controlled flow extruder");
  Serial.println("Speed is regulated by potentiometer (knob)");
  Serial.println("The extrusion starts only when the first button is pressed and the sensor is at the correct distance (below the defined value)");
  Serial.println("Avoid Serial.Print type commands in the void loop to take full advantage of the motor speed");
  lcd.begin(8, 2);
  lcd.print("E.A.D.C. BUTTON");
  lcd.setCursor(0, 1);
  lcd.print("+DIST +KNOB");
  delay(2000);
  lcd.clear();
  lcd.print("DIST:");
  lcd.setCursor(10, 0);
  lcd.print(StepsPerRev);
  lcd.print("REV");
  lcd.setCursor(0, 1);
  lcd.print("STANDBY  SPD:");
}

void loop() {
  int sensorReading = analogRead(Knob);
  int motorSpeed = map(sensorReading, 0, 1023, 0, 99);

  digitalWrite(Trigger, LOW);
  delayMicroseconds(1);
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);
  duration = pulseIn(Echo, HIGH);
  distance = duration * 0.034 / 2;

  lcd.setCursor(5, 0);
  lcd.print("   ");
  lcd.setCursor(5, 0);
  lcd.print(distance);
  lcd.setCursor(14, 1);
  lcd.print(" ");
  lcd.setCursor(13, 1);
  lcd.print(motorSpeed);
  lcd.setCursor(15, 1);
  lcd.print("%");

  int ButtonAReading = digitalRead(ButtonA);
  if (ButtonAReading != ButtonAState) {
    ButtonAState = ButtonAReading;
    if (ButtonAState == HIGH) {
      programOn = !programOn;
      if (programOn) {
        lcd.setCursor(0, 1);
        lcd.print("READY2PR");
        digitalWrite(LedA, HIGH);
      } else {
        lcd.setCursor(0, 1);
        lcd.print("STANDBY ");
        digitalWrite(LedA, LOW);
      }
    }
  }

  if (programOn) {
    if (distance < ExtrusionDistance && distance > 0) {
      digitalWrite(LedB, HIGH);
      stepper_NEMA17.setSpeed(motorSpeed);
      stepper_NEMA17.step(StepsPerRev / 100);
      digitalWrite(LedB, LOW);
    }
  }
}
