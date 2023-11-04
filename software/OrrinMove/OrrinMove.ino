#include <HCPCA9685.h>

#define I2CAdd 0x40

HCPCA9685 HCPCA9685(I2CAdd);

/*
Copy Paste Movement Input
  moveOrrin(0, 0, 0,
            0, 0, 0,
            0, 0, 0,
            0, 0, 0);
*/

//      LEGS
const double thighLength = 6.5;
const double calfLength = 6.5;
const double maxY = calfLength + thighLength;
double angles[4][3];

//  Front Left LEG
const int FLThigh = 0;
const double FLThighOffset = 0.9;
const int FLCalf = 1;
const double FLCalfOffset = 1.0;
const int FLHip = 2;

//  Front Right LEG
const int FRThigh = 3;
const double FRThighOffset = 1.0;
const int FRCalf = 4;
const double FRCalfOffset = 1.2;
const int FRHip = 5;

//  Back Left LEG
const int BLThigh = 6;
const double BLThighOffset = 1.2;
const int BLCalf = 7;
const double BLCalfOffset = 1.;
const int BLHip = 8;

// Back Right LEG
const int BRThigh = 9;
const double BRThighOffset = 1.0;
const int BRCalf = 10;
const double BRCalfOffset = 1.0;
const int BRHip = 11;

//    JOYSTICK
const int XPin = A0;
const int YPin = A1;
double joystickX, joystickY;
double tempX, tempY = 2;
double tempR = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  
  joystickX = analogRead(XPin);
  joystickY = analogRead(YPin);

  if (joystickY > 600 && tempY > 0) {
    tempY -= 0.3;
  } else if (joystickY == 0 && tempY < maxY) {
    tempY += 0.3;
  }
  if (joystickX > 600) {
    tempX -= 0.3;
  } else if (joystickX == 0) {
    tempX += 0.3;
  }

  moveOrrin(tempX, tempY, tempR,
            0, tempY, 150+tempR,
            0, tempY, tempR,
            0, 0, 0);

  Serial.print("thigh: ");
  Serial.println(convertThighAngleToServo(angles[1][0]));
  Serial.print("calf: ");
  Serial.println(convertCalfAngleToServo(angles[1][1]));
  Serial.println(tempR);
  //Serial.println(angles[1][2]);
}

void moveOrrin(double x0, double y0, double r0,
               double x1, double y1, double r1,
               double x2, double y2, double r2,
               double x3, double y3, double r3) {
               
  double pos[4][3] = {{x0, y0, r0},
                      {x1, y1, r1},
                      {x2, y2, r2},
                      {x3, y3, r3}};
  
  calculateLegAngles(pos);
  delay(100);

  //  Front Left LEG
  // THIGH
  HCPCA9685.Servo(FLThigh, altConvertThighAngleToServo(angles[0][0]) * FLThighOffset);
  // CALF
  HCPCA9685.Servo(FLCalf, 90);
  // HIP
  HCPCA9685.Servo(FLHip, angles[0][2]);

  //  Front Right LEG
  // THIGH
  HCPCA9685.Servo(FRThigh, convertThighAngleToServo(angles[1][0]));
  // CALF
  HCPCA9685.Servo(FRCalf, altConvertCalfAngleToServo(angles[1][1]));
  // HIP
  HCPCA9685.Servo(FRHip, angles[1][2]);

  //  Back Left LEG
  // THIGH
  HCPCA9685.Servo(BLThigh, convertThighAngleToServo(angles[2][0]) * BLThighOffset);
  // CALF
  HCPCA9685.Servo(BLCalf, altConvertCalfAngleToServo(angles[2][1]) * BLCalfOffset);
  // HIP
  HCPCA9685.Servo(BLHip, angles[2][2]);

//  //  Back Right LEG
//  // THIGH
//  HCPCA9685.Servo(BRThigh, convertThighAngleToServo(angles[3][0]));
//  // CALF
//  HCPCA9685.Servo(BRCalf, convertCalfAngleToServo(angles[3][1]));
//  // HIP
//  HCPCA9685.Servo(BRHip, angles[3][2]);

  
}

void calculateLegAngles(double pos[4][3]) {

  for (int i = 0; i <= sizeof(pos); i++){
    double x = pos[i][0];
    double y = pos[i][1];
    double r = pos[i][2];
    
    double q2 = acos(
        (pow(x, 2)
        + pow(y, 2)
        - pow(thighLength, 2)
        - pow(calfLength, 2))
        / (2 * thighLength * calfLength)
      );
     double q1 = atan(y / x)
        - atan((calfLength * sin(q2))
        / (thighLength + calfLength * cos(q2)));

     angles[i][0] = q1;
     angles[i][1] = q2;
     angles[i][2] = r;
  }
}

double convertThighAngleToServo(double angle) {
  return (angle * (180 / PI));
}

double altConvertThighAngleToServo(double angle) {
  return (180-(angle * (180 / PI)));
}

double convertCalfAngleToServo(double angle) {
  return (angle * (180 / PI));
}

double altConvertCalfAngleToServo(double angle) {
  return  (90-(angle * (180 / PI)));
}
