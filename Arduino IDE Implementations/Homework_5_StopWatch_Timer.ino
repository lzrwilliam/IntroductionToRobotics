// Define pin connections to the shift register
const int latchPin = 11;  // Pin for latching data into the register
const int clockPin = 10;  // Pin for the shift register clock
const int dataPin = 12;   // Pin for the serial data input to the shift register
// Define control pins for the individual digits of the display
const int segD1 = 4;     // Control pin for the first digit
const int segD2 = 5;     // Control pin for the second digit
const int segD3 = 6;     // Control pin for the third digit
const int segD4 = 7;     // Control pin for the fourth digit
const byte regSize = 8;  // Number of bits in the register
// Array of digit control pins
int displayDigits[] = {
  segD1, segD2, segD3, segD4
};
const int displayCount = 4;      // Number of digits in the display
const int encodingsNumber = 20;  // Number of different character encodings

volatile bool Counting = false;  // initial nu se contorizeaza timpul, este afisat 000.0
volatile unsigned long lastDebounceTime = 0;
const long debounceDelay = 200;
unsigned long startTime = 0;
unsigned long pausedTime = 0;

const int ButonReset = 2;
const int ButonStartStop = 3;
const int ButonLap = 8;
const int buzzer = 13;

unsigned long laps[4] = { 0 };   // Vector unde salvam cele 4 lap-uri
int currentLap = 0;              // Indexul lap-ului curent
int lapViewIndex = -1;           // Index pentru vizualizarea lap-urilor salvate
bool VizualizareLapuri = false;  // daca suntem in modul de view al lap-urilor
bool lastButtonLapState = HIGH;
bool CurrentLapState;
bool firstResetPressed = false;


byte encodingArray[encodingsNumber][regSize] = {
  // A  B  C  D  E  F  G DP
  { 1, 1, 1, 1, 1, 1, 0, 0 },  // 0
  { 0, 1, 1, 0, 0, 0, 0, 0 },  // 1
  { 1, 1, 0, 1, 1, 0, 1, 0 },  // 2
  { 1, 1, 1, 1, 0, 0, 1, 0 },  // 3
  { 0, 1, 1, 0, 0, 1, 1, 0 },  // 4
  { 1, 0, 1, 1, 0, 1, 1, 0 },  // 5
  { 1, 0, 1, 1, 1, 1, 1, 0 },  // 6
  { 1, 1, 1, 0, 0, 0, 0, 0 },  // 7
  { 1, 1, 1, 1, 1, 1, 1, 0 },  // 8
  { 1, 1, 1, 1, 0, 1, 1, 0 },  // 9
  { 1, 1, 1, 1, 1, 1, 0, 1 },  // 0 cu DP
  { 0, 1, 1, 0, 0, 0, 0, 1 },  // 1 cu DP
  { 1, 1, 0, 1, 1, 0, 1, 1 },  // 2 cu DP
  { 1, 1, 1, 1, 0, 0, 1, 1 },  // 3 cu DP
  { 0, 1, 1, 0, 0, 1, 1, 1 },  // 4 cu DP
  { 1, 0, 1, 1, 0, 1, 1, 1 },  // 5 cu DP
  { 1, 0, 1, 1, 1, 1, 1, 1 },  // 6 cu DP
  { 1, 1, 1, 0, 0, 0, 0, 1 },  // 7 cu DP
  { 1, 1, 1, 1, 1, 1, 1, 1 },  // 8 cu DP
  { 1, 1, 1, 1, 0, 1, 1, 1 },  // 9 cu DP
};
void setup() {

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  // Initialize digit control pins and set them to on
  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }

  pinMode(ButonStartStop, INPUT_PULLUP);
  pinMode(ButonReset, INPUT_PULLUP);
  pinMode(ButonLap, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(ButonStartStop), StartPauseToggle, FALLING);
  attachInterrupt(digitalPinToInterrupt(ButonReset), ResetToggle, FALLING);
  Serial.begin(9600);
}
void loop() {
  if (Counting) {
    unsigned long currentTime = millis();
    unsigned long elapsed = currentTime - startTime;
    displayTime(elapsed);
  } else if (VizualizareLapuri == true) {
    displayTime(laps[lapViewIndex]);

  } else {
    displayTime(pausedTime);
  }

  CurrentLapState = digitalRead(ButonLap);
  if (CurrentLapState == LOW && lastButtonLapState == HIGH) {
    if ((millis() - lastDebounceTime) > debounceDelay) {
      lastDebounceTime = millis();
      tone(buzzer, 10, 100);
      ApasareLapButton();
    }
  }

  lastButtonLapState = CurrentLapState;  // ca sa nu se auda buzzer si sa se schimbe lap-urile cand tinem apasat pe buton, ci doar cand apasam o singura data
}




void displayTime(unsigned long time) {
  //1000 milisecunde = 1 secunda
  int seconds = time / 1000;
  int milliseconds = (time % 1000) / 100;

  int firstDigit = (seconds / 100) % 10;
  int secondDigit = (seconds / 10) % 10;
  int thirdDigit = seconds % 10;
  int fourthDigit = milliseconds;

  displayDigit(0, firstDigit);
  displayDigit(1, secondDigit);
  displayDigit(2, thirdDigit + 10);  // Adaugam 10 pentru a selecta cifra cu DP
  displayDigit(3, fourthDigit);
}
void displayDigit(int digit, int value) {
  // Dezactivam toate cifrele
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }

  // Activeaza cifra curenta
  digitalWrite(displayDigits[digit], LOW);

  // Scrie valoarea pe cifra activata
  writeReg(encodingArray[value]);

  delay(4);  // Un scurt delay pentru a permite afișajul fiecărei cifre
}


void writeReg(byte encoding[]) {
  // Send each bit of the encoding to the shift register
  digitalWrite(latchPin, LOW);  // incepe sa transmita date
  for (int i = 0; i < regSize; i++) {
    digitalWrite(clockPin, LOW);         // Set the clock pin low before sending data
    digitalWrite(dataPin, encoding[i]);  // Send the data bit
    digitalWrite(clockPin, HIGH);        // Clock the data bit into the register
  }
  digitalWrite(latchPin, HIGH);  // Latch the data into the register to update the display
}


void StartPauseToggle() {
  if ((millis() - lastDebounceTime) > debounceDelay) {
    lastDebounceTime = millis();
    if (firstResetPressed) {
      // navigam descendent prin lap-uri daca suntem la prima apasare a butonului reset
      if (!Counting) {
        if (lapViewIndex == 0) {
          lapViewIndex = 3;  // Setează la ultimul lap dacă suntem deja la primul
        } else {
          lapViewIndex--;  // Altfel, doar scade indexul cu 1
        }
        VizualizareLapuri = true;
      }
    } else {
      // start/pauza in mod normal altfel
      Counting = !Counting;
      if (Counting) {
        if (VizualizareLapuri) {
          VizualizareLapuri = false;
          lapViewIndex = -1;
        }
        startTime = millis() - pausedTime;
      } else {
        pausedTime = millis() - startTime;
      }
    }
  }
}

void ResetToggle() {
  if ((millis() - lastDebounceTime) > debounceDelay) {
    lastDebounceTime = millis();
    if (!Counting) {
      if (!firstResetPressed) {
        pausedTime = 0;
        firstResetPressed = true;
      } else {
        pausedTime = 0;
        for (int i = 0; i < 4; i++) {
          laps[i] = 0;
        }
        currentLap = 0;
        lapViewIndex = -1;
        VizualizareLapuri = false;
        firstResetPressed = false;
      }
    }
  }
}

void ApasareLapButton() {
  if (Counting) {
    // Salvam lap=ul curent
    laps[currentLap] = millis() - startTime;
    if (currentLap == 3) {
      currentLap = 0;  // daca suntem la ultimul lap  revenim la primul
    } else {
      currentLap++;  // altfel trecem la următorul lap
    }
  } else if (!Counting && firstResetPressed) {
    // Navigare ascendenta prin lap-uri
    lapViewIndex = (lapViewIndex + 1) % 4;  // echivalentul if-ului de mai sus; am pus ambele metode ca sa nu uit logica
    VizualizareLapuri = true;
  }
}
