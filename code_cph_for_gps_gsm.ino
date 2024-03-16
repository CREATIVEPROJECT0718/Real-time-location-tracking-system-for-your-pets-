#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial GPS(RXPin, TXPin);

String Phone="+919886502738";

void setup() 
{
  Serial.begin(115200);
  GPS.begin(GPSBaud);
  Serial.println("Goodnight moon!");
  delay(5000);
  Serial.println("AT");
  UpdateSerial();
  Serial.println("AT+CPMS=\"SM\",\"SM\",\"SM\"");  // Store SMS in Simcard
  delay(500);
  UpdateSerial();
  Serial.println("AT+CMGD=4,4");     // Delete all sms
  delay(500);
  UpdateSerial();
  Serial.println("AT+CPMS?");        // Check sms store location
  delay(500); 
  UpdateSerial();
  Serial.println("AT+CMGF=1");       // Text mode
  delay(500); 
  UpdateSerial();
  Serial.print ("AT+CSMP=17,167,0,0\r"); // Configuring TEXT mode
  delay(500);
  Serial.println("AT+CSQ");          // Check Network signal level (0-31)
  delay(500); 
  UpdateSerial();
  Reply("https://maps.app.goo.gl/ey8d3rDBfNBzuZ9VA",Phone);
}


void loop() 
{ 
while (GPS.available() > 0){
    gps.encode(GPS.read());
    if (gps.location.isUpdated()){
      Serial.print("Latitude= "); 
      Serial.print(gps.location.lat(), 6);
      Serial.print(" Longitude= "); 
      Serial.println(gps.location.lng(), 6);
    }
  }

while(Serial.available())    { parseData(Serial.readString());  }
while (Serial.available())    { Serial.write(Serial.read());     }
}

void parseData(String buff)
{
  Serial.println(buff);
}



void UpdateSerial()
{
   while (Serial.available())  Serial.write(Serial.read());
   while (Serial.available())   Serial.write(Serial.read());
}


void Reply(String text, String Phone)
{
    Serial.print("AT+CMGF=1\r");
    delay(1000);
    Serial.print ("AT+CSMP=17,167,0,0\r");// Configuring TEXT mode
    delay(1000);
    Serial.print("AT+CMGS=\""+Phone+"\"\r");
    delay(1000);
    Serial.print(text);
    delay(100);
    Serial.write(0x1A); //ascii code for ctrl-26 //Serial1.println((char)26); //ascii code for ctrl-26
    delay(1000);
    Serial.println("SMS Sent Successfully.");
    UpdateSerial();
    delay(2000);
    Serial.println("AT+CMGD=4,4");
    delay(1000);
    UpdateSerial();
}