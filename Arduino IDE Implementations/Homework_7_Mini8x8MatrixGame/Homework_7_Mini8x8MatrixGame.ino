#include "LedControl.h"
const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;

const int xPin = A0;
const int yPin = A1;
const int swPin = 2;
const int pinBuzzer = 3;

// Create an LedControl object to manage the LED matrix
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);  // DIN, CLK, LOAD, No. DRIVER
// Variable to set the brightness level of the matrix
byte matrixBrightness = 2;
// Variables to track the current and previous positions of the joystick-controlled LED
byte xPos = 0;
byte yPos = 0;
byte xLastPos = 0;
byte yLastPos = 0;
// Thresholds for detecting joystick movement
const int minThreshold = 200;
const int maxThreshold = 600;

const byte moveInterval = 500;  // Timing variable to control the speed of LED movement
const byte BlinkingPlayerPositionInterval = 600;
unsigned long lastBlink = 0;       // Ultimul timp cand a avut loc un blink
bool isLedOn = true;               // Starea curenta a LED-ului
unsigned long long lastMoved = 0;  // Tracks the last time the LED moved

const byte matrixSize = 8;  // Size of the LED matrix
bool matrixChanged = true;  // Flag to track if the matrix display needs updating
byte matrix[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};

const byte EMPTY = 0;
const byte PLAYER = 1;
const byte INDESTRUCTIBLE_WALL = 2;
const byte DESTRUCTIBLE_WALL = 3;
const byte BOMB = 4;

uint8_t numarPeretiDistrusi = 0;
bool bombPlaced = false;
int bombX = 0;
int bombY = 0;
unsigned long bombTimer = 0;
const unsigned long bombDelay = 3000;    // 3 secunde pana explodeaza bomba
const unsigned long debounceDelay = 50;  //
unsigned long lastDebounceTime = 0;
int buttonState=HIGH;
int lastButtonState = HIGH;

struct Bomb {
  bool active;
  int x;
  int y;
  unsigned long timer;
};
const int maxBombs = 10;
Bomb bombs[maxBombs];

void setup() {
  Serial.begin(9600);
  pinMode(swPin, INPUT_PULLUP);
  randomSeed(analogRead(0));  // initializam generaratorul de numere random
  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false);                 // turn off power saving, enables display
  lc.setIntensity(0, matrixBrightness);  // sets brightness (0~15 possible values)
  lc.clearDisplay(0);                    // Clear the matrix display
  spawnPlayer();
  initializeWalls();

  BombsSetToFalse();
}

void loop() {
  handleBombLogic();
  updateMatrix();
  BlinkPlayerPos();
  if (millis() - lastMoved > moveInterval) {
    updatePositions();
    lastMoved = millis();
  }
  if (matrixChanged) {
    updateMatrix();
    matrixChanged = false;
  }
}
void updateMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      bool bombHere = false;
      for (int i = 0; i < maxBombs; i++) {
        if (bombs[i].active && bombs[i].x == row && bombs[i].y == col) {
          bombHere = true;
          break;
        }
      }
      if (bombHere) {
        lc.setLed(0, row, col, (millis() / 50) % 2);  // blink rapid rapid la bomba
      } else {
        switch (matrix[row][col]) {
          case INDESTRUCTIBLE_WALL:
            lc.setLed(0, row, col, true);  // peretii indestructibili fara blink
            break;
          case DESTRUCTIBLE_WALL:
            lc.setLed(0, row, col, (millis() / 700) % 2);  // pereti destructibili blink usor
            break;
          case PLAYER:
            lc.setLed(0, row, col, true);  // Player mereu pornit
            break;
          default:
            lc.setLed(0, row, col, false);  // celulele goale off
            break;
        }
      }
    }
  }
}
void updatePositions() {
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);

  byte newXPos = xPos;
  byte newYPos = yPos;

  // verifica pozitia pe axa x
  if (xValue < minThreshold && xPos > 0) {
    if (matrix[xPos - 1][yPos] == EMPTY) {
      newXPos = xPos - 1;
    }
  } else if (xValue > maxThreshold && xPos < matrixSize - 1) {
    if (matrix[xPos + 1][yPos] == EMPTY) {
      newXPos = xPos + 1;
    }
  }

  // Verifica pozitia pe axa y
  if (yValue < minThreshold && yPos < matrixSize - 1) {
    if (matrix[xPos][yPos + 1] == EMPTY) {
      newYPos = yPos + 1;
    }
  } else if (yValue > maxThreshold && yPos > 0) {
    if (matrix[xPos][yPos - 1] == EMPTY) {
      newYPos = yPos - 1;
    }
  }

  //actualizam jucatorul si matricea daca e cazu;
  if (newXPos != xPos || newYPos != yPos) {
    matrix[xPos][yPos] = EMPTY;  // celula unde era player inainte sa se mute o oprim
    xPos = newXPos;
    yPos = newYPos;
    matrix[xPos][yPos] = PLAYER;  // setam noua pozitie a player
    matrixChanged = true;
  }
}
void BlinkPlayerPos() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastBlink > BlinkingPlayerPositionInterval) {
    lastBlink = currentMillis;
    isLedOn = !isLedOn;
    matrix[xPos][yPos] = isLedOn ? 1 : 0;
    matrixChanged = true;
  }
}
void initializeWalls() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      if (random(100) < 65) {                                                // sanse de 65% sa avem un perete
        byte wallType = random(INDESTRUCTIBLE_WALL, DESTRUCTIBLE_WALL + 1);  // Alege un tip de perete aleatoriu
        matrix[row][col] = wallType;
      } else {
        matrix[row][col] = EMPTY;
      }
    }
  }
}
void spawnPlayer() {
  do {
    xPos = random(0, matrixSize);
    yPos = random(0, matrixSize);
  } while (matrix[xPos][yPos] != EMPTY);  // ne asiguram ca pozitia random unde punem player nu este un perete acolo
  matrix[xPos][yPos] = PLAYER;
  matrixChanged = true;
}
void placeBomb() {

  // verificam sa nu mai fie o alta bomba deja plantata pe acea celula
  for (int i = 0; i < maxBombs; i++) {
    if (bombs[i].active && bombs[i].x == xPos && bombs[i].y == yPos) {
      return;  // exista bomba, nu putem pune alta
    }
  }
  for (int i = 0; i < maxBombs; i++) {
    if (!bombs[i].active) {
      bombs[i].active = true;
      bombs[i].x = xPos;
      bombs[i].y = yPos;
      bombs[i].timer = millis();
      matrix[xPos][yPos] = BOMB;
      tone(pinBuzzer, 300, 70);
      break;
    }
  }
}
void detonateBomb() {
  bool peretiDistrusiInAceastaDetonare = false;
  for (int i = 0; i < maxBombs; i++) {
    if (bombs[i].active && millis() - bombs[i].timer > bombDelay) {
      bombs[i].active = false;

      // Distrugere orizontala dreapta
      for (int dx = 1; dx <= 2; dx++) {
        int colRight = bombs[i].y + dx;
        if (colRight < matrixSize) {
          if (matrix[bombs[i].x][colRight] == DESTRUCTIBLE_WALL) {
            matrix[bombs[i].x][colRight] = EMPTY;
            numarPeretiDistrusi++;
            peretiDistrusiInAceastaDetonare = true;

          } else if (matrix[bombs[i].x][colRight] == INDESTRUCTIBLE_WALL) {
            break;  // daca in drum avem un perete care nu poate fi distrus, ne oprim, nu putem distruge nimic ce se afla dupa un acest tip de perete
          }
        }
      }
      // Distrugere orizontala stanga
      for (int dx = -1; dx >= -2; dx--) {
        int colLeft = bombs[i].y + dx;
        if (colLeft >= 0) {
          if (matrix[bombs[i].x][colLeft] == DESTRUCTIBLE_WALL) {
            matrix[bombs[i].x][colLeft] = EMPTY;
            numarPeretiDistrusi++;
            peretiDistrusiInAceastaDetonare = true;


          } else if (matrix[bombs[i].x][colLeft] == INDESTRUCTIBLE_WALL) {
            break;  
          }
        }
      }
      // Distrugere verticala in sus
      for (int dy = 1; dy <= 2; dy++) {
        int rowUp = bombs[i].x - dy;
        if (rowUp >= 0) {
          if (matrix[rowUp][bombs[i].y] == DESTRUCTIBLE_WALL) {
            matrix[rowUp][bombs[i].y] = EMPTY;
            numarPeretiDistrusi++;
            peretiDistrusiInAceastaDetonare = true;
          } else if (matrix[rowUp][bombs[i].y] == INDESTRUCTIBLE_WALL) {
            break;  
          }
        }
      }
      // Distrugere verticala in jos
      for (int dy = 1; dy <= 2; dy++) {
        int rowDown = bombs[i].x + dy;
        if (rowDown < matrixSize) {
          if (matrix[rowDown][bombs[i].y] == DESTRUCTIBLE_WALL) {
            matrix[rowDown][bombs[i].y] = EMPTY;
            numarPeretiDistrusi++;
            peretiDistrusiInAceastaDetonare = true;


          } else if (matrix[rowDown][bombs[i].y] == INDESTRUCTIBLE_WALL) {
            break;  
          }
        }
      }
      if (peretiDistrusiInAceastaDetonare) {
        Serial.print("Pana acum s-au distrus :");
        Serial.print(numarPeretiDistrusi);
        Serial.println(" pereti.");
        peretiDistrusiInAceastaDetonare = false;
      }
      matrix[bombs[i].x][bombs[i].y] = EMPTY;
      matrixChanged = true;
    }
  }
}
void handleBombLogic() {
  int reading = digitalRead(swPin);

  // Debounce 
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        placeBomb();
      }
    }
  }
  lastButtonState = reading;
  detonateBomb();
}
void BombsSetToFalse() {
  for (int i = 0; i < maxBombs; i++) {
    bombs[i].active = false;
  }
}
