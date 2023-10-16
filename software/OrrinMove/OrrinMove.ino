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
const double thighLength = 15;
const double calfLength = 15;
double angles[4][3];

//  Front Left LEG
const int FLThigh = 0;
const int FLCalf = 1;
const int FLHip = 2;

//  Front Right LEG
const int FRThigh = 3;
const int FRCalf = 4;
const int FRHip = 5;

//  Back Left LEG
const int BLThigh = 6;
const int BLCalf = 7;
const int BLHip = 8;

// Back Right LEG
const int BRThigh = 9;
const int BRCalf = 10;
const int BRHip = 11;

//    JOYSTICK
const int XPin = A1;
const int YPin = A0;
double joystickX, joystickY;

void setup() {
  Serial.begin(9600);
}

void loop() {
  
  joystickX = analogRead(XPin);
  joystickY = analogRead(YPin);

  moveOrrin(0, 20,0,
            0, 0, 0,
            0, 0, 0,
            0, 0, 0);

  Serial.print("thigh: ");
  Serial.println(angles[0][0]);
  Serial.print("calf: ");
  Serial.println(angles[0][1]);
  //Serial.println(angles[0][2]);
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

  //  Front Left LEG
  // THIGH
  HCPCA9685.Servo(FLThigh, angles[0][0]);
  // CALF
  HCPCA9685.Servo(FLCalf, angles[0][1]);
  // HIP
  HCPCA9685.Servo(FLHip, angles[0][2]);
}

void calculateLegAngles(double pos[4][3]) {

  for (int i = 0; i < sizeof(pos); i++){
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

     angles[i][0] = convertThighAngleToServo(q1);
     angles[i][1] = convertCalfAngleToServo(q2);
     angles[i][2] = r;
  }
}

double convertThighAngleToServo(double angle) {
  return (127.5 - (angle / (2 * PI)) * 255);
}

double convertCalfAngleToServo(double angle) {
  return ((angle / (2 * PI)) * 255);
}
