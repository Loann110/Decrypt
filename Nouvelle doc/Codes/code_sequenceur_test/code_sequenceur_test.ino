#include <Servo.h>

// Pins
#define enable_stepper A1       // Special: shared with button_launch
#define step 3
#define direction_stepper 4
#define pwm_pin 6

#define button_trap A0          // Button to manually open/close the parachute trap
#define led_trap_closed 12      // LED indicating trap is closed
#define led_trap_opened 13      // LED indicating trap is opened

Servo servo1;

// Flight States
enum FlightState {WAIT_LAUNCH, FLIGHT, PARACHUTE_DEPLOY, MOTOR_RUNNING, FINISHED};
FlightState etat = WAIT_LAUNCH;

// Variables
unsigned long timerStart = 0;
bool trapClosed = true;

// Stepper
unsigned long lastStepTime = 0;
const unsigned long stepInterval = 1000; // Step interval in microseconds
bool stepState = false;

// PWM
int dutyCycle = 128; // PWM value (0-255)

void setup() {
  Serial.begin(9600);

  pinMode(step, OUTPUT);
  pinMode(direction_stepper, OUTPUT);
  pinMode(pwm_pin, OUTPUT);

  pinMode(button_trap, INPUT);   // External pull-down resistor
  pinMode(enable_stepper, INPUT); // Shared: input first

  pinMode(led_trap_closed, OUTPUT);
  pinMode(led_trap_opened, OUTPUT);

  servo1.attach(10);
  servo1.write(0); // Trap closed at startup

  updateTrapLEDs();
  Serial.println("System ready. Waiting manual trap control and launch...");
}

void loop() {
  unsigned long currentMillis = millis();

  switch (etat) {
    case WAIT_LAUNCH:
      if (digitalRead(button_trap) == HIGH) {
        delay(200); // Debounce
        if (trapClosed) {
          Serial.println("Manual: Opening parachute trap...");
          servo1.write(90);
          trapClosed = false;
        } else {
          Serial.println("Manual: Closing parachute trap...");
          servo1.write(0);
          trapClosed = true;
        }
        updateTrapLEDs();
        while (digitalRead(button_trap) == HIGH); // Wait button release
        delay(200);
      }

      if (digitalRead(enable_stepper) == HIGH) { // Button launch
        delay(200);
        if (trapClosed) {
          Serial.println("Launch confirmed. Starting flight!");
          pinMode(enable_stepper, OUTPUT);
          digitalWrite(enable_stepper, LOW); // Enable motor
          timerStart = millis();
          etat = FLIGHT;
        } else {
          Serial.println("ERROR: Close the trap before launching!");
        }
        while (digitalRead(enable_stepper) == HIGH); // Wait button release
        delay(200);
      }
      break;

    case FLIGHT:
      if (currentMillis - timerStart >= 15000) { // After 15 seconds
        Serial.println("Deploying parachute...");
        servo1.write(90);
        timerStart = millis();
        etat = PARACHUTE_DEPLOY;
      }
      break;

    case PARACHUTE_DEPLOY:
      if (currentMillis - timerStart >= 5000) { // After 5 seconds parachute deployed
        Serial.println("Activating stepper motor and PWM output...");
        digitalWrite(direction_stepper, HIGH); // Set stepper direction
        analogWrite(pwm_pin, dutyCycle);        // Start PWM
        timerStart = millis();
        etat = MOTOR_RUNNING;
      }
      break;

    case MOTOR_RUNNING:
      if (currentMillis - timerStart <= 5000) {
        gererStepper();
      } else {
        Serial.println("Stopping stepper motor and PWM output. Sequence finished!");
        digitalWrite(pwm_pin, LOW);
        digitalWrite(step, LOW);
        etat = FINISHED;
      }
      break;

    case FINISHED:
      // Do nothing - Flight finished
      break;
  }
}

void gererStepper() {
  unsigned long currentMicros = micros();
  if (currentMicros - lastStepTime >= stepInterval) {
    lastStepTime = currentMicros;
    stepState = !stepState;
    digitalWrite(step, stepState);
  }
}

void updateTrapLEDs() {
  if (trapClosed) {
    digitalWrite(led_trap_closed, HIGH);
    digitalWrite(led_trap_opened, LOW);
  } else {
    digitalWrite(led_trap_closed, LOW);
    digitalWrite(led_trap_opened, HIGH);
  }
}
