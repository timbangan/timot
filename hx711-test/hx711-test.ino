#include "HX711.h"
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "timol-4a35f.firebaseio.com"
#define FIREBASE_AUTH "7nX2G0C50nagJ1pstQM7dbxAZnyNlMdpDSlOgAG3"
#define WIFI_SSID "hacktiv8-guest" // hacktiv
#define WIFI_PASSWORD "helloworld" // hacktiv
//#define WIFI_SSID "Snoopy"
//#define WIFI_PASSWORD "asd130997"

#define DOUT  D0
#define CLK  D1
#define _LED D2 

int tmpScale = 0;

HX711 scale(DOUT, CLK);
float calibration_factor = -7050; 
void setup() {
  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(_LED, HIGH);
    Serial.print(".");
    delay(500);
    digitalWrite(_LED, LOW);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  scale.set_scale();
  scale.tare(); 
  long zero_factor = scale.read_average();
  Serial.print(zero_factor);
  Serial.print(" ");
  Serial.print("zero_factor");
  digitalWrite(_LED, LOW);
}
void loop() {
  delay(100);
  Serial.println(getScale()/3);

  if (getScale() > 1 ) {
    digitalWrite(_LED, HIGH);
    delay(200);
    Serial.println("post to firebase");
    postFirebase(getScale()/3);
      
    if (Firebase.failed()) {
      Serial.print("error post firebase");
      Serial.println(Firebase.error());  
      return;
    }
  }

  if (getScale() < 1) {
    digitalWrite(_LED, LOW);
    postFirebase(0); 
  }
  
  
  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
      calibration_factor += 10;
    else if(temp == '-' || temp == 'z')
      calibration_factor -= 10;
  }
}

void postFirebase(int scale) {
  Firebase.setFloat("Timbangans/9/value", scale);
} 

float getScale() {
  scale.set_scale(calibration_factor);
  if ((scale.get_units() > 0.1 && scale.get_units() < 0.5) || scale.get_units() < -0.1) {
    scale.tare();    
  }
  return scale.get_units();
}
