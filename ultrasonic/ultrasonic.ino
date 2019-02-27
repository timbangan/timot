#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "timol-4a35f.firebaseio.com"
#define FIREBASE_AUTH "7nX2G0C50nagJ1pstQM7dbxAZnyNlMdpDSlOgAG3"
#define WIFI_SSID "hacktiv8-guest" // hacktiv
#define WIFI_PASSWORD "helloworld" // hacktiv
//#define WIFI_SSID "Snoopy"
//#define WIFI_PASSWORD "asd130997"

#define _trigger D0
#define _echo D1

long duration;
long distance;
int height = 0;
String user;
int maxHeight = 178;

void setup() {
pinMode(_trigger, OUTPUT); // Sets the trigPin as an Output
pinMode(_echo, INPUT); // Sets the echoPin as an Input
Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  postFirebase(0);
  Serial.println("Preparing device");
  for (int i = 0; i <= 10; i++) {
    getHeight();
    Serial.print(".");
//    getHeight();
    delay(70);
  }
  delay(1000);
}

void loop() {
  getHeight();
  if (height == 0) {
    height = getHeight();
  }
//  postFirebase(height-getHeight());
  if ( height - getHeight() < 200 && height - getHeight() > 20 ) {
//    Serial.print(getHeight());
//    Serial.print(" - ");
//    Serial.print(height);
//    Serial.print(" = ");
//    Serial.println(height-getHeight());
  }
//  delay(00);
}

void postFirebase(int height) {
  Firebase.setFloat("Timbangans/9/height", height);
}

void getCurrentUser() {
  Serial.println(Firebase.getString("Timbangans/9/currentUser"));
}

int getHeight() {
//  digitalWrite(_trigger, LOW);
//  delayMicroseconds(2);
//  digitalWrite(_trigger, HIGH);
//  delayMicroseconds(10);
//  digitalWrite(_trigger, LOW);
//  duration = pulseIn(_echo, HIGH);
//  distance= duration*0.034/2;
//  return distance;

//long duration, distance;
digitalWrite(_trigger, LOW);
delayMicroseconds(2);
digitalWrite(_trigger, HIGH);
delayMicroseconds(10);
digitalWrite(_trigger, LOW);
duration = pulseIn(_echo, HIGH);
distance = maxHeight - (duration/2) / 29.1; 
if (distance < maxHeight && distance > -3) {
  Serial.println(distance);
}
if (distance > 20 && distance < maxHeight) {
  postFirebase(distance + 10);
}
else if(distance < 20) {
  postFirebase(0);
}
return distance;
}
