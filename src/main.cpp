#include <Arduino.h>
#include <AFMotor.h>
#include <Servo.h>

// Motor setup
AF_DCMotor motor1(1);
AF_DCMotor motor2(4);
AF_DCMotor motor3(3);
AF_DCMotor motor4(2);

// Servo setup
Servo servo1;
Servo servo2;

// Variables
char c;
// int moistureValue;

// Function prototypes
void move(byte dir, byte speed);
void set_speed(byte speed);
void extend_servo(Servo servo);
void retract_servo(Servo servo);

void move(byte dir, byte speed) {
  motor1.setSpeed(speed);
  motor2.setSpeed(speed);
  motor3.setSpeed(speed);
  motor4.setSpeed(speed);

  switch (dir) {
    case 'f':
      motor1.run(FORWARD);
      motor2.run(FORWARD);
      motor3.run(FORWARD);
      motor4.run(FORWARD);
      break;
    case 'b':
      motor1.run(BACKWARD);
      motor2.run(BACKWARD);
      motor3.run(BACKWARD);
      motor4.run(BACKWARD);
      break;
    case 'l':
      motor1.run(BACKWARD);
      motor2.run(BACKWARD);
      motor3.run(FORWARD);
      motor4.run(FORWARD);
      break;
    case 'r':
      motor1.run(FORWARD);
      motor2.run(FORWARD);
      motor3.run(BACKWARD);
      motor4.run(BACKWARD);
      break;
    case 's':
      motor1.run(RELEASE);
      motor2.run(RELEASE);
      motor3.run(RELEASE);
      motor4.run(RELEASE);
      break;
    default:
      break;
  }
}

void set_speed(byte speed) {
  motor1.setSpeed(speed);
  motor2.setSpeed(speed);
  motor3.setSpeed(speed);
  motor4.setSpeed(speed);
}

void extend_servo(Servo servo) {
  for (int pos = 0; pos <= 180; pos += 1) {
    servo.write(pos);
    delay(10);
  }
}

void retract_servo(Servo servo) {
  for (int pos = 180; pos >= 0; pos -= 1) {
    servo.write(pos);
    delay(10);
  }
}
int moisture;
int moistureV;

void process_command(char c) {
  switch (c) {
    case 'f':
    case 'b':
    case 's':
      move(c, 127);
      break;
    case 'l':
    case 'r':
      move(c, 255);
      break;

    case 'm':
      // Serial.println("Taking measurement");
      // Serial1.println("Taking measurement");

      extend_servo(servo1);
      delay(1500);
      moisture = analogRead(A3);
      moistureV = map(moisture, 500, 1021, 0, 100);
      moistureV = 100 - moistureV;

      delay(500);
      retract_servo(servo1);

      Serial.print("water:");
      Serial1.print("water:");
      Serial.println(moistureV);
      Serial1.println(moistureV);

      break;

    case 'T':
      Serial.println("Testing");
      move('f', 255);
      delay(500);
      move('b', 255);
      delay(500);
      move('l', 255);
      delay(500);
      move('r', 255);
      delay(500);
      move('s', 255);

      extend_servo(servo1);
      delay(1500);
      moisture = analogRead(A3);
      Serial.print(moisture);
      moistureV = map(moisture, 500, 1021, 0, 100);
      moistureV = 100 - moistureV;
      delay(500);
      retract_servo(servo1);
      Serial.print("water:");
      Serial.println(moistureV);
      
      break;

    default:
      break;
    
  }
}



void setup() {
  Serial.begin(9600);

  Serial1.begin(9600);

  // Motor setup
  motor1.setSpeed(127);
  motor2.setSpeed(127);
  motor3.setSpeed(127);
  motor4.setSpeed(127);

  pinMode(A3, INPUT);
  servo1.attach(10);
  servo1.write(0);

  servo2.attach(9);
  servo2.write(0);
  
  // Initial motor test
  // move('f', 255);
  // delay(500);
  // move('s', 255);
}

void loop() {
  if (Serial.available()) {
    c = Serial.read();
    Serial.print("Received: ");
    Serial.println(c);
    process_command(c);
  }

  if (Serial1.available()) {
    c = Serial1.read();
    Serial1.print("Received | 1: ");
    Serial1.println(c);
    process_command(c);
  }
}
