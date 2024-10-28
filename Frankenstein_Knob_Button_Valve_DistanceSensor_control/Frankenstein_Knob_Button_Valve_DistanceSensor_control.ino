/// Controlled flow extruder ///
/// Speed is regulated by potentiometer (knob) ///
/// The extrusion starts only when both buttons are pressed and the sensor is at the correct distance (below the defined value) ///
/// The first button activates the system and puts it in standby. The second button activates the valve that introduces compressed air into the system ///
/// Avoid Serial.Print type commands in the void loop to take full advantage of the motor speed ///

const int StepsPerRev = 400;
const int Knob = A0;
const int ButtonA = 9;
const int ButtonB = 11;
const int LedA = 8;
const int LedB = 10;
const int Valve = 14;
const int Echo = 3;
const int Trigger = 2;
const int ExtrusionDistance = 8;

int ButtonAState = LOW;
int ButtonBState = LOW;

long duration;
int distance;

bool programAOn = false;
bool programBOn = false;

#include <Stepper.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(23, 22, 25, 24, 27, 26);
Stepper stepper_NEMA17(StepsPerRev, 50, 51, 52, 53);

void setup() {
  pinMode(ButtonA, INPUT);
  pinMode(ButtonB, INPUT);
  pinMode(LedA, OUTPUT);
  pinMode(LedB, OUTPUT);
  pinMode(Valve, OUTPUT);
  pinMode(Trigger, OUTPUT); 
  pinMode(Echo, INPUT);
  Serial.begin(9600);
  Serial.println("Controlled flow extruder");
  Serial.println("Speed is regulated by potentiometer (knob)");
  Serial.println("The extrusion starts only when both buttons are pressed and the sensor is at the correct distance (below the defined value)");
  Serial.println("The first button activates the system and puts it in standby. The second button activates the valve that introduces compressed air into the system");
  Serial.println("Avoid Serial.Print type commands in the void loop to take full advantage of the motor speed");
  lcd.begin(8, 2);
  lcd.print("E.A.D.C. BUTTON");
  lcd.setCursor(0, 1);
  lcd.print("+DIS +VALV +KNOB");
  delay(2000);
  lcd.clear();
  lcd.print("VALVE:OFF ");
  lcd.print(StepsPerRev);
  lcd.print("REV");
  lcd.setCursor(0, 1);
  lcd.print("DIST:   ");
  lcd.print(" SPD:");
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

  lcd.setCursor(5, 1);
  lcd.print("   ");
  lcd.setCursor(5, 1);
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
      programAOn = !programAOn;
      if (programAOn) {
        digitalWrite(LedA, HIGH);
        lcd.setCursor(6, 0);
        lcd.print("ON ");
        digitalWrite(Valve, HIGH);
      } else {
        digitalWrite(LedA, LOW);
        lcd.setCursor(6, 0);
        lcd.print("OFF");
        digitalWrite(Valve, LOW);
      }
    }
  }

  int ButtonBReading = digitalRead(ButtonB);
  if (ButtonBReading != ButtonBState) {
    ButtonBState = ButtonBReading;
    if (ButtonBState == HIGH) {
      programBOn = !programBOn;
      if (programBOn) {
        digitalWrite(LedB, HIGH);
      } else {
        digitalWrite(LedB, LOW);
      }
    }
  }

  if (programBOn) {
    if (motorSpeed > 0) {
      stepper_NEMA17.setSpeed(motorSpeed);
      stepper_NEMA17.step(StepsPerRev / 100);
    }
  }
}
