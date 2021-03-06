#include "AccelStepper.h"

//Positive motor steps == total motors

int motorSpeedX = 500; //Here you can set the steps per second (about 3rps / at 16 microsteps)
int motorAccelX = 80000; //Here you can set the acceleration Speed steps/second/second to accelerate

int motorSpeedY = 500;
int motorAccelY = 80000;

int motorDirPinX = D1; //direction pin - black
int motorStepPinX = D0; //step pin - white

int motorDirPinY = D3; //direction pin - black
int motorStepPinY = D4; //step pin - white

int screenX = 550; //width of canvas element from touch.html
int screenY = 825; //

const int servoPin = RX;
Servo servo;

const int penUp = 0;
const int penDown = 50;

AccelStepper stepperX(1, motorStepPinX, motorDirPinX);
AccelStepper stepperY(1, motorStepPinY, motorDirPinY);


void setup()
{
  Serial.begin(9600);
  Serial.flush();
  stepperX.setMaxSpeed(500); //max speed the motor will go to
  stepperX.setSpeed(motorSpeedX); //the actual speed the motor will go (right now the same as the max speed)
  stepperX.setAcceleration(motorAccelX);//how much the motor will accelerate (not using right now)

  stepperY.setMaxSpeed(500);
  stepperY.setSpeed(motorSpeedY);
  stepperY.setAcceleration(motorAccelY);

  servo.attach(servoPin);
  servo.write(penUp); //initialize pen up
}

String message = "";
bool wait_for_end = false;
bool start_drawing = false;
char c;

void loop()
{
    if (Serial.available()) {
      c = Serial.read();
      message += c;
      if (c == 'e') {
        start_drawing = true;
        wait_for_end = false;
      }
      else {
        wait_for_end = true;
        start_drawing = false;
      }
      delay(1);
    }
    else if (wait_for_end == true) {
      delay(500);
      Serial.print('*');
      //wait_for_end = false;
    }
    else if (start_drawing == true) {
      delay(3000);
      Serial.flush();
      draw(message);
      //Serial.println(message.length());
      start_drawing = false;
    }
}

void draw(String code) {
int array_size = 0;
  for (int i = 0; i< code.length(); i++){
    char c = code[i];
    if (c== 'y'){
      break;
    }
    else if(c == ','){
      array_size += 1;
    }
  }

Serial.println(array_size);
  int xArray[array_size];
  int yArray[array_size];

  int count = 0;
  int index = 0;
  String coord_num = "";

for (int i = 0; i< code.length(); i++){
  char c = code[i];
  if (c == 'x'){
    coord_num = "";
  }
  else if (c == 'y'){
    count = 0;
    index = i+1;
    break;
  }
  else if (c == 'u'){
    coord_num = '8000';
  }
  else if(c == ','){
    int b = atoi(coord_num.c_str());
      xArray[count] = b;
      count += 1;
      coord_num = "";
  }
  else {
    coord_num += c;
  }
}

//Y array values start here
for (int i = index; i< code.length(); i++){
  char c = code[i];
  if(c == ','){
    int b = atoi(coord_num.c_str());
      yArray[count] = b;
      count += 1;
      coord_num = "";
  }
  else if (c == 'u'){
    coord_num = '8000';
  }
  else if (c == 'e'){
      break;
  }
  else {
    coord_num += c;
  }
}


moveMotor(xArray, yArray, array_size);
//spiral test 148 long
//int testX[] = {162,158,154,147,141,137,133,129,127,124,122,121,118,114,112,108,105,103,102,102,101,101,101,101,101,102,103,106,110,113,117,121,125,130,134,137,140,143,147,153,158,163,169,176,183,191,199,208,216,224,232,238,243,247,251,256,261,265,270,273,277,279,281,283,283,284,285,285,285,285,285,282,280,278,274,271,268,264,260,255,251,248,243,240,236,229,226,222,218,213,209,205,200,196,193,189,187,184,182,180,176,174,172,170,169,168,168,167,167,167,167,167,167,167,167,167,167,169,170,171,172,174,175,176,178,179,181,184,187,189,191,193,194,196,198,200,201,202,203,204,205,206,206,207,207,208,208, 942682160};
//int testY[] = {69,71,75,80,85,87,91,95,97,100,101,103,106,113,119,126,132,137,142,148,153,158,162,167,172,176,179,183,187,191,195,200,204,209,214,218,220,223,225,227,229,230,231,231,231,231,231,230,228,226,223,221,218,216,214,211,208,204,199,195,189,185,180,175,170,165,162,160,157,155,151,147,144,141,138,135,132,130,127,126,124,122,120,120,119,117,117,116,116,116,116,116,116,116,118,120,121,123,125,128,130,132,135,138,140,142,143,144,145,146,149,152,156,160,162,164,165,166,166,166,167,167,168,169,169,169,170,170,170,172,172,172,172,172,172,170,170,168,167,165,162,159,157,155,153,152,151, 942682160};

//int testX[] = {100, 200, 942682160, 0, 300, 942682160, 100, 400, 942682160};
//int testY[] = {100, 200, 942682160, 0, 300, 942682160, 0, 600, 942682160};
//moveMotor(testX, testY, 9);

}

void moveMotor(int xArray[], int yArray[], int array_size){
  int motorstep = -1;
  bool coord_remaining = true;
  bool pen_up = true;
  bool reached_putdown = false;

  while(coord_remaining){
  if (motorstep<array_size){
    if(stepperX.distanceToGo()==0 && stepperY.distanceToGo()==0){
      motorstep++;
      if (xArray[motorstep] == 942682160){
        servo.write(penUp);
        pen_up = true;
        reached_putdown = false;
        delay(500);
      }
      else {
      int y_mapped = map(yArray[motorstep], 0, screenY, 0, 1297);
      int x_mapped = map(xArray[motorstep], 0, screenX, 0, -865);
      stepperX.moveTo(x_mapped);
      stepperY.moveTo(y_mapped);
      }
      if(pen_up && reached_putdown){
        servo.write(penDown);
        pen_up = false;
        delay(500);
      }
    }else if(!reached_putdown){
      reached_putdown = true;
    }

    stepperX.run();
    stepperY.run();
  }
  else{
    coord_remaining = false;
    Serial.print("ended");
    resetMotors();
    }
  }
}

void resetMotors (){
  message = "";
  stepperX.runToNewPosition(0);
  stepperX.disableOutputs();
  stepperY.runToNewPosition(0);
  stepperY.disableOutputs();
}
