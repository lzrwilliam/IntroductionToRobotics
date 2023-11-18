#include <EEPROM.h>
// Valoarea 5 la ultrasonic inseamna ca obiectul este aproape; 1000 departe
// ldr sub 5-10 spre intuneric, peste 20-30 cu blitz in ea
const int adresaIntervalEsantionare1 = 0;
const int adresaIntervalEsantionare2 = sizeof(short int) + adresaIntervalEsantionare1;
const int MinorMaxUltrasonic = sizeof(bool) + adresaIntervalEsantionare2;
const int ValoareUltrasonic = sizeof(short int) + MinorMaxUltrasonic;
const int MinorMaxLDR = sizeof(bool) + ValoareUltrasonic;
const int ValoareLDR = sizeof(short int) + MinorMaxLDR;

const int maxReadings = 10;
const int readingsAddressStart = ValoareLDR + 1;
const int indexAddress = readingsAddressStart;                           // Adresa pentru index in EEPROM
const int dataStartAddress = indexAddress + sizeof(unsigned short int);  // Adresa de start pentru date
const int autoModeAddress = (dataStartAddress + sizeof(unsigned short int) * maxReadings * 2) + 1;

const int lastRGBValuesAddress = autoModeAddress + 1;

uint8_t currentMenu = 0;

const uint8_t trigPin = 9;
const uint8_t echoPin = 10;
const uint8_t RedPin = A2;
const uint8_t GreenPin = A1;
unsigned long duration = 0;
uint16_t distance = 0;

const uint8_t photocellPin = A0;
uint16_t photocellValue;

unsigned long lastLDRWarningTime = 0;
unsigned long lastUltrasonicWarningTime = 0;
const unsigned long LDRWarningInterval = 5000;  // 5 secunde în milisecunde

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  while (!Serial) {
    ;  // asteptam input de la serial
  }
  displayMainMenu();
}

void loop() {
  updateLedColor();
  if (Serial.available() > 0) {

    int choice = Serial.parseInt();
    // Serial.println(choice);
    if (choice) {
      processChoice(choice);
    }
  }
}

void displayMainMenu() {
  currentMenu = 0;
  Serial.println("Meniu Principal:");
  Serial.println("1. Setări Senzori");
  Serial.println("2. Resetare Date Logger");
  Serial.println("3. Stare Sistem");
  Serial.println("4. Control LED RGB");
  Serial.println("Introduceți numărul opțiunii:");
}

void displaySensorSettingsMenu() {
  currentMenu = 1;
  Serial.println("Setări Senzori:");
  Serial.println("1. Interval Esantionare Senzori");
  Serial.println("2. Prag Alerta Ultrasonic");
  Serial.println("3. Prag Alerta LDR");
  Serial.println("4. Înapoi");
  Serial.println("Introduceti numarul sub-optiunii:");
}
void displayResetLogMenu() {
  currentMenu = 2;
  Serial.println(F("Doriti sa resetati valorile senzorilor?"));
   Serial.println("VALORI LDR SI ULTRASONIC \n");
  int valoareCitita1 = 0;
  int val2 = 0;
  EEPROM.get(ValoareLDR, valoareCitita1);  // Citim valoarea corect
  Serial.println(valoareCitita1);
  EEPROM.get(ValoareUltrasonic, val2);
  Serial.println(val2);
  Serial.println(F("---------------------------------"));
  Serial.println("1. Da");
  Serial.println("2. Nu");
}

void displaySystemMenu() {
  currentMenu = 3;
  Serial.println(F("System status:"));
  Serial.println(F("1. Current Sensor Readings"));
  Serial.println(F("2. Current Sensor Settings"));
  Serial.println(F("3. Display Logged Data "));
  Serial.println(F("4. Back"));
  Serial.println("Introduceți numarul sub-optiunii:");
}

void displayRGBMenu() {
  currentMenu = 4;
  Serial.println("RGB Led Control:");
  Serial.println("1. Manual Color Control");
  Serial.println("2. Toggle Automatic ON/OFF");
  Serial.println("3. Back");
}
void processChoice(int choice) {
  if (currentMenu == 0) {
    switch (choice) {
      case 1:
        displaySensorSettingsMenu();
        break;
      case 2:
        displayResetLogMenu();
        break;
      case 3:
        displaySystemMenu();
        break;
      case 4:
        displayRGBMenu();
        break;
      default:
        Serial.println(F("Optiune invalida."));
        displayMainMenu();
        break;
    }
  } else if (currentMenu == 1) {
    switch (choice) {
      case 1:
        setSamplingInterval();
        break;
      case 2:
        SetUltrasonic();
        break;
      case 3:
        SetLDR();
        break;
      case 4:
        displayMainMenu();
        break;
      default:
        Serial.println(F("Sub-optiune invalida."));
        displaySensorSettingsMenu();
        break;
    }
  } else if (currentMenu == 2) {
    switch (choice) {
      case 1:
        ResetSensorsValue();
        break;
      case 2:
        displayMainMenu();
        break;
      default:
        Serial.println(F("Sub-optiune invalida."));
        displayResetLogMenu();
        break;
    }
  } else if (currentMenu == 3) {
    switch (choice) {
      case 1:
        displayCurrentSensorReadings();
        break;
      case 2:
        CurrentSensorsSettings();
        break;
      case 3:
        displayLoggedData();
        break;
      case 4:
        displayMainMenu();
        break;
      default:
        Serial.println(F("Sub-optiune invalida."));
        displayResetLogMenu();
        break;
    }
  } else if (currentMenu == 4) {
    switch (choice) {
      case 1:
        manualLedControl();
        break;
      case 2:
        toggleRGBLedOnOff();
        break;
      case 3:
        displayMainMenu();
        break;
      default:
        Serial.println("Sub-optiune invalida.");
        displayResetLogMenu();
        break;
    }
  }
}
void setSamplingInterval() {
  int intervalEsantionare1 = 0;
  int intervalEsantionare2 = 0;
  while (true) {
    Serial.println("Introduceti intervalul de esantionare pentru primul senzor (1-10 secunde):");
    while (Serial.available() == 0) { ; }
    intervalEsantionare1 = Serial.parseInt();
    if (isValidSamplingInterval(intervalEsantionare1)) {
      break;
    } else {
      Serial.println(F("Valoare invalida. Introduceți un numar intre 1 si 10."));
    }
  }

  while (true) {
    Serial.println("Introduceti intervalul de esantionare pentru al doilea senzor (1-10 secunde):");
    while (Serial.available() == 0) { ; }
    intervalEsantionare2 = Serial.parseInt();

    if (isValidSamplingInterval(intervalEsantionare2)) {
      break;  // Iesim din bucla daca valoarea este valida
    } else {
      Serial.println("Valoare invalida. Introduceti un numar intre 1 si 10.");
    }
  }

  EEPROM.put(adresaIntervalEsantionare1, intervalEsantionare1);
  EEPROM.put(adresaIntervalEsantionare2, intervalEsantionare2);
  Serial.println("Intervale setate cu succes:");
  Serial.println(intervalEsantionare1);
  Serial.println(intervalEsantionare2);

  displayMainMenu();
}




bool isValidSamplingInterval(int interval) {
  return interval >= 1 && interval <= 10;
}

void SetUltrasonic() {
  bool alegereValida = false;
  int alegere = -1;

  while (!alegereValida) {
    Serial.println("Alegeti valoarea Ultrasonic, pentru minim (0) iar pentru maxim (1):");
    while (!Serial.available()) { ; }
    alegere = Serial.parseInt();

    if (alegere == 0 || alegere == 1) {
      alegereValida = true;
      if (alegere == 0) {
        Serial.print("Introduceti valoarea pentru minim: ");
      } else {
        Serial.print("Introduceti valoarea pentru maxim: ");
      }
    } else {
      Serial.println("Introduceti doar 0 sau 1.");
    }
  }

  while (Serial.available() == 0) { ; }
  int Valoareultrasonic = Serial.parseInt();
  EEPROM.put(MinorMaxUltrasonic, alegere);
  EEPROM.put(ValoareUltrasonic, Valoareultrasonic);
  Serial.print("Ati ales ");
  if (alegere == 0) Serial.print("minim ");
  else Serial.print("maxim ");
  Serial.print(" : ");
  int valoareCitita;
  EEPROM.get(ValoareUltrasonic, valoareCitita);
  Serial.println(valoareCitita);
  displayMainMenu();
}

void SetLDR() {

  bool alegereValida = false;
  int alegere = -1;

  while (!alegereValida) {
    Serial.println("Alegeti valoarea LDR, pentru minim (0) iar pentru maxim (1):");
    while (!Serial.available()) { ; }
    alegere = Serial.parseInt();

    if (alegere == 0 || alegere == 1) {
      alegereValida = true;
      if (alegere == 0) {
        Serial.print("Introduceti valoarea pentru minim: ");
      } else {
        Serial.print("Introduceti valoarea pentru maxim: ");
      }
    } else {
      Serial.println("Introduceti doar 0 sau 1.");
    }
  }

  while (Serial.available() == 0) { ; }
  int ValoareLDRcitita = Serial.parseInt();

  EEPROM.put(MinorMaxLDR, alegere);
  EEPROM.put(ValoareLDR, ValoareLDRcitita);

  Serial.print(F("Ati ales "));
  if (alegere == 0) Serial.print("minim ");
  else Serial.print("maxim ");
  Serial.print(" : ");
  int valoareCitita;
  EEPROM.get(ValoareLDR, valoareCitita);  // Citim valoarea corect
  Serial.println(valoareCitita);
  displayMainMenu();
}

void ResetSensorsValue() {
  bool alegereValida = false;
  int alegere = -1;

 
  while (!alegereValida) {
    Serial.println("Alegeti care se reseteaza  pentru senzorul Ultrasonic(0),LDR (1) sau ambii senzori (2):");
    while (!Serial.available()) { ; }
    alegere = Serial.parseInt();
    if ((alegere == 0 || alegere == 1) || alegere == 2) {
      alegereValida = true;
    } else {
      Serial.println("Introduceti doar 0 sau 1.");
    }
  }
  if (alegere == 0) {
    EEPROM.put(ValoareUltrasonic, 0);
    Serial.print("Valoarea senzorului Ultrasonic a fost resetata");
  } else if (alegere == 1) {
    EEPROM.put(ValoareLDR, 0);
    Serial.print("Valoarea senzorului LDR a fost resetata");
  } else if (alegere == 2) {
    EEPROM.put(ValoareUltrasonic, 0);
    EEPROM.put(ValoareLDR, 0);
    Serial.println("Valorile pentru senzorii Ultrasonic si LDR au fost resetate! \n");
  }
  int valoareCitita1;
  Serial.print("Ultrasonic: ");
  EEPROM.get(ValoareUltrasonic, valoareCitita1);  // Citim valoarea corect
  Serial.println(valoareCitita1);

  Serial.print("LDR: ");
  EEPROM.get(ValoareLDR, valoareCitita1);  // Citim valoarea corect
  Serial.println(valoareCitita1);

  displayMainMenu();
}

void CurrentSensorsSettings() {

  Serial.print("Valoarea de esantionare pentru senzorul ultrasonic este setata la secunde: ");
  Serial.println(EEPROM.read(adresaIntervalEsantionare1));
  Serial.println();

  Serial.print("Valoarea de esantionare pentru senzorul LDR este setata la secunde: ");
  Serial.println(EEPROM.read(adresaIntervalEsantionare2));
  Serial.println();


  Serial.print("Pentru sezorul Ultrasonic, avem ");
  if (EEPROM.read(MinorMaxUltrasonic) == 0) Serial.print("minim setat la: ");
  else Serial.print("maxim setat la: ");
  int Valoaresenzor = 0;
  EEPROM.get(ValoareUltrasonic, Valoaresenzor);
  Serial.println(Valoaresenzor);
  Serial.println();

  Serial.print("Pentru sezorul LDR, avem ");
  if (EEPROM.read(MinorMaxLDR) == 0) Serial.print("minim setat la: ");
  else Serial.print("maxim setat la: ");
  EEPROM.get(ValoareLDR, Valoaresenzor);
  Serial.println(EEPROM.read(ValoareLDR));
  Serial.println();


  displayMainMenu();
}

void displayCurrentSensorReadings() {
  Serial.println("Current real-time readings for Ultrasonic and LDR sensors:");
  Serial.println("Press any key to exit.");

  unsigned long previousMillisUltrasonic = 0;
  unsigned long previousMillisLDR = 0;
  int samplingIntervalUltrasonic = EEPROM.read(adresaIntervalEsantionare1) * 1000;
  int samplingIntervalLDR = EEPROM.read(adresaIntervalEsantionare2) * 1000;  // am converit in milisecunde

  while (true) {
    // updateLedColor();
    unsigned long currentMillis = millis();

    if (Serial.available() > 0 && Serial.read()) {
      break;  // iesim din bucla cand apasam orice tasta
    }
    int valoareUltrasonic = 0;

    if (currentMillis - previousMillisUltrasonic >= samplingIntervalUltrasonic) {
      previousMillisUltrasonic = currentMillis;
      valoareUltrasonic = readUltrasonicSensor();
      Serial.print("Valoare Ultrasonic: ");
      Serial.println(valoareUltrasonic);
      updateLedColor();
    }
    int valoareLDR = 0;
    if (currentMillis - previousMillisLDR >= samplingIntervalLDR) {
      previousMillisLDR = currentMillis;

      valoareLDR = readLDRSensor();
      Serial.print("Valoare LDR: ");
      Serial.println(valoareLDR);
      Serial.println("Press any key to exit");
      updateLedColor();
    }
    if (valoareLDR != 0 && valoareUltrasonic != 0) {
      storeSensorReadings(valoareUltrasonic, valoareLDR);
    }
  }

  displayMainMenu();  // meniul principal
}

int readUltrasonicSensor() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  unsigned long currentTime = millis();
  if (currentTime - lastUltrasonicWarningTime >= LDRWarningInterval && EEPROM.read(ValoareUltrasonic) != 0) {

    if (EEPROM.read(MinorMaxUltrasonic) && EEPROM.read(ValoareUltrasonic) < distance) {
      Serial.print(F("Valoarea pentru senzorul  Ultrasonic este mai mare decat cea stabilita:"));
      int valultrasonic = EEPROM.read(ValoareUltrasonic);
      Serial.println(valultrasonic);
      lastUltrasonicWarningTime = currentTime;
    } else if (!EEPROM.read(MinorMaxUltrasonic) && EEPROM.read(ValoareUltrasonic) > distance) {
      Serial.print(F("Valoarea pentru senzorul Ultrasonic este mai mica decat cea minima stabilita: "));
      int valultrasonic = EEPROM.read(ValoareUltrasonic);
      Serial.println(valultrasonic);
      lastUltrasonicWarningTime = currentTime;
    }
  }
  return distance;
}

int readLDRSensor() {
  unsigned long currentTime = millis();
  if (currentTime - lastLDRWarningTime >= LDRWarningInterval && EEPROM.read(ValoareLDR) != 0) {

    if (EEPROM.read(MinorMaxLDR) && EEPROM.read(ValoareLDR) < analogRead(photocellPin)) {
      Serial.print(F("Valoarea pentru senzorul LDR este mai mare decat cea stabilita:"));
      int valldr = EEPROM.read(ValoareLDR);
      Serial.println(valldr);
      lastLDRWarningTime = currentTime;
    } else if (!EEPROM.read(MinorMaxLDR) && EEPROM.read(ValoareLDR) > analogRead(photocellPin)) {
      Serial.print(F("Valoarea pentru senzorul LDR este mai mica decat cea minima stabilita: "));
      int valldr = EEPROM.read(ValoareLDR);
      Serial.println(valldr);
      lastLDRWarningTime = currentTime;
    }
  }

  return analogRead(photocellPin);
}
void storeSensorReadings(unsigned short int ultrasonicReading, unsigned short int ldrReading) {
  unsigned short int currentIndex;
  EEPROM.get(indexAddress, currentIndex);

  // Calculeaza noi adrese pentru citire
  int ultrasonicAddress = dataStartAddress + (currentIndex % maxReadings) * sizeof(unsigned short int);
  int ldrAddress = ultrasonicAddress + maxReadings * sizeof(unsigned short int);

  EEPROM.update(ultrasonicAddress, ultrasonicReading);
  EEPROM.update(ldrAddress, ldrReading);

  // Incrementarea si actualizarea indexului
  currentIndex = (currentIndex + 1) % maxReadings;
  EEPROM.update(indexAddress, currentIndex);
}
void displayLoggedData() {
  unsigned short int currentIndex;
  EEPROM.get(indexAddress, currentIndex);

  Serial.println("Ultimele 10 citiri:");
  for (int i = 0; i < maxReadings; i++) {
    int idx = (currentIndex + i) % maxReadings;
    int ultrasonicAddress = dataStartAddress + idx * sizeof(unsigned short int);
    int ldrAddress = ultrasonicAddress + maxReadings * sizeof(unsigned short int);
    unsigned short int ultrasonicValue, ldrValue;
    EEPROM.get(ultrasonicAddress, ultrasonicValue);
    EEPROM.get(ldrAddress, ldrValue);
    Serial.print("Ultrasonic: ");
    Serial.print(ultrasonicValue);
    Serial.print("  LDR: ");
    Serial.println(ldrValue);
  }
}
void toggleRGBLedOnOff() {
  bool autoMode;
  EEPROM.get(autoModeAddress, autoMode);
  autoMode = !autoMode;
  EEPROM.put(autoModeAddress, autoMode);
  Serial.print("Modul auto este ");
  Serial.println(autoMode ? "activat" : "dezactivat");
  // daca modul auto este off, salvam culoarea la rgb
  if (!autoMode) {
    saveCurrentLedState();
  }
}


void updateLedColor() {
  bool autoMode;
  EEPROM.get(autoModeAddress, autoMode);
  int ultrasonicReading = readUltrasonicSensor();
  int ldrReading = readLDRSensor();
  if (autoMode) {

    bool ultrasonicThresholdType;  // true pentru maxim, false pentru minim
    bool ldrThresholdType;         // true pentru maxim, false pentru minim

    int ultrasonicThreshold, ldrThreshold;
    EEPROM.get(ValoareUltrasonic, ultrasonicThreshold);
    EEPROM.get(ValoareLDR, ldrThreshold);
    EEPROM.get(MinorMaxUltrasonic, ultrasonicThresholdType);
    EEPROM.get(MinorMaxLDR, ldrThresholdType);



    bool ultrasonicAlert = (ultrasonicThresholdType && ultrasonicReading > ultrasonicThreshold) || (!ultrasonicThresholdType && ultrasonicReading < ultrasonicThreshold);
    bool ldrAlert = (ldrThresholdType && ldrReading > ldrThreshold) || (!ldrThresholdType && ldrReading < ldrThreshold);
    if (ldrThreshold != 0 || ultrasonicThreshold != 0) {
      if (ultrasonicAlert != 0 || ldrAlert != 0) {
        analogWrite(RedPin, 200);
        analogWrite(GreenPin, 0);
      } else {
        analogWrite(RedPin, 0);
        analogWrite(GreenPin, 200);
      }
    }
  } else {

    int redValue, greenValue;
    EEPROM.get(lastRGBValuesAddress, redValue);
    EEPROM.get(lastRGBValuesAddress + sizeof(int), greenValue);
    analogWrite(RedPin, redValue);
    analogWrite(GreenPin, greenValue);
  }
}

void saveCurrentLedState() {

  int redValue = analogRead(RedPin);
  int greenValue = analogRead(GreenPin);
  EEPROM.put(lastRGBValuesAddress, redValue);
  EEPROM.put(lastRGBValuesAddress + sizeof(int), greenValue);
}
void manualLedControl() {
  Serial.println(F("Control manual LED:"));
  Serial.println(F("Introduceți 'r' pentru rosu sau 'v' pentru verde:"));

  while (!Serial.available()) {
    ;
  }
  char colorChoice = Serial.read();
  if (colorChoice == 'r') {
    analogWrite(RedPin, 255);
    analogWrite(GreenPin, 0);
    Serial.println(F("LED-ul a fost setat pe rosu."));
  } else if (colorChoice == 'v') {
    analogWrite(RedPin, 0);
    analogWrite(GreenPin, 255);
    Serial.println(F("LED-ul a fost setat pe verde."));
  } else {
    Serial.println(F("Optiune invalida. Introduceti 'r' pentru rosu sau 'v' pentru verde."));
  }


  // Salvam starea LED-ului în EEPROM
  saveCurrentLedState();
  displayMainMenu();
}
