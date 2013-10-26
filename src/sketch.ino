
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
  for (i=0; i<6; i++) {
    digitalWrite(i,HIGH);
  }
  delay(300);
  for (i=0; i<6; i++) {
    digitalWrite(i,LOW);
  }
  delay(300);
}
