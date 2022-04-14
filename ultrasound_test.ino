
// include the library code for LCD display
#include <LiquidCrystal.h>

//Pins
//ultrasonic: trig sends signal, echo receives signal, buzzer screams
const int trig = 7;
const int echo = 6;


const int led = 4; //gate
const int button = 2;
const int esu = 3; //power source from esu (through transistor)
//LCD pins
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

uint32_t rawValue;
float pressurediff;
float actualpressure;

/*The maximum range of the ultrasonic sensor is 400cm. 
 *The speed of sound at sea level and 68 F is 34300 cm/s, or 0.0343 cm/us
 *distance = speed * time / 2
 *distance (cm) = 0.0343 (cm/us) * pulse (us) / 2
 *400 cm = .0343 * pulse / 2
 *Solving for pulse yields pulse = ~ 23,300. This is the maximum pulse duration, corresponding to the sensor rrange of 400cm.
*/
const long MAXIMUM_DIST = 23300; 
int buttonState = 0;

void setup() {
  //Define input and output pins
  pinMode(trig, OUTPUT); 
  pinMode(led, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(button, INPUT);
  //Ensure that buzzer is off from the start, and there is no extra signal 
  digitalWrite(trig, LOW);
  digitalWrite(led,LOW);
  Serial.begin(9600);

  //set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
}
/*
void pulse_trig() {
  //Turn the ultrasonic sensor on for 10 microseconds, emitting a total of 8 ultrasonic pulses.
  //Pulses emitted are unique to the sensor, so multiple sensors can be used in same proximity.
  digitalWrite(trig,  HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,  LOW);
}

void loop() {
  //ULTRASONIC
  //Initialize variables that are used to calculate pulse length
  long time_1, time_2, pulse;
  float cm;
  //emit an ultrasonic pulse 
  pulse_trig(); 
  //Wait for the echo pin to turn HIGH
  while(digitalRead(echo)==0){
      delayMicroseconds(1);
  }
  // As soon as the echo pin goes HIGH, note the time
  time_1 = micros();
  while(digitalRead(echo)==1){
        delayMicroseconds(1);
  }
  // Note the time at which the echo pin goes low
  time_2 = micros();
  // time for the ultrasonic pulse to travel to the obstacle and back
  pulse = time_2 - time_1;
  cm = 0.0343 * pulse * 0.5;
*/
  //PRESSURE O2
  rawValue = analogRead(A0); //pressure pin
  float voltage = rawValue/1024.0*5.0;
  //Serial.println(voltage);
  pressurediff = ((voltage/5)-0.04)/0.009;
  actualpressure = 101.325-pressurediff;
  //Serial.println(rawValue);
  Serial.print("PRESSURE: ");
  Serial.println(actualpressure);

  //LCD CODE
  lcd.setCursor(0, 0);
  for (int thisChar = 0; thisChar < 10; thisChar++) {
    lcd.print(thisChar);
    delay(500);
  }

  // set the cursor to (16,1):
  lcd.setCursor(16, 1);
  // set the display to automatically scroll:
  lcd.autoscroll();
  // print from 0 to 9:
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
    //Serial.println(cm);
    digitalWrite(led, LOW);
  }
  else{
    //Serial.println(cm);
    digitalWrite(led, HIGH);
  }
  //Delay the next loop
  delay(300);
}
