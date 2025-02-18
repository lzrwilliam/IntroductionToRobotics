
enum StareLift {
    Asteptare,
    InchidereUsi,
    InMiscare,
    DeschidereUsiLaDestinatie
};

StareLift stare = Asteptare;


int etajCurent = 1;
int etajDestinatie = 1;
unsigned long timpAnterior = 0;



const long TimpPeEtaj = 1000; // timpul de deplasare intre etaje
const long intervalUsi = 1000; // timp pentru deschidere/inchidere usi

const int ledEtaj[] = {4, 5, 6};
const int butonEtaj[] = {8, 9, 10};
const int buzzer = 2;
const int LedBlink = 3;
size_t  numarEtaje = sizeof(ledEtaj) / sizeof(ledEtaj[0]);




const long delayDebounce=40;
int StareButoane[3]={HIGH,HIGH,HIGH};
unsigned long TimpVerificareButon[3]={0};



bool blinkActive = false;  // Daca led-ul verde sa se aprinda si stinga continuu sau nu.



void setup() {
    for (int i = 0; i < numarEtaje; i++) {
        pinMode(ledEtaj[i], OUTPUT);
        pinMode(butonEtaj[i], INPUT_PULLUP);
    }
    pinMode(buzzer, OUTPUT);
    pinMode(LedBlink, OUTPUT);
  
  digitalWrite(ledEtaj[etajCurent-1],HIGH);
}


void loop() {
 
  if(blinkActive==false){ // daca liftul stationeaza, led-ul verde ramane aprins fara a clipii si aprindem led-ul de la etajul curent
    digitalWrite(LedBlink,HIGH);
}
  
  
  if(!blinkActive){ // nu luam in seama daca se apasa alt buton cand liftul se misca
    for (int i = 0; i < numarEtaje; i++) {
        if (VerificaDebounce(i) && etajCurent!=i+1) { // cazul in care apasam butonul de la etajul curent
            etajDestinatie = i + 1;
                      blinkActive = true;
                      tone(buzzer, 1000, 500);  // se inchid usile
// liftul trebuie sa plece, deci led-ul verde trebuie sa clipeasca

        }
    }
  }
  
  
    switch (stare) {
        case Asteptare:
            if (etajDestinatie != etajCurent) {
                stare = InchidereUsi;
                timpAnterior = millis();
            }
            break;
            
        case InchidereUsi: // inchidem usile si deplasam liftul
            if (millis() - timpAnterior > intervalUsi) {
                stare = InMiscare;
                timpAnterior = millis();
                tone(buzzer, 800); // buzzer continuu pana ajungem la etajul dorit
            }
            break;

case InMiscare:
   

    if (millis() - timpAnterior > TimpPeEtaj) { // timpul in care ne deplasam intre etaje
              digitalWrite(ledEtaj[etajCurent - 1], LOW);

        if (etajDestinatie > etajCurent) {
            etajCurent++;  
        } else {
            etajCurent--;  
        }
        digitalWrite(ledEtaj[etajCurent - 1], HIGH); // Aprindem led-ul unde este liftul

        if (etajCurent == etajDestinatie) {
          
            stare = DeschidereUsiLaDestinatie;
            timpAnterior = millis();
            tone(buzzer, 600, 500); // pentru cand se deschid usile cand a ajuns liftul
        } else {
            timpAnterior = millis();  // Reseteaza timerul pentru urmatorul etaj
        }
    }
    break;


        case DeschidereUsiLaDestinatie:
            if (millis() - timpAnterior > intervalUsi) {
                stare = Asteptare;
                blinkActive = false; // opriti blink-ul dupa ce liftul ajunge la etaj

            }
            break;
    }
  
  
  
  
  

    static unsigned long previousMillis = 0;
    static bool ledState = LOW;
    if (blinkActive && millis() - previousMillis > 100) { // Facem ca led-ul verde sa se aprinda succesiv cat timp liftul functioneaza
        previousMillis = millis();
        ledState = !ledState;
        digitalWrite(LedBlink, ledState);
    }
}

bool VerificaDebounce(int i){
bool apasat=false;
  int reading=digitalRead(butonEtaj[i]);
  if(millis()-TimpVerificareButon[i]>delayDebounce){
  
    if(reading!=StareButoane[i]){
      TimpVerificareButon[i]=millis();
      if(reading==LOW) 
        apasat=true;

    }
    
    StareButoane[i]=reading;
  }
  return apasat;

}