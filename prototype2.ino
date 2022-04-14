
// include the library code for LCD display
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

//Pins
const int led = 4; //gate
const int button = 2;
const int esu = 3; //power source from esu (through transistor)
//LCD pins
const int rs = 5, en = 6, d4 = 7, d5 = 8, d6 = 9, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
SoftwareSerial mySerial(10, 11); // RX (O2 Pin 3), TX (O2 Pin 4)

uint32_t rawValue;
float pressurediff;
String pressure;

const long MAXIMUM_DIST = 23300; 
int buttonState = 0;


void setup() {
  //Define input and output pins
 
  pinMode(led, OUTPUT);
  
  pinMode(button, INPUT);
  digitalWrite(led, LOW);
  Serial.begin(9600);
  mySerial.begin(9600); //for O2 sensor
  Serial.write("hi new\n");

  //set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.print("hello, world!");
}

void loop() {
  //PRESSURE, O2
  if (mySerial.available()) {
    String output = mySerial.readStringUntil('\n');
    String val = output.substring(3,8);
    char* oxygen = (char*) malloc(sizeof(char)*(val.length() + 1));
    val.toCharArray(oxygen, val.length() + 1);
    Serial.write("in loop\n");
    Serial.print(oxygen);
    Serial.write("after o2\n");

    Serial.write('\n');

    String o2(oxygen);
    Serial.print(o2);

   
  /*if (mySerial.available()) {
    Serial.write(mySerial.read());
  }

  rawValue = analogRead(A0); //pressure pin
  float voltage = rawValue/1024.0*5.0;
  //Serial.println(voltage);
  pressurediff = ((voltage/5)-0.04)/0.009;
  actualpressure = 101.325-pressurediff;
  //Serial.println(rawValue);
  Serial.print("PRESSURE: ");
  Serial.println(actualpressure);*/


  /*//LCD CODE
  // set the cursor to (16,1):
  lcd.setCursor(16, 1);
  // set the display to automatically scroll:
  lcd.autoscroll();
  // print pressure
  lcd.print(pressure);
  delay(500); */

  
  //BUTTON
  //Read the state of the button
  buttonState = digitalRead(button);
  //Serial.println(buttonState);
  if (buttonState == 1) {
    //turn esu voltage on:
    digitalWrite(esu, HIGH);
  } else {
    digitalWrite(esu, LOW);
  }
  
  //SHUTOFF
  
  //If pressure < 0, turn off gate voltage
  if (o2.toFloat() < 100.0){
    digitalWrite(led, LOW);
  } else{
    digitalWrite(led, HIGH);
  }
  delay(300);
}
}
