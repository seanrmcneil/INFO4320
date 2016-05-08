const int servoPin = RX;
Servo servo;

void setup(){
    Serial.begin(9600);
    servo.attach(servoPin);
    servo.write(50);
}

void loop(){
  servo.write(50);
  Serial.print("45 degrees");
  delay(1000);
  servo.write(0);
  Serial.print("0 degrees");
  delay(1000);
}
