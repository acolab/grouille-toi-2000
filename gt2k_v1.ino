/*
  Serial RGB controller
 
 Reads a serial input string looking for three comma-separated
 integers with a newline at the end. Values should be between 
 0 and 255. The sketch uses those values to set the color 
 of an RGB LED attached to pins 9 - 11.
 
 The circuit:
 * Common-anode RGB LED cathodes attached to pins 9 - 11
 * LED anode connected to pin 13
 
 To turn on any given channel, set the pin LOW.  
 To turn off, set the pin HIGH. The higher the analogWrite level,
 the lower the brightness.
 
 created 29 Nov 2010
 by Tom Igoe
 
 This example code is in the public domain. 
 */

String inString = "";    // string to hold input
int currentColor = 0;
int red, green, blue = 0;

int WledPin = 9;  // White leds ramp
int RledPin = 6;   // Red Leds
int GledPin = 10;  // Green Leds
int BledPin = 5;  // Blue Leds

int buttonPin = 4;
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers



void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // send an intro:
  Serial.println("\n\nString toInt() RGB:");
  Serial.println();
  // set LED cathode pins as outputs:
  pinMode(RledPin, OUTPUT);
  pinMode(GledPin, OUTPUT);
  pinMode(BledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  // turn on pin 13 to power the LEDs:
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  


  
  
  
}

void loop() {
  int inChar;

  // Read serial input:
  if (Serial.available() > 0) {
    inChar = Serial.read();
    //Serial.write(inChar);
  }

  if (isDigit(inChar)) {
    // convert the incoming byte to a char 
    // and add it to the string:
    inString += (char)inChar; 
  }

  // if you get a comma, convert to a number,
  // set the appropriate color, and increment
  // the color counter:
  if (inChar == ',') {
    // do something different for each value of currentColor:
    switch (currentColor) {
    case 0:    // 0 = red
      red = inString.toInt();
      // clear the string for new input:
      inString = ""; 
      break;
    case 1:    // 1 = green:
      green = inString.toInt();
      // clear the string for new input:
      inString = ""; 
      break;
    }
    currentColor++;
  }
  // if you get a newline, you know you've got
  // the last color, i.e. blue:
  if (inChar == '\n') {
    blue = inString.toInt();

    // set the levels of the LED.
    // subtract value from 255 because a higher
    // analogWrite level means a dimmer LED, since
    // you're raising the level on the anode:
    analogWrite(RledPin,  red);
    analogWrite(GledPin, green);
    analogWrite(BledPin, blue);

    // print the colors:
    //Serial.print("Red: ");
    //Serial.print(red);
    //Serial.print(", Green: ");
    //Serial.print(green);
    //Serial.print(", Blue: ");
    //Serial.println(blue);

    // clear the string for new input:
    inString = ""; 
    // reset the color counter:
    currentColor = 0;
  }




 // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:  

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
 
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        Serial.println("Button_pressed");
      }
    }
  }
   // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;
  
  
}














/*
Here's a Processing sketch that will draw a color wheel and send a serial
 string with the color you click on:
 
 // Subtractive Color Wheel with Serial
 // Based on a Processing example by Ira Greenberg. 
 // Serial output added by Tom Igoe
 // 
 // The primaries are red, yellow, and blue. The secondaries are green, 
 // purple, and orange. The tertiaries are  yellow-orange, red-orange, 
 // red-purple, blue-purple, blue-green, and yellow-green.
 // 
 // Create a shade or tint of the subtractive color wheel using
 // SHADE or TINT parameters.
 
 // Updated 29 November 2010.
 
 
 
 import processing.serial.*;
 
 int segs = 12;
 int steps = 6;
 float rotAdjust = TWO_PI / segs / 2;
 float radius;
 float segWidth;
 float interval = TWO_PI / segs;
 
 Serial myPort;
 
 void setup() {
 size(200, 200);
 background(127);
 smooth();
 ellipseMode(RADIUS);
 noStroke();
 // make the diameter 90% of the sketch area
 radius = min(width, height) * 0.45;
 segWidth = radius / steps;
 
 // swap which line is commented out to draw the other version
 // drawTintWheel();
 drawShadeWheel();
 // open the first serial port in your computer's list
 myPort = new Serial(this, Serial.list()[0], 9600);
 }
 
 
 void drawShadeWheel() {
 for (int j = 0; j < steps; j++) {
 color[] cols = { 
 color(255-(255/steps)*j, 255-(255/steps)*j, 0), 
 color(255-(255/steps)*j, (255/1.5)-((255/1.5)/steps)*j, 0), 
 color(255-(255/steps)*j, (255/2)-((255/2)/steps)*j, 0), 
 color(255-(255/steps)*j, (255/2.5)-((255/2.5)/steps)*j, 0), 
 color(255-(255/steps)*j, 0, 0), 
 color(255-(255/steps)*j, 0, (255/2)-((255/2)/steps)*j), 
 color(255-(255/steps)*j, 0, 255-(255/steps)*j), 
 color((255/2)-((255/2)/steps)*j, 0, 255-(255/steps)*j), 
 color(0, 0, 255-(255/steps)*j),
 color(0, 255-(255/steps)*j, (255/2.5)-((255/2.5)/steps)*j), 
 color(0, 255-(255/steps)*j, 0), 
 color((255/2)-((255/2)/steps)*j, 255-(255/steps)*j, 0)
 };
 for (int i = 0; i < segs; i++) {
 fill(cols[i]);
 arc(width/2, height/2, radius, radius, 
 interval*i+rotAdjust, interval*(i+1)+rotAdjust);
 }
 radius -= segWidth;
 }
 }
 
 
 void drawTintWheel() {
 for (int j = 0; j < steps; j++) {
 color[] cols = { 
 color((255/steps)*j, (255/steps)*j, 0), 
 color((255/steps)*j, ((255/1.5)/steps)*j, 0), 
 color((255/steps)*j, ((255/2)/steps)*j, 0), 
 color((255/steps)*j, ((255/2.5)/steps)*j, 0), 
 color((255/steps)*j, 0, 0), 
 color((255/steps)*j, 0, ((255/2)/steps)*j), 
 color((255/steps)*j, 0, (255/steps)*j), 
 color(((255/2)/steps)*j, 0, (255/steps)*j), 
 color(0, 0, (255/steps)*j),
 color(0, (255/steps)*j, ((255/2.5)/steps)*j), 
 color(0, (255/steps)*j, 0), 
 color(((255/2)/steps)*j, (255/steps)*j, 0)
 };
 for (int i = 0; i < segs; i++) {
 fill(cols[i]);
 arc(width/2, height/2, radius, radius, 
 interval*i+rotAdjust, interval*(i+1)+rotAdjust);
 }
 radius -= segWidth;
 }
 }
 
 void draw() {
 // nothing happens here
 }
 
 void mouseReleased() {
 // get the color of the mouse position's pixel:
 color targetColor = get(mouseX, mouseY);
 // get the component values:
 int r = int(red(targetColor));
 int g = int(green(targetColor));
 int b = int(blue(targetColor));
 // make a comma-separated string:
 String colorString = r + "," + g + "," + b + "\n";
 // send it out the serial port:
 myPort.write(colorString );
 }
 
*/










