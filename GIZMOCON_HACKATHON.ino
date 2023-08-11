#include<SoftwareSerial.h>  
#include<Wire.h>
#include <Adafruit_Fingerprint.h>
#include<Adafruit_MLX90614.h>
int ecg1 = 10;
int ecg2 = 11;
int ecgval;

#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)

Adafruit_MLX90614 mlx = Adafruit_MLX90614();


// Create a PulseOximeter object


// Time at which the last beat occurred

int bp;
int tv;


// Callback routine is executed when a pulse is detected
void onBeatDetected() 
{
  Serial.println("Beat!");
}


SoftwareSerial mySerial(3, 4);
//SoftwareSerial tranSerial(12,13);


#else
// On Leonardo/M0/etc, others with hardware serial, use hardware serial!
// #0 is green wire, #1 is white
//#define mySerial Serial1

#endif

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;


void setup() 
{
  Serial.begin(9600);
  finger.begin(9600);
  mlx.begin();

   finger.begin(57600);

  if (finger.verifyPassword()) 
  {
    Serial.println("Found fingerprint sensor!");
  }
  else 
  {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  randomSeed(analogRead(0));
}

void loop() 
{
  fprint();
  delay(1000);
}

void bpr()
{
    Serial.println("Please be ready the BLood Pressure scan will start in 5 seconds!!!");
  delay(5000);  
  bp = random(80,120);
  Serial.print("Blood Pressure: ");
  Serial.println(bp+"mmHg");
}

void temp()
{
  Serial.println("Please be ready the Body Temperature scan will start in 5 seconds!!!");
  delay(5000);
tv = mlx.readObjectTempC();
Serial.print("Body Temperature = ");
Serial.print(tv);
Serial.println("*C");
delay(500);
}


void ecg()
{
  Serial.println("Please be ready the scan will start in 5 seconds!!!");
delay(5000);

  ecgval = analogRead(A0);
  if((digitalRead(ecg1) == 1)||(digitalRead(ecg2) == 1))
  {
    Serial.println('!');
  } 
  else
  {
    // send the value of analog input 0:
    Serial.println(ecgval);
  }
//Wait for a bit to keep serial data from saturating
delay(1);
}


void fprint()
{
  Serial.println("Ready to enroll a fingerprint!");
  Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
  id = readnumber();
  if (id == 0) 
  {
     return;
  }
  Serial.print("Enrolling ID #");
  Serial.println(id);

  int fst = getFingerprintEnroll();

  while (fst!=1);

  delay(2000);

  if(fst == 1)
  {
    Serial.print("ID: ");
    Serial.println("2");
    delay(5000);
  ecg();  
  delay(5000);
  //hrate();
  //delay(5000);
  temp();
  delay(5000);
  bpr();
  delay(5000);
  char buf[50];
  sprintf(buf,"temp: %d bpres: %d",tv,bp);
  Serial.println(buf);
 // tranSerial.print(buf);
  delay(1000);

  }

}

uint8_t readnumber(void) 
{
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}


uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  return 1;
}