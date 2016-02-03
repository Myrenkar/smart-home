#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include "EEPROMAnything.h"

#define Diod1 5
#define Diod2 6
#define Diod3 7
#define Diod4 8
//boleans
bool isAlarmActivated = false;
boolean koniec_odczytu = false;

//Strings
String odczyt_serial = "";

//ints
int y = 0;

int sensorVal1;
int sensorVal2 ;
int sensorVal3 ;
int sensorVal4 ;
int diodNumber;
//files
TinyGPSPlus gps;
SoftwareSerial gpsSerial(4, 3);

struct Configuration
{
  int gStart ;
  int gStop ;
  int dStart;
  int dStop ;
  int oMin;
  int oMax;
};

Configuration parameters;


void setup() {
  parameters = {8, 20, 1, 31, 0, 0};
  EEPROM_writeAnything(0, parameters);

  Serial.begin(115200);
  gpsSerial.begin(9600);


  pinMode(Diod1, OUTPUT);
  pinMode(Diod2, OUTPUT);
  pinMode(Diod3, OUTPUT);
  pinMode(Diod4, OUTPUT);

  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);

  // put off the leds
  digitalWrite(Diod1, HIGH);
  digitalWrite(Diod2, HIGH);
  digitalWrite(Diod3, HIGH);
  digitalWrite(Diod4, HIGH);


  Serial.print(F("Testing TinyGPS++ library v. \n\n")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println("Dostepne opcje: \n1. Wlacz alarm w punkcie nr(1-4): alarm 'nr'\n2. Wylacz wszystkie alarmy: noalarm\n3. Ustaw godzine rozpoczecia: gstart 'godzina' \n4. Ustaw godzine zakonczenia: gstop 'godzina'\n5. Ustaw dzien dzien tygodnia, od ktorego alarm dziala: daystart: 'nr_dnia'\n6. Ustaw dzien dzien tygodnia, do ktorego alarm dziala: daystop: 'nr_dnia'\n7. Ustaw minimalny czas wlaczenia obwodu: omin 'czas'\n8. Ustaw maksymalny czas wlaczenia obwodu: omax 'czas'\n9. Wyswietl menu: menu ");
}

void loop() {

  sensorVal1 = digitalRead(9);
  sensorVal2 = digitalRead(10);
  sensorVal3 = digitalRead(11);
  sensorVal4 = digitalRead(12);

 activateAlarm();
  runSerialPort();

}

//------------------------------------------------------------------------
void blinkLeds() {
  int diod = 0;
  if (diodNumber == 1)
  {
    diod = Diod1;
  }
  else  if (diodNumber == 2)
  {
    diod = Diod2;
  }
  else if (diodNumber == 3)
  {
    diod = Diod3;
  }
  if (diodNumber == 4)
  {
    diod = Diod4;
  }
  if (isAlarmActivated) {
    digitalWrite(diod, LOW);
  }


}

//------------------------------------------------------------------------
void activateAlarm() {
  int sensor;
  if (diodNumber == 1)
  {
    sensor = sensorVal1;
  }
  else if (diodNumber == 2)
  {
    sensor = sensorVal2;
  }
  else if (diodNumber == 3)
  {
    sensor = sensorVal3;
  }
  else if (diodNumber == 4)
  {
    sensor = sensorVal4;
  }
  
  if (isAlarmActivated && !isValidDateTime() && sensor == LOW) {
    blinkLeds();
  }

}

//------------------------------------------------------------------------
void deactivateAlarm() {
  if (isAlarmActivated == false) {

    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);

  }
}
//------------------------------------------------------------------------
void runSerialPort() {
  // sprawdzamy czy sÄ… nowe dane
  if (Serial.available() > 0)
  {
    //wykonujemy pÄ™tle do momentu w ktĂłrym sÄ… dane
    while (Serial.available())
    {
      char znak = (char)Serial.read();
      if (znak == '\n') {
        koniec_odczytu = true;
      }
      else {
        odczyt_serial += znak;
      }
    }

    if (koniec_odczytu == true)
    {
      odczyt(odczyt_serial);
      koniec_odczytu = false;
      odczyt_serial = "";
    }
  }
}

//-----------------------------------------------------------------------
void odczyt(String komenda) {
  //  Configuration parameters;
  //Configuration reads;
  if (komenda.substring(0, 4) == "menu") {
    Serial.println("Dostepne opcje: \n1. Wlacz alarm w punkcie nr(1-4): alarm 'nr'\n2. Wylacz wszystkie alarmy: noalarm\n3. Ustaw godzine rozpoczecia: gstart 'godzina' \n4. Ustaw godzine zakonczenia: gstop 'godzina'\n5. Ustaw dzien dzien tygodnia, od ktorego alarm dziala: daystart: 'nr_dnia'\n6. Ustaw dzien dzien tygodnia, do ktorego alarm dziala: daystop: 'nr_dnia'\n7. Ustaw minimalny czas wlaczenia obwodu: omin 'czas'\n8. Ustaw maksymalny czas wlaczenia obwodu: omax 'czas'\n9. Wyswietl menu: menu ");

  }
  else if (komenda.substring(0, 5) == "alarm") {
    
    komenda = komenda.substring(6);
    int n = komenda.toInt();
    isAlarmActivated = true;
    Serial.println(n);
    diodNumber  = n;
  }
  else if (komenda.substring(0, 7) == "noalarm") {
    isAlarmActivated = false;
    deactivateAlarm();
  }
  else if (komenda.substring(0, 6) == "gstart") {
    komenda = komenda.substring(7);
    int n = komenda.toInt();
    Serial.println(n);
    parameters.gStart = n;
    EEPROM_writeAnything(0, parameters);
  }
  else if (komenda.substring(0, 5) == "gstop") {
    komenda = komenda.substring(6);
    int n = komenda.toInt();
    Serial.println(n);
    parameters.gStop = n;
    EEPROM_writeAnything(0, parameters);
  }
  else if (komenda.substring(0, 4) == "omin") {
    komenda = komenda.substring(5);
    int n = komenda.toInt();
    Serial.println(n);

    parameters.oMin = n;
    EEPROM_writeAnything(0, parameters);
  }
  else if (komenda.substring(0, 4) == "omax") {
    komenda = komenda.substring(5);
    int n = komenda.toInt();
    Serial.println(n);

    parameters.oMax = n;
    EEPROM_writeAnything(0, parameters);
  }
  else if (komenda.substring(0, 8) == "daystart") {
    komenda = komenda.substring(9);
    int n = komenda.toInt();
    Serial.println(n);

    parameters.dStart = n;
    EEPROM_writeAnything(0, parameters);
  }
  else if (komenda.substring(0, 7) == "daystop") {
    komenda = komenda.substring(8);
    int n = komenda.toInt();
    Serial.println(n);

    parameters.dStop = n;
    EEPROM_writeAnything(0, parameters);

  }
  else if (komenda.substring(0, 4) == "test") {
    EEPROM_readAnything(0, parameters);
    Serial.println(parameters.gStart);
    Serial.println(parameters.gStop);
    Serial.println(parameters.dStart);
    Serial.println(parameters.dStop);
    Serial.println(parameters.oMin);
    Serial.println(parameters.oMax);
  }
}
static int getDateFromGPS(TinyGPSDate &d) {
  int day;
  if (!d.isValid())
  {
    day = 0;
  }
  else
  {
    day = d.day();
  }
  smartDelay(100);
  return day;
}

static int getHourFromGPS(TinyGPSTime &t) {
  int hour;
  if (!t.isValid())
  {
    Serial.print("Invalid time\n");
    hour = -1;
  }
  else
  {
    hour = t.hour();
  }
  smartDelay(100);
  return hour;
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (gpsSerial.available())
      gps.encode(gpsSerial.read());
  } while (millis() - start < ms);
}

bool isValidDateTime() {
  bool validation;

  Configuration dataParams;
  EEPROM_readAnything(0, dataParams);

  if (getDateFromGPS(gps.date) < dataParams.dStart && getDateFromGPS(gps.date) > dataParams.dStop && getHourFromGPS(gps.time) < dataParams.gStart && getHourFromGPS(gps.time) > dataParams.gStop) {
    validation = true;
  }
  else {
    validation = false;
  }
  return validation;
}

