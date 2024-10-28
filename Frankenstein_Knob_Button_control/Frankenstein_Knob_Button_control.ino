/// Controlled flow extruder ///
/// Speed is regulated by potentiometer (knob) ///
/// The extrusion starts only when the first button is pressed, which starts the motor rotation ///
/// Avoid Serial.Print type commands in the void loop to take full advantage of the motor speed ///

const int StepsPerRev = 350;
const int Knob = A0;
const int ButtonA = 9;
const int LedA = 8;

int ButtonAState = LOW;
bool programOn = false;

#include <Stepper.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(23, 22, 25, 24, 27, 26);
Stepper stepper_NEMA17(StepsPerRev, 50, 51, 52, 53);

void setup() {
  pinMode(ButtonA, INPUT);
  pinMode(LedA, OUTPUT);
  Serial.begin(9600);
  Serial.println("Controlled flow extruder");
  Serial.println("Speed is regulated by potentiometer (knob)");
  Serial.println("The extrusion starts only when the first button is pressed, which starts the motor rotation");
  Serial.println("Avoid Serial.Print type commands in the void loop to take full advantage of the motor speed");
  lcd.begin(8, 2);
  lcd.print("E.A.D.C. BUTTON");
  lcd.setCursor(0, 1);
  lcd.print("+KNOB");
  delay(2000);
  lcd.clear();
  lcd.setCursor(10, 0);
  lcd.print(StepsPerRev);
  lcd.print("REV");
  lcd.setCursor(0, 1);
  lcd.print("STANDBY");
  lcd.print("  SPD:");
}

void loop() {
  int sensorReading = analogRead(Knob);
  int motorSpeed = map(sensorReading, 0, 1023, 0, 99);

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
        digitalWrite(LedA, HIGH);
        lcd.setCursor(10, 0);
        lcd.print(StepsPerRev);
        lcd.print("REV");
        lcd.setCursor(0, 1);
        lcd.print("PRINTING  SPD:");
      } else {
        lcd.clear();
        digitalWrite(LedA, LOW);
        lcd.setCursor(10, 0);
        lcd.print(StepsPerRev);
        lcd.print("REV");
        lcd.setCursor(0, 1);
        lcd.print("STANDBY  SPD:");
      }
    }
  }

  if (programOn) {
    if (motorSpeed > 0) {
      stepper_NEMA17.setSpeed(motorSpeed);
      stepper_NEMA17.step(StepsPerRev / 100);
    }
  }
}
