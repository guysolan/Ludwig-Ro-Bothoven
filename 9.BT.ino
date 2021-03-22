#include <PS3BT.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
PS3BT PS3(&Btd); // This will just create the instance

bool printTemperature, printAngle;

// Servo Initialisation
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

//DC Motor Initialisation

#define ENABLE 4
#define DIRA 2
#define DIRB 3

// 74HC595 Set Up
int latchPin = 6; //Pin 12 of 74HC595
int clockPin = 7; //Pin 11 of 74HC595
int dataPin = 5; //Pin 14 of 74HC595
int dt = 250;

byte LED0s = 0b00000000;
byte LED1s = 0b10000000;
byte LED2s = 0b01000000;
byte LED3s = 0b00100000;
byte LED4s = 0b00010000;
byte LED5s = 0b00001000;
byte LED6s = 0b00000100;
byte LED7s = 0b00000010;
byte LED8s = 0b00000001;

byte REDLEDs = 0b10101010;
byte BLUELEDs = 0b01010101;

int bpm = 1000;
int semi_demi_hemi_quaver = bpm / 16;
int semi_quaver = bpm / 4;
int quaver = bpm / 2;
int dot_quaver = bpm * 3 / 4;
int crotchet = bpm;
int dot_crotchet = bpm * 3 / 2;
int minim = bpm * 2;

bool started = false;

void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));

  servo1.attach(13, 600, 2300);
  servo2.attach(12, 600, 2300);
  servo3.attach(11, 600, 2300);
  servo4.attach(8, 600, 2300);
  Serial.print(F("\r\nServos Attached"));

  //---set pin direction for DC motor
  pinMode(ENABLE, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);

  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  Serial.print(F("\r\nLEDs Attached"));
}
void loop() {

  Usb.Task();

  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
    if (PS3.getAnalogHat(LeftHatX) > 147 ||  PS3.getAnalogHat(LeftHatY) > 147 || PS3.getAnalogHat(RightHatX) > 137 || PS3.getAnalogHat(RightHatY) > 137) {
      Serial.print(F("\r\nLeftHatX: "));
      Serial.print(PS3.getAnalogHat(LeftHatX));

      analogWrite(ENABLE, 255); //enable on
      digitalWrite(DIRA, HIGH); //one way
      digitalWrite(DIRB, LOW);
      analogWrite(ENABLE, LeftHatX); //half speed

      Serial.print(F("\tLeftHatY: "));
      Serial.print(PS3.getAnalogHat(LeftHatY));
      if (PS3.PS3Connected) { // The Navigation controller only have one joystick
        Serial.print(F("\tRightHatX: "));
        Serial.print(PS3.getAnalogHat(RightHatX));
        Serial.print(F("\tRightHatY: "));
        Serial.print(PS3.getAnalogHat(RightHatY));
      }
    }
    else if (PS3.getAnalogHat(LeftHatX) < 117 || PS3.getAnalogHat(LeftHatY) < 117 ||  PS3.getAnalogHat(RightHatX) < 117 || PS3.getAnalogHat(RightHatY) < 117) {
      Serial.print(F("\r\nLeftHatX: "));
      Serial.print(PS3.getAnalogHat(LeftHatX));

      analogWrite(ENABLE, 255); //enable on
      digitalWrite(DIRB, LOW);
      digitalWrite(DIRA, HIGH); //other way
      analogWrite(ENABLE, LeftHatX); //half speed


      Serial.print(F("\tLeftHatY: "));
      Serial.print(PS3.getAnalogHat(LeftHatY));
      if (PS3.PS3Connected) { // The Navigation controller only have one joystick
        Serial.print(F("\tRightHatX: "));
        Serial.print(PS3.getAnalogHat(RightHatX));
        Serial.print(F("\tRightHatY: "));
        Serial.print(PS3.getAnalogHat(RightHatY));
      }
    }
    else {
      digitalWrite(ENABLE, LOW); //all done
    }

    // Analog button values can be read from almost all buttons
    if (PS3.getAnalogButton(L2) || PS3.getAnalogButton(R2)) {
      Serial.print(F("\r\nL2: "));
      Serial.print(PS3.getAnalogButton(L2));

      if (PS3.PS3Connected) {
        Serial.print(F("\tR2: "));
        Serial.print(PS3.getAnalogButton(R2));
      }
    }

    if (PS3.getButtonClick(PS)) {
      Serial.print(F("\r\nPS"));
      PS3.disconnect();
    }
    else {
      if (PS3.getButtonClick(SQUARE)) {
        Serial.print(F("\r\nSquare"));
        lightServo(LED8s, servo1, 0, 200);
      }
      if (PS3.getButtonClick(TRIANGLE)) {
        Serial.print(F("\r\nTriangle"));
        lightServo(LED7s, servo1, 180, 200);
      }
      if (PS3.getButtonClick(CIRCLE)) {
        Serial.print(F("\r\nCircle"));
        lightServo(LED6s, servo2, 0, 200);
      }
      if (PS3.getButtonClick(CROSS)) {
        Serial.print(F("\r\nCross"));
        lightServo(LED5s, servo2, 180, 200);
      }
      if (PS3.getButtonClick(UP)) {
        Serial.print(F("\r\nUp"));
        lightServo(LED4s, servo3, 0, 200);
      }
      if (PS3.getButtonClick(RIGHT)) {
        Serial.print(F("\r\nRight"));
        lightServo(LED3s, servo3, 180, 200);
      }
      if (PS3.getButtonClick(DOWN)) {
        Serial.print(F("\r\nDown"));
        lightServo(LED2s, servo4, 0, 200);
      }
      if (PS3.getButtonClick(LEFT)) {
        Serial.print(F("\r\nLeft"));
        lightServo(LED1s, servo4, 180, 200);
      }
      if (PS3.getButtonClick(L1)) {
        Serial.print(F("\r\nL1"));
        moveleft();
      }
      if (PS3.getButtonClick(R1)) {
        Serial.print(F("\r\nR1"));
        moveright();
      }
      if (PS3.getButtonClick(SELECT)) {
        Serial.print(F("\r\nSelect - "));
        seven_nation_army();
      }
      if (PS3.getButtonClick(START)) {
        Serial.print(F("\r\nStart"));
        sonata();
      }
    }
  }
}

void lightServo(byte LEDN, Servo & theServo, int angle, int duration) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, LEDN);
  digitalWrite(latchPin, HIGH);
  theServo.write(angle);
  delay(duration * 4 / 5);
  theServo.write(90);
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, LED0s);
  digitalWrite(latchPin, HIGH);
  delay(duration * 1 / 5);
}

void moveleft() {
  digitalWrite(ENABLE, HIGH); // enable on
  digitalWrite(DIRA, HIGH); //one way
  digitalWrite(DIRB, LOW);
  delay(1000);
  digitalWrite(ENABLE, LOW); // disable
}

void moveright() {
  digitalWrite(ENABLE, HIGH);
  digitalWrite(DIRA, LOW); //reverse
  digitalWrite(DIRB, HIGH);
  delay(1000);
  digitalWrite(ENABLE, LOW); // disable
}

void lightup(byte LEDN, int duration) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, LEDN);
  digitalWrite(latchPin, HIGH);
  delay(duration * 4 / 5);
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, LED0s);
  digitalWrite(latchPin, HIGH);
  delay(duration * 1 / 5);
}

void staylit(byte LEDN) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, LEDN);
  digitalWrite(latchPin, HIGH);
}

void rest(int duration) {
  delay(duration);
}

void seven_nation_army() {
  bpm = 700;
  //Seven Nation Army
  //Bar 1
  lightServo(LED5s, servo2, 180, dot_crotchet);
  lightServo(LED5s, servo2, 180, quaver);
  lightServo(LED7s, servo1, 180, dot_quaver);
  lightServo(LED5s, servo2, 180, semi_quaver);
  rest(quaver);
  lightServo(LED4s, servo3, 0, quaver);
  //Bar 2
  lightServo(LED3s, servo3, 180, minim);
  lightServo(LED2s, servo4, 0, minim);
}

void sonata() {
  //Bar 3
  lightServo(LED2s, servo4, 0, semi_quaver); //F
  lightServo(LED3s, servo3, 180, semi_quaver); //G
  //BEAT
  lightServo(LED4s, servo3, 0, semi_quaver); //A
  lightServo(LED5s, servo2, 180, semi_quaver); //B
  lightServo(LED6s, servo2, 0, semi_quaver); //C
  lightServo(LED7s, servo1, 180, semi_quaver);//D
  //BEAT
  lightServo(LED8s, servo1, 0, semi_quaver); //E
  lightServo(LED7s, servo1, 180, semi_quaver); //D
  lightServo(LED6s, servo2, 0, semi_quaver); //C
  lightServo(LED5s, servo2, 180, semi_quaver); //B
  //BEAT
  lightServo(LED4s, servo3, 0, semi_quaver); //A
  lightServo(LED3s, servo3, 180, semi_quaver); //G
  lightServo(LED2s, servo4, 0, semi_quaver); //F
  lightServo(LED1s, servo4, 180, semi_quaver); //E
  rest(crotchet);
  //beat
  rest(quaver);
  rest(semi_quaver);
  lightServo(LED1s, servo4, 180, semi_quaver); //C
  // BEAT
  lightServo(LED2s, servo4, 0, semi_quaver); //D
  rest(quaver);
  lightServo(LED1s, servo4, 180, semi_quaver); //C
  //BEAT
  lightServo(LED2s, servo4, 0, semi_quaver); //D
  lightServo(LED3s, servo3, 180, semi_quaver); //E
  lightServo(LED4s, servo3, 0, semi_quaver); //F
  lightServo(LED5s, servo2, 180, semi_quaver); //G
}
