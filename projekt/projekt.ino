#include <LiquidCrystal.h>
#include <EEPROM.h>

//boleans
bool isAlarmActivated = true;
boolean koniec_odczytu = false;

//Strings
String odczyt_serial = "";

//ints
int y = 0;

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
  Serial.println("Dostepne opcje: \n1. Ustaw godzine rozpoczecia: gstart 'godzina' \n2. Ustaw godzine zakonczenia: gstop 'godzina'\n3. Ustaw minimalny czas wlaczenia obwodu: omin 'czas'\n4. Ustaw maksymalny czas wlaczenia obwodu: omax 'czas'\n5. Wlacz alarm: start\n6. Wylacz alarm: stop\n7. Wyswietl menu: menu ");
}

void loop() {
  // put your main code here, to run repeatedly:
  //  controllAlarm();

  runSerialPort();
  //  blinkLeds();
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
    Serial.println("Dostepne opcje: \n1. Ustaw godzine rozpoczecia: gstart 'godzina' \n2. Ustaw godzine zakonczenia: gstop 'godzina'\n3. Ustaw minimalny czas wlaczenia obwodu: omin 'czas'\n4. Ustaw maksymalny czas wlaczenia obwodu: omax 'czas'\n5. Wlacz alarm: start\n6. Wylacz alarm: stop\n7. Wyswietl menu: menu ");

  }
  else if (komenda.substring(0, 4) == "echo") {

  }
  else if (komenda.substring(0, 4) == "line") {
    if (komenda == "line 1")
      y = 0;
    else if (komenda == "line 2")
      y = 1;
    else
      Serial.println("Błędny numer lini.");
  }
  else if (komenda.substring(0, 5) == "store") {
    int n = komenda.length() - 5;
    char array[n];
    komenda.substring(6).toCharArray(array, n);
    EEPROM.write(0, n);
    for (int i = 1; i < n + 1; i++)
      EEPROM.write(i, (int)array[i - 1]);
  }
  else if (komenda.substring(0, 4) == "read") {

  }

}
//------------------------------------------------------------------------
//String getCommand() {
//  if(Serial.available() > 0) {
//    String str = Stream.read();
//    Serial.print("asdasd");
//    return str;
//  }
//}

//------------------------------------------------------------------------
//void controllAlarm() {
//  if(getCommand() == String("Turn off")) {
//    deactivateAlarm();
//  }
//  else if(getCommand() == String("Turn on")) {
//    activateAlarm();
//  }
//}
