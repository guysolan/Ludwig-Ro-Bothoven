#include <Ps3Controller.h>
#include <ESP32_Servo.h>

#define ENABLE 27
#define DIRA 26
#define DIRB 14

#define motor1Pin1 27
#define motor1Pin2  26
#define enable1Pin 14

// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 200;

//Digital Push Control
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

int servoTurnAngle = 90;

void clockwise(Servo &theServo) {
  theServo.write(90 - servoTurnAngle);
}

void anticlockwise(Servo &theServo) {
  theServo.write(90 + servoTurnAngle);
}

void homeServo(Servo &theServo) {
  theServo.write(90);
}

void notify()
{
  //------DC Motor-------
  if ( Ps3.event.button_down.r1 ) {
    Serial.println("Started pressing R1");
    moveright();
  }
  if ( Ps3.event.button_down.l1 ) {
    Serial.println("Started pressing the L1");
    moveleft();
  }

  // -------Servos------
  if ( Ps3.event.button_down.cross ) {
    Serial.println("Started pressing the cross button");
    clockwise(servo1);
  }

  if ( Ps3.event.button_up.cross ) {
    Serial.println("Released the cross button");
    homeServo(servo1);
  }

  if ( Ps3.event.button_down.circle ) {
    Serial.println("Started pressing the circle button");
    anticlockwise(servo1);
  }
  if ( Ps3.event.button_up.circle ) {
    Serial.println("Released the circle button");
    homeServo(servo1);
  }

  if ( Ps3.event.button_down.square ) {
    Serial.println("Started pressing the square button");
    anticlockwise(servo2);
  }
  if ( Ps3.event.button_up.square ) {
    Serial.println("Released the square button");
    homeServo(servo2);
  }
  if ( Ps3.event.button_down.triangle ) {
    Serial.println("Started pressing the triangle button");
    clockwise(servo2);
  }
  if ( Ps3.event.button_up.triangle ) {
    Serial.println("Released the triangle button");
    homeServo(servo2);
  }
  if ( Ps3.event.button_down.up ) {
    Serial.println("Started pressing the up button");
    clockwise(servo3);
  }

  if ( Ps3.event.button_up.up ) {
    Serial.println("Released the up button");
    homeServo(servo3);
  }

  if ( Ps3.event.button_down.right ) {
    Serial.println("Started pressing the right button");
    anticlockwise(servo3);
  }
  if ( Ps3.event.button_up.right ) {
    Serial.println("Released the right button");
    homeServo(servo3);
  }
  if ( Ps3.event.button_down.down ) {
    Serial.println("Started pressing the down button");
    clockwise(servo4);
  }
  if ( Ps3.event.button_up.down ) {
    Serial.println("Released the down button");
    homeServo(servo4);
  }
  if ( Ps3.event.button_down.left ) {
    Serial.println("Started pressing the left button");
    anticlockwise(servo4);
  }
  if ( Ps3.event.button_up.left ) {
    Serial.println("Released the left button");
    homeServo(servo4);
  }
}

void onConnect() {
  Serial.println("Connected.");
}

void setup()
{
  Serial.begin(115200);
  Ps3.attach(notify);
  Ps3.attachOnConnect(onConnect);
  Ps3.begin("0c:ee:e6:4e:fb:db");
  Serial.println("Ready.");

//   sets the pins as outputs:
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    pinMode(enable1Pin, OUTPUT);

  // configure LED PWM functionalitites
  ledcSetup(pwmChannel, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(enable1Pin, pwmChannel);
  ledcAttachPin(ENABLE, pwmChannel);

  servo1.attach(21);
  servo2.attach(22);
  servo3.attach(19);
  servo4.attach(23);

  pinMode(ENABLE, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
}

void loop()
{
  if (!Ps3.isConnected())
    return;
  delay(2000);
}


void moveleft() {
  digitalWrite(ENABLE, HIGH); // enable on
  digitalWrite(DIRA, HIGH); //one way
  digitalWrite(DIRB, LOW);
  delay(700);
  digitalWrite(ENABLE, LOW); // disable
}

void moveright() {
  digitalWrite(ENABLE, HIGH);
  digitalWrite(DIRA, LOW); //reverse
  digitalWrite(DIRB, HIGH);
  delay(700);
  digitalWrite(ENABLE, LOW); // disable
}
