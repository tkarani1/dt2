
// include the library code for LCD display
#include <LiquidCrystal.h>

//Pins
const int led = 4; //gate
const int button = 2;
const int esu = 3; //power source from esu (through transistor)
//LCD pins
const int rs = 5, en = 6, d4 = 7, d5 = 8, d6 = 9, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

uint32_t rawValue;
float pressurediff;
float actualpressure;

const long MAXIMUM_DIST = 23300; 
int buttonState = 0;

void setup() {
  //Define input and output pins
 
  pinMode(led, OUTPUT);
  
  pinMode(button, INPUT);
  digitalWrite(led, LOW);
  Serial.begin(9600);

  //set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.print("hello, world!");
}

void loop() {
  //PRESSURE, O2
  rawValue = analogRead(A0); //pressure pin
  float voltage = rawValue/1024.0*5.0;
  //Serial.println(voltage);
  pressurediff = ((voltage/5)-0.04)/0.009;
  actualpressure = 101.325-pressurediff;
  //Serial.println(rawValue);
  Serial.print("PRESSURE: ");
  Serial.println(actualpressure);

  //LCD CODE
  // set the cursor to (16,1):
  lcd.setCursor(16, 1);
  // set the display to automatically scroll:
  lcd.autoscroll();
  // print pressure
  lcd.print(actualpressure);
  delay(500); 

  
  //BUTTON
  //Read the state of the button
  buttonState = digitalRead(button);
  Serial.println(buttonState);
  if (buttonState == 1) {
    //turn esu voltage on:
    digitalWrite(esu, HIGH);
  } else {
    digitalWrite(esu, LOW);
  }
  
  //SHUTOFF
  
  //If pressure < 0, turn off gate voltage
  if (actualpressure < 0){
    digitalWrite(led, LOW);
  } else{
    digitalWrite(led, HIGH);
  }
  delay(300);
}
