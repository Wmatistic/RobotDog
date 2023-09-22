#include <HCPCA9685.h>

#include <Servo.h>

#define I2CAdd 0x40

HCPCA9685 HCPCA9685(I2CAdd);

Servo thigh;

double thighLength = 15;
double calfLength = 15;

double a1, a2;
double q1, q2;

double x, y;

// Joystick
const int Xpin = A1;
const int Ypin = A0;

double joystickX, joystickY;

void setup() {
  Serial.begin(9600);

  thigh.attach(9);

  a1 = thighLength;
  a2 = calfLength;

  x = 0.0;
  y = 10.0;
}

void loop() {
  
  joystickX = analogRead(Xpin);
  joystickY = analogRead(Ypin);

//  Serial.print("Joystick X: ");
//  Serial.println(joystickX);
//  Serial.print("Joystick Y: ");
//  Serial.println(joystickY);

  if (joystickY == 0) {
    y -= 0.1;
    //delay(500);
  } else if (joystickY > 600) {
    y += 0.1;
    //delay(500);
  }

  if (joystickX == 0) {
    x -= 0.1;
    //delay(500);
  } else if (joystickX > 600) {
    x += 0.1;
    //delay(500);
  }

//  Serial.print("q1: ");
//  Serial.println((q1 / (2 * PI)) * 360);
//  Serial.print("q2: ");
//  Serial.println(180 - ((q2 / (2 * PI)) * 360));
//
  Serial.println(x);
  Serial.println(y);
  
  calculateLegAngles(x, y);

  // THIGH
  HCPCA9685.Servo(0, (1 * ((q1 / (2 * PI)) * 360)));
  // CALF
  HCPCA9685.Servo(1, (127.5 - (q2 / (2 * PI)) * 255));

  
}

void calculateLegAngles(double x, double y) {
  calculateCalfAngle(x, y);
  calculateThighAngle(x, y);
}

void calculateCalfAngle(double x, double y) {
  q2 = acos(
      (pow(x, 2)
      + pow(y, 2)
      - pow(a1, 2)
      - pow(a2, 2))
      / (2 * a1 * a2)
    );
}

void calculateThighAngle(double x, double y) {
  q1 = atan(y / x)
      - atan((a2 * sin(q2))
      / (a1 + a2 * cos(q2)));
}
