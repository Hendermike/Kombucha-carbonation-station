/*
DHT11

This example reads a DHT11 sensor hooked up on pin D7. Reads both
temperature and humidity and sends it to the Serial port

created in Feb 2019 by D. Cuartielles
based on work by F. Vanzati (2011) and M. Loglio (2013)

This example code is in the public domain.
*/
// include LCD I2C library
#include <LiquidCrystal_I2C.h>
// include SoftwareSerial for SIM800L interface 
#include <SoftwareSerial.h>

#include <DHT.h>

bool inRange0 = false;
bool inRange1 = false;
bool inRange2 = false;
bool inRange3 = false;
bool inRange4 = false;

int controlTolerance = 1;
int setPoint = 22;

DHT dht11_0 = DHT(A0, DHT11);  // creating the object sensor on pin 'D7'
DHT dht11_1 = DHT(A2, DHT22);
DHT dht11_2 = DHT(4, DHT11);
DHT dht11_3 = DHT(5, DHT11);
DHT dht11_4 = DHT(6, DHT11);

LiquidCrystal_I2C lcd(0x27,16,2); // creating LCD for 16 chars and 2 lines

//Create software serial object to communicate with SIM800L
SoftwareSerial mySerial(3, 2); //SIM800L Tx & Rx is connected to Arduino #3 & #2

int C0, C1, C2, C3, C4;   // temperature C readings are integers
int H0, H1, H2, H3, H4;   // humidity readings are integers
int HE0 = 7, HE1 = 8, HE2 = 9, HE3 = 10, HE4 = 11;

unsigned long referenceMillis = 0;
unsigned long SMSinterval = 60000; // intervalo de mensaje de 1 minuto

void setup()
{
  // initialize serial communications at 9600 bps
  Serial.begin(9600);
  Serial.println("Hola");
  // initialize LCD
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  // Print a message to the LCD.
  lcd.setCursor(0,0);
  lcd.print("Carb. station");
  lcd.setCursor(0,1);
  lcd.print("by hendermike");

  delay(1000);
  
  // Initialize relay pins
  pinMode(HE0, OUTPUT);
  digitalWrite(HE0, LOW);
  pinMode(HE1, OUTPUT);
  digitalWrite(HE1, LOW);
  pinMode(HE2, OUTPUT);
  digitalWrite(HE2, LOW);
  pinMode(HE3, OUTPUT);
  digitalWrite(HE3, LOW);
  pinMode(HE4, OUTPUT);
  digitalWrite(HE4, LOW);

  dht11_0.begin();
  delay(2500);
  dht11_1.begin();
  delay(2500);
  dht11_2.begin();
  delay(2500);
  dht11_3.begin();
  delay(2500);
  dht11_4.begin();
  delay(2500);
  // Initialize SIM800L module
  //Begin serial communication with Arduino and SIM800L
  mySerial.begin(9600);

  Serial.println("Initializing..."); 
}

void loop()
{
  
  C0 = dht11_0.readTemperature();       // Reading the temperature in Celsius degrees and store in the C variable
  delay(3000);
  C1 = dht11_1.readTemperature();
  delay(3000);
  C2 = dht11_2.readTemperature();
  delay(3000);
  C3 = dht11_3.readTemperature();
  delay(3000);
  C4 = dht11_4.readTemperature();
  delay(3000);
  //H0 = dht11_0.readHumidity();     // Reading the humidity index
  
  // Print the collected data in a row on the Serial Monitor
  Serial.print("C0: ");
  Serial.print(C0);
  Serial.print("\tC1: ");
  Serial.print(C1);
  Serial.print("\tC2: ");
  Serial.print(C2);
  Serial.print("\tC3: ");
  Serial.print(C3);
  Serial.print("\tC4: ");
  Serial.println(C4);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("A:" + String(C0) + ",B:" + String(C1) + ",C:" + String(C2));
  lcd.setCursor(0,1);
  lcd.print("D:" + String(C3) + ",E:" + String(C4) + " R:" + String(setPoint));

  if (C0*C1*C2*C3*C4 != 0) {
  
    if (C0 >= setPoint + controlTolerance) {digitalWrite(HE0, HIGH);} else if (C0 < setPoint) {digitalWrite(HE0, LOW);}
    if (C1 >= setPoint + controlTolerance) {digitalWrite(HE1, HIGH);} else if (C1 < setPoint) {digitalWrite(HE1, LOW);}
    if (C2 >= setPoint + controlTolerance) {digitalWrite(HE2, HIGH);} else if (C2 < setPoint) {digitalWrite(HE2, LOW);}
    if (C3 >= setPoint + controlTolerance) {digitalWrite(HE3, HIGH);} else if (C3 < setPoint) {digitalWrite(HE3, LOW);}
    if (C4 >= setPoint + controlTolerance) {digitalWrite(HE4, HIGH);} else if (C4 < setPoint) {digitalWrite(HE4, LOW);}

  } else {

    digitalWrite(HE0, HIGH);
    digitalWrite(HE1, HIGH);
    digitalWrite(HE2, HIGH);
    digitalWrite(HE3, HIGH);
    digitalWrite(HE4, HIGH);//HIGH es apagado;

    //delay(5000);
    
  }
  /*if(millis() - referenceMillis > SMSinterval) {
    sendSMSUpdate();
    referenceMillis = millis();
  }*/
}

/*void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}

void sendSMSUpdate(void) {
  //Send text message to specified number
  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CMGS=\"+56991983186\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  mySerial.print("Last Minute Engineers | lastminuteengineers.com"); //text content
  updateSerial();
  mySerial.write(26); // Equivalente a enviar Ctrl+Z 
}*/
