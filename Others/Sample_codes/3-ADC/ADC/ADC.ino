/*To read the analog values and print it on the Serial monitor*/

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600); 
  }

void loop()
{
  // put your main code here, to run repeatedly:
  unsigned int adc_val;
  static unsigned int pre_val=0;
  adc_val = analogRead(A0);
  if (pre_val != adc_val)
  {
    pre_val = adc_val;
    Serial.println(adc_val);
  }
  delay(1000);
}
