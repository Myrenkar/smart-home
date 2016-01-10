#include <LiquidCrystal.h>
#include <EEPROM.h>

//boleans
bool isAlarmActivated = true;
boolean koniec_odczytu = false;

//Strings
String odczyt_serial = "";

//ints
int y = 0;

//files
File GPSFile;

void setup() {
  //set active leds to be used to alarm
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  // put off the leds
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);

  //enable serial port work
  Serial.begin(9600);
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

  if (komenda.substring(0, 4) == "menu") {
    Serial.println("Dostepne opcje: \n1. Wlacz alarm w punkcie nr(1-4): alarm 'nr'\n2. Wylacz wszystkie alarmy: noalarm\n3. Ustaw godzine rozpoczecia: gstart 'godzina' \n4. Ustaw godzine zakonczenia: gstop 'godzina'\n5. Ustaw dzien dzien tygodnia, od ktorego alarm dziala: daystart: 'nr_dnia'\n6. Ustaw dzien dzien tygodnia, do ktorego alarm dziala: daystop: 'nr_dnia'\n7. Ustaw minimalny czas wlaczenia obwodu: omin 'czas'\n8. Ustaw maksymalny czas wlaczenia obwodu: omax 'czas'\n9. Wyswietl menu: menu ");

  }
  else if (komenda.substring(0, 4) == "alarm") {

  }
  else if (komenda.substring(0, 6) == "noalarm") {
    deactivateAlarm();
  }
  else if (komenda.substring(0, 5) == "gstart") {
    int n = komenda.length() - 5;
    char array[n];
    komenda.substring(6).toCharArray(array, n);
    EEPROM.write(0, n);
    for (int i = 1; i < n + 1; i++)
      EEPROM.write(i, (int)array[i - 1]);
  }
  else if (komenda.substring(0, 5) == "gstop") {

  }
  else if (komenda.substring(0, 4) == "omin") {

  }
  else if (komenda.substring(0, 4) == "omax") {

  }
  else if (komenda.substring(0, 4) == "daystart") {

  }
  else if (komenda.substring(0, 4) == "daystart") {

  }

}

void getTimeFromGPS(){
  
  
  }
