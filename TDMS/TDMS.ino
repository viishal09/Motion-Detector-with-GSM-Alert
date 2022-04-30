
/*
 *Connect 5VT to D9 and 5VR to D10
 *Feed GSM SIM900A with Arduino's 5V  
 * */
#include <SoftwareSerial.h>

int ledPin = 8;                // choose the pin for the LED
int inputPin = 2;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;  
//int sms_count=0; // variable for reading the pin status
int buzz = 11;                  // buzzer pin at 11
SoftwareSerial mySerial(9, 10);
//char msg;

void setup() {
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
  pinMode(buzz, OUTPUT);
}

void loop(){
  val = digitalRead(inputPin);  // read input value
  if (val == HIGH)  // check if the input is HIGH
  {            
    digitalWrite(ledPin, HIGH);  // turn LED ON
    digitalWrite(buzz, HIGH); //ACTIVATE BUZZER
    if (pirState == LOW)    // we have just turned on
    {
      Serial.println("Motion detected!\r\n\n");
      Serial.println("Alerting Admin/Owner \r\n\n");
      mySerial.println("AT+CMGF=1");    //To send SMS in Text Mode
      delay(500);
      mySerial.println("AT+CMGS=\"Your Phone Number\"\r"); // change to the phone number you using 
      delay(500);
      mySerial.println("Someone is Trying to Break IN \r\n\n");//the content of the message
      delay(200);
      mySerial.println("Succesfully Sent Message from GSM Module \r\n");//the stopping character
      delay(800);      
      // We only want to print on the output change, not state
      pirState = HIGH; 
    }
  } 
  else 
  {
    digitalWrite(ledPin, LOW); // turn LED OFF
    digitalWrite(buzz, LOW);  // turn off buzzer
    if (pirState == HIGH)
    {
      // we have just turned of
      Serial.println("Motion ended! \r\n\n");
      mySerial.println("AT+CNMI=2,2,0,0,0\r\n\n"); // AT Command to recieve a live SMS
      mySerial.println("Message Sent"); // AT Command to recieve a live SMS
      delay(1000);
      // We only want to print on the output change, not state
      pirState = LOW; 
    }
  }
}
