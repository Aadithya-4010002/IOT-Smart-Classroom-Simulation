#include <Wire.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>

// Define pins
const int pirPin = 2;
const int ledPin = 13;
const int buzzerPin = 6;
const int ringPin = 9;
const int servoPin = 5;

Adafruit_NeoPixel ring = Adafruit_NeoPixel(12, ringPin, NEO_GRB + NEO_KHZ800);
Servo myServo;

unsigned long lastMotionTime = 0;
const unsigned long timeout = 5000;

void setup() {
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  myServo.attach(servoPin);
  myServo.write(0); // Start with the servo at 0 degrees

  ring.begin();
  ring.show();

  Serial.begin(9600);
}

void loop() {
  int motionState = digitalRead(pirPin);

  if (motionState == HIGH) {
    // Motion detected
    Serial.println("Motion detected! Turning on devices.");
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH); // Turn on buzzer

    // Turn on NeoPixel Ring (White color)
    for (int i = 0; i < ring.numPixels(); i++) {
      ring.setPixelColor(i, ring.Color(255, 255, 255));
    }
    ring.show();

    // Move the servo motor
    myServo.write(90); // Rotate to 90 degrees

    lastMotionTime = millis();
  }

  // Turn off devices after timeout
  if (millis() - lastMotionTime > timeout) {
    Serial.println("No motion. Turning off devices.");
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW); // Turn off buzzer

    // Turn off NeoPixel Ring
    for (int i = 0; i < ring.numPixels(); i++) {
      ring.setPixelColor(i, ring.Color(0, 0, 0));
    }
    ring.show();

    // Reset the servo motor
    myServo.write(0); // Rotate back to 0 degrees
  }

  delay(100);
}
