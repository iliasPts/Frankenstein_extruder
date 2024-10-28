/// Controlled flow extruder ///
/// Speed is regulated by potentiometer (knob) ///
/// The extrusion starts automatically as soon as the microcontroller is powered with 12 volts ///
/// Avoid Serial.Print type commands in the void loop to take full advantage of the motor speed ///

const int StepsPerRev = 400;
const int Knob = A0;
const int LedA = 8;

#include <Stepper.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(23, 22, 25, 24, 27, 26);
Stepper stepper_NEMA17(StepsPerRev, 50, 51, 52, 53);

void setup() {
  Serial.begin(9600);
  pinMode(LedA, OUTPUT);
  Serial.println("Controlled flow extruder");
  Serial.println("Speed is regulated by potentiometer (knob)");
  Serial.println("The extrusion starts automatically as soon as the microcontroller is powered with 12 volts");
  Serial.println("Avoid Serial.Print type commands in the void loop to take full advantage of the motor speed");
  lcd.begin(8, 2);
  lcd.print("E.A.D.C. KNOB");
  delay(2000);
  lcd.clear();
  lcd.setCursor(10, 0);
  lcd.print(StepsPerRev);
  lcd.print("REV");
  lcd.setCursor(9, 1);
  lcd.print("SPD:");
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

  if (motorSpeed > 0) {
    digitalWrite(LedA, HIGH);
    stepper_NEMA17.setSpeed(motorSpeed);
    stepper_NEMA17.step(StepsPerRev / 100);
  }
}
