/*To control the brightness of the led using PWM(analogwrite)*/

const int analog_ip = A1;
const int LED = 3;
int inputVal = 0;

void setup() {
  pinMode (LED, OUTPUT);
}

void loop() {
  inputVal = analogRead(analog_ip);
  analogWrite (LED, inputVal/4);
  delay(1000);
}
