const int pinSW = 2;  // pin pentru butonul de pe joystick

const int pinX = A0;  // pin analog pentru axa x a joystick-ului

const int pinY = A1;  // pin analog pentru axay a joystick-ului


const int pinA = 12;
const int pinB = 10;
const int pinC = 9;
const int pinD = 8;
const int pinE = 7;
const int pinF = 6;
const int pinG = 5;
const int pinDP = 4;

const int VecinNull = -1;

int SegmentCurent = 7;
const int segSize = 8;





const int DeadZone = 100;

const int DebounceTime = 600;
unsigned long lastDebounceTime = 0;

const int IntervalBlink = 300;
unsigned long TimpAnteriorBlink = 0;


unsigned long TimpApasareLung = 1500;  // Timpul pentru resetarea starii a 7 digit display

volatile bool buttonPressed = false;
volatile unsigned long lastButtonPressTime = 0;


int SegmentsStates[segSize] = { 0, 0, 0, 0, 0, 0, 0, 0 };  // retinem starile pentru fiecare segment ca sa stim daca il stingem sau aprindem cand facem intreruperea

int segments[segSize] = {

  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP

};

typedef enum {
  LEFT,
  RIGHT,
  UP,
  DOWN,
  NONE
} Direction;


// a =0,b=1,c=2,d=3,e=4,f=5,g=6,dp=7

int VeciniSegmente[segSize][4] = {
  // vecinii pe care putem merge(pozitiile din segments) in functie de directia data din joystick
  // stanga,dreapta,sus,jos
  { 5, 1, VecinNull, 6 },                 // Vecinii lui A
  { 5, VecinNull, 0, 6 },                 // B
  { 4, 7, 6, 3 },                         // C
  { 4, 2, 6, VecinNull },                 //D
  { VecinNull, 2, 5, 3 },                 //E
  { VecinNull, 1, 0, 4 },                 //F
  { VecinNull, VecinNull, 0, 3 },         //G
  { 2, VecinNull, VecinNull, VecinNull }  //DP
};




void setup() {


  for (int i = 0; i < segSize; i++) {

    pinMode(segments[i], OUTPUT);
  }

  pinMode(pinSW, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinSW), toggleButon, CHANGE);


  Serial.begin(9600);
}



void loop() {

  if (IsJoystickMoved()) {

    MoveSegment();
  }

  BlinkContinuu(segments[SegmentCurent]);


  for (int i = 0; i < segSize; i++) {
    if (i != SegmentCurent) {  // Aprindem/stingem segmentele in functie de starile retinute, dar nu pe cel curent caruia ii facem blink
      digitalWrite(segments[i], SegmentsStates[i]);
    }
  }
}

void BlinkContinuu(int PinNumber) {
  unsigned long TimpCurent = millis();
  if (TimpCurent - TimpAnteriorBlink > IntervalBlink) {

    digitalWrite(PinNumber, !digitalRead(PinNumber));
    TimpAnteriorBlink = TimpCurent;
  }
}

bool IsJoystickMoved() {  // Debounce pentru miscarea de joystick

  int x = analogRead(pinX) - 501;
  int y = analogRead(pinY) - 512;
  if (abs(x) > DeadZone || abs(y) > DeadZone) {


    unsigned long currentTime = millis();
    if (currentTime - lastDebounceTime > DebounceTime) {
      lastDebounceTime = currentTime;
      return true;
    }
  }
  return false;
}


Direction getJoystickDirection() {

  int x = analogRead(pinX) - 501;  // am dat print la valoarea lui x cand nu atingem joystick si am observat ca este 501 in pozitia neutra
  int y = analogRead(pinY) - 512;

  if (abs(x) > abs(y)) {  // daca valoarea lui x este mai mare, inseamna ca ne miscam orizontal


    if (x > DeadZone) return RIGHT;
    if (x < -DeadZone) return LEFT;
  } else {
    if (y > DeadZone) return UP;
    if (y < -DeadZone) return DOWN;
  }
  return NONE;
}

void MoveSegment() {
  Direction dir = getJoystickDirection();
  if (dir != NONE) {
    int nextSegment = VeciniSegmente[SegmentCurent][dir];
    if (nextSegment != -1) {

      SegmentCurent = nextSegment;
    }
  }
}
void toggleButon() {
  bool currentState = digitalRead(pinSW);

  if (currentState == LOW) {  // Cand apasam butonul
    if (!buttonPressed) {
      buttonPressed = true;            // Marcam ca butonul a fost apasat
      lastButtonPressTime = millis();  // Inregistram timpul la care a fost apasat
    }
  }
  // Altfel, daca am dat drumul butonului
  else if (buttonPressed) {
    buttonPressed = false;
    unsigned long pressDuration = millis() - lastButtonPressTime;  // Calculam durata apasarii

    // Verificam daca durata este mai lunga decat timpul pentru o apasare lunga
    if (pressDuration >= TimpApasareLung) {
      Resetare();
    }

    else {  // apasare scurta, dam doar toggle
      SegmentsStates[SegmentCurent] = !SegmentsStates[SegmentCurent];
    }
  }
}

void Resetare() {
  for (int i = 0; i < segSize; i++) {
    SegmentsStates[i] = 0;
  }
  SegmentCurent = 7;
}
