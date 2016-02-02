#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <C:\Users\Tomek\Documents\GitHub\smart-home\projekt\TinyGPS++.h>
#include "EEPROMAnything.h"

//boleans
bool isAlarmActivated = true;
boolean koniec_odczytu = false;

//Strings
String odczyt_serial = "";

//ints
int y = 0;

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

uint8_t dateDay;

void setup() {
  parameters = {8, 20, 1, 31, 0, 0};
  EEPROM_writeAnything(0, parameters);

  Serial.begin(115200);
  gpsSerial.begin(9600);


  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  // put off the leds
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
 

  Serial.print(F("Testing TinyGPS++ library v. \n\n")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println("Dostepne opcje: \n1. Wlacz alarm w punkcie nr(1-4): alarm 'nr'\n2. Wylacz wszystkie alarmy: noalarm\n3. Ustaw godzine rozpoczecia: gstart 'godzina' \n4. Ustaw godzine zakonczenia: gstop 'godzina'\n5. Ustaw dzien dzien tygodnia, od ktorego alarm dziala: daystart: 'nr_dnia'\n6. Ustaw dzien dzien tygodnia, do ktorego alarm dziala: daystop: 'nr_dnia'\n7. Ustaw minimalny czas wlaczenia obwodu: omin 'czas'\n8. Ustaw maksymalny czas wlaczenia obwodu: omax 'czas'\n9. Wyswietl menu: menu ");
}

void loop() {
  
    getDateFromGPS(gps.date);
  dateDay = gps.date.month();
  runSerialPort();

}

//------------------------------------------------------------------------
void blinkLeds() {
  if (isAlarmActivated) {
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);

    delay(700);

    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);

    delay(700);
  }
}

//------------------------------------------------------------------------
void activateAlarm(int stationNumber) {
  isAlarmActivated = true;

}

//------------------------------------------------------------------------
void deactivateAlarm() {
  isAlarmActivated = false;

  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
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
    komenda = komenda.substring(7);
    int n = komenda.toInt();
    dateDay = gps.date.month();
    //get
    Serial.println(dateDay);
    getTimeFromGPS(gps.time);
  }
  else if (komenda.substring(0, 7) == "noalarm") {

    //    deactivateAlarm();
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
    Serial.print("Invalid date\n");
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

  Configuration dateParams;
  EEPROM_readAnything(0, dateParams);

  if(getDateFromGPS(gps.date) < dataParams.dStart && getDateFromGPS(gps.date) > dataParams.dStop && HourFromGPS(gps.time) < dataParams.gStart && getHourFromGPS(gps.time) > dataParams.gStop) {
    validation = true;
  }
  else {
    validation = false;
  }  
  return validation;
}

}

