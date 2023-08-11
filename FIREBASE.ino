#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>


FirebaseData firebaseData;

float randNumber1;
float randNumber2;
float randNumber3;
float randNumber4;
float randNumber5;
float randNumber6;
float randNumber7;
float randNumber8;
float randNumber9;
float randNumber10;

#define FIREBASE_HOST "https://hellby-a3676-default-rtdb.firebaseio.com/"  
#define FIREBASE_AUTH "Uoa0cyq3nxoFhFLmlzCUYJghx3onKenHuf5dvB4c"  
#define WIFI_SSID "VINIT1"
#define WIFI_PASSWORD "22223333"

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));

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
}

void loop() {

Serial.println("Sending_Data");

randNumber1 = random(10);
randNumber2 = random(100000);
randNumber3 = random(10);
randNumber4 = random(100);
randNumber5 = random(100);
randNumber6 = random(10);
randNumber7 = random(1000);
randNumber8 = random(10);
randNumber9 = random(10);
randNumber10 = random(100);

  Firebase.setFloat (firebaseData,"Smoke",randNumber1);
  Firebase.setFloat (firebaseData,"Cng",randNumber2);
  Firebase.setFloat (firebaseData,"Lpg",randNumber3);
  Firebase.setFloat (firebaseData,"Air_Quality",randNumber4);
  Firebase.setFloat (firebaseData,"Rain_Value",randNumber5);
  Firebase.setFloat (firebaseData,"Temperature",randNumber6);
  Firebase.setFloat (firebaseData,"Humidity",randNumber7);
  Firebase.setFloat (firebaseData,"Ldr",randNumber8);
  Firebase.setFloat (firebaseData,"Pressure",randNumber9);
  Firebase.setFloat (firebaseData,"Altitude",randNumber10);
  delay(10000);

  /*if(Firebase.failed())
{
  Serial.println("Firebase log sending failed");
  Serial.println(Firebase.error());
  return;
}
*/

}
