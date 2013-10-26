
void setup()
{
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
}

void loop()
{
  int i;
  for (i=0; i<255; i++) {
    analogWrite(0,i);
    analogWrite(1,i);
    analogWrite(4,i);
    delay(10);
  }
}
