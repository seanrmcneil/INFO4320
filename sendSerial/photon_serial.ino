
void setup()
{
  Serial.begin(9600);
  Serial.flush();
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
      delay(1000);
      Serial.flush();
      Serial.println(message.length());
      //Serial.flush();
      start_drawing = false;
    }
}
