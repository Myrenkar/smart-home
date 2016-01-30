#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
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
} ;


void setup() {
  Configuration parameters = {8, 20, 1, 31, 0, 0};
  EEPROM_writeAnything(0, parameters);
  
  Serial.begin(115200);
  gpsSerial.begin(9600);
  //set active leds to be used to alarm
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  // put off the leds
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);

  //    EEPROM_readAnything(0, Configuration);
  Serial.print(F("Testing TinyGPS++ library v. \n\n")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println("Dostepne opcje: \n1. Wlacz alarm w punkcie nr(1-4): alarm 'nr'\n2. Wylacz wszystkie alarmy: noalarm\n3. Ustaw godzine rozpoczecia: gstart 'godzina' \n4. Ustaw godzine zakonczenia: gstop 'godzina'\n5. Ustaw dzien dzien tygodnia, od ktorego alarm dziala: daystart: 'nr_dnia'\n6. Ustaw dzien dzien tygodnia, do ktorego alarm dziala: daystop: 'nr_dnia'\n7. Ustaw minimalny czas wlaczenia obwodu: omin 'czas'\n8. Ustaw maksymalny czas wlaczenia obwodu: omax 'czas'\n9. Wyswietl menu: menu ");
}

void loop() {
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
void activateAlarm() {
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
  // sprawdzamy czy są nowe dane
  if (Serial.available() > 0)
  {
    //wykonujemy pętle do momentu w którym są dane
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
Configuration parameters;
  
  if (komenda.substring(0, 4) == "menu") {
    Serial.println("Dostepne opcje: \n1. Wlacz alarm w punkcie nr(1-4): alarm 'nr'\n2. Wylacz wszystkie alarmy: noalarm\n3. Ustaw godzine rozpoczecia: gstart 'godzina' \n4. Ustaw godzine zakonczenia: gstop 'godzina'\n5. Ustaw dzien dzien tygodnia, od ktorego alarm dziala: daystart: 'nr_dnia'\n6. Ustaw dzien dzien tygodnia, do ktorego alarm dziala: daystop: 'nr_dnia'\n7. Ustaw minimalny czas wlaczenia obwodu: omin 'czas'\n8. Ustaw maksymalny czas wlaczenia obwodu: omax 'czas'\n9. Wyswietl menu: menu ");

  }
  else if (komenda.substring(0, 5) == "alarm") {
    //    getTimeFromGPS(gps.time);
  }
  else if (komenda.substring(0, 7) == "noalarm") {
    deactivateAlarm();
  }
  else if (komenda.substring(0, 6) == "gstart") {
    int n = komenda.length() - 7;

    parameters.gStart = n;
    EEPROM_writeAnything(0, parameters);
  }
  else if (komenda.substring(0, 5) == "gstop") {
    int n = komenda.length() - 6;
    parameters.gStop = n;
    EEPROM_writeAnything(0, parameters);
  }
  else if (komenda.substring(0, 4) == "omin") {
    int n = komenda.length() - 5;
    parameters.oMin = n;
    EEPROM_writeAnything(0, parameters);
  }
  else if (komenda.substring(0, 4) == "omax") {
  int n = komenda.length() - 5;
    parameters.oMax = n;
    EEPROM_writeAnything(0, parameters);
  }
  else if (komenda.substring(0, 8) == "daystart") {
    int n = komenda.length() - 8;
    char array[n];
    komenda.substring(6).toCharArray(array, n);
    EEPROM.write(0, n);
    for (int i = 1; i < n + 1; i++)
      EEPROM.write(i, (int)array[i - 1]);
  }
  else if (komenda.substring(0, 7) == "daystop") {
   EEPROM_readAnything(0, parameters);
   Serial.println("kupa");
   Serial.println(parameters.gStart);
   Serial.println(parameters.gStop);
   Serial.println(parameters.dStart);
    Serial.println(parameters.dStop);
  }

}

static void getDateFromGPS(TinyGPSDate &d) {
  if (!d.isValid())
  {
    Serial.print("Invalid date\n");
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
    Serial.println(sz);
  }
  smartDelay(100);
}

static void getTimeFromGPS(TinyGPSTime &t) {
  if (!t.isValid())
  {
    Serial.print("Invalid time\n");
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
    Serial.println(sz);
  }
  smartDelay(100);
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

