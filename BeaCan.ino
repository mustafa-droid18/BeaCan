#include "HX711.h"

// HX711.DOUT	- pin #A1
// HX711.PD_SCK	- pin #A0
int trigPin=7; /*pin Connection */
int echopin=6; /*Pin Connection*/
long duration;
int distanceInch;
int pirPin = 8;
int val= 0;
int RELAY_PIN= 9;// Relay connected to the Motor controlling the wheels
int RELAY_PIN1= 10;// Relay Connected to the Motor Controlling the Conveyor
HX711 scale(A1, A0);		// parameter "gain" is ommited; the default value 128 is used by the library

void setup() {
  Serial.begin(38400);
  pinMode(pirPin, INPUT);
  pinMode(RELAY_PIN,OUTPUT);
  pinMode(RELAY_PIN1,OUTPUT);
  pinMode(trigPin,OUTPUT); 
  pinMode(echopin,INPUT);
  
  Serial.println("HX711 Trial");

  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read());			// print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));  	// print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));		// print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);	// print the average of 5 readings from the ADC minus tare weight (not set) divided 
						// by the SCALE parameter (not set yet)  

  scale.set_scale(2280.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();				        // reset the scale to 0

  Serial.println("After setting up the scale:");

  Serial.print("read: \t\t");
  Serial.println(scale.read());                 // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));		// print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided 
						// by the SCALE parameter set with set_scale
}

void loop() 
{
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPin,LOW);
  duration = pulseIn(echopin,HIGH);
  distanceInch = (duration/74)/2;
  
  if(distanceInch>=1 && distanceInch<=5)
  {
    digitalWrite(RELAY_PIN,LOW); // This part will be replaced by an updation in the route
  }
  else
  {
    digitalWrite(RELAY_PIN,HIGH);
  }
  float units = scale.get_units(10);
  if(units>=1000000)
  {
    digitalWrite(RELAY_PIN1,LOW);// This Section will changed by protocol wherein the bot will return to the docking place
  }
  else
  {
    digitalWrite(RELAY_PIN1,HIGH);
  } 
  val= digitalRead(pirPin);
  if(val== HIGH)
  {
      digitalWrite(RELAY_PIN, LOW);
  }
  else
  {
    digitalWrite(RELAY_PIN,HIGH);
  }
  scale.power_down();			        // put the ADC in sleep mode
  delay(1000);
  scale.power_up();
  delay(500);
}
