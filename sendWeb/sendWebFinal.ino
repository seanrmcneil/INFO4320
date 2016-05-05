#include "AccelStepper.h"

//Positive motor steps == total motors

int motorSpeedX = 250; //Here you can set the steps per second (about 3rps / at 16 microsteps)
int motorAccelX = 80000; //Here you can set the acceleration Speed steps/second/second to accelerate

int motorSpeedY = 250;
int motorAccelY = 80000;

int motorDirPinX = D1; //direction pin - black
int motorStepPinX = D0; //step pin - white

int motorDirPinY = D3; //direction pin - black
int motorStepPinY = D4; //step pin - white

int screenX = 300;
int screenY = 600;

//set up the accelStepper intance
//the "1" tells it we are using a driver
AccelStepper stepperX(1, motorStepPinX, motorDirPinX);
AccelStepper stepperY(1, motorStepPinY, motorDirPinY);

void loop(){
  Particle.process();
}

void setup() {
  Particle.function("draw", draw);
  Serial.begin(9600);
  stepperX.setMaxSpeed(1000); //max speed the motor will go to
  stepperX.setSpeed(motorSpeedX); //the actual speed the motor will go (right now the same as the max speed)
  stepperX.setAcceleration(motorAccelX);//how much the motor will accelerate (not using right now)

  stepperY.setMaxSpeed(1000);
  stepperY.setSpeed(motorSpeedY);
  stepperY.setAcceleration(motorAccelY);
}

//int array_size = 0;
int draw(String code) {

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
  else {
    coord_num += c;
  }
}

Serial.println("X array");
for (int i= 0; i < array_size; i++){
  Serial.println(xArray[i]);
}

Serial.println("Y array");
for (int i= 0; i < array_size; i++){
  Serial.println(yArray[i]);
}

//moveMotor(xArray, yArray, array_size);
int testX[] = {150, 0, 0};
int testY[] = {150, 0, 0};
moveMotor(testX, testY, 3);

}

void moveMotor(int xArray[], int yArray[], int array_size){
  int motorstep = -1;
  bool coord_remaining = true;
  while(coord_remaining){
  if (motorstep<array_size){
    if(stepperX.distanceToGo()==0 && stepperY.distanceToGo()==0)
    {
      motorstep++;
      if (xArray[motorstep] == 942682160){
        Serial.println("lift pen!");
        delay(500);
        //lift pen
      }
      else {
      int y_mapped = map(yArray[motorstep], 0, screenY, 0, 1600);
      int x_mapped = map(xArray[motorstep], 0, screenX, -400, 0);
      stepperX.moveTo(x_mapped);
      stepperY.moveTo(y_mapped);
      //put down pen if it was lifted
      }
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
  stepperX.runToNewPosition(0);
  stepperX.disableOutputs();
  stepperY.runToNewPosition(0);
  stepperY.disableOutputs();
}
