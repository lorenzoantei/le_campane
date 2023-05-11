#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define DEBUG 1
#define pinLedBuiltIn 13

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN 100
#define SERVOMAX 300

#define servoNum 6
int usedServo[servoNum] = {0, 0, 0, 0, 0, 0}; // array per tenere traccia dei numeri già estratti
int counterUsedServo = 0; // contatore per tenere traccia del numero di numeri estratti

#define pirPin 8
// bool isAnybody = true; // init PIR status

void setup() {
  if (DEBUG) pinMode(pinLedBuiltIn, OUTPUT);
  pinMode(pirPin, INPUT);

  int seed = analogRead(A0);
  randomSeed(seed);
  
  pwm.begin();
  pwm.setPWMFreq(60);
  delay(100);

  if (DEBUG) Serial.begin(9600);
  if (DEBUG) Serial.println("READY");
  
}

// funzione per ripristinare l'array usedNumbers
void resetUsedNumbers() {
  if (DEBUG) Serial.println("RESET");
  for (int i = 0; i < 6; i++) {
    usedServo[i] = 0;
  }
}

void loop() {
  bool isAnyone = digitalRead(pirPin);

  if (isAnyone) {
    int cycle = random(2, 5); // imposto quante volte si deve ripeter il ciclo
    if (DEBUG) Serial.println();
    if (DEBUG) Serial.print("Ho estratto cicli #");
    if (DEBUG) Serial.println(cycle);

    resetUsedNumbers();
    for (int i=0; i<(servoNum+1)*cycle; i++) {

      if (counterUsedServo < servoNum) {
        int index = -1; // inizializza l'indice del servo a -1
        while (index == -1) { // continua a selezionare numeri casuali finché non viene trovato un numero valido
          int num = random(0, 6); // estrai un numero casuale tra 0 e 5
          if (usedServo[num] == 0) { // se il numero non è stato già estratto
          
            usedServo[num] = 1; // segna il numero come già estratto
            index = num; // memorizza l'indice del numero estratto
            if (DEBUG) Serial.print("Ho estratto il numero: ");
            if (DEBUG) Serial.println(index);
            counterUsedServo++; // incrementa il contatore dei numeri estratti
          }
        }

        int servonum = index; // utilizza l'indice del numero come indice del servo motore

        for (int a = SERVOMIN; a <SERVOMAX; a++){ //dalla posizione di riposo a quella di finale
          pwm.setPWM(servonum, 0, a);
          delay(1);
        }

        delay(random(30,50));

        for (int a = SERVOMAX; a >SERVOMIN; a--){ // dalla posizione finale a quella di riposo
          pwm.setPWM(servonum, 0, a);
          delay(1);
        }

        delay(random(30,50));
      } else { // tutti i numeri sono stati estratti
        resetUsedNumbers(); // ripristina l'array usedNumbers
        counterUsedServo = 0; // resetta il contatore dei numeri estratti
      }

    }

  } else {
    if (DEBUG) Serial.println("\nnon c'è nessuno");
    delay(1000);
  }

}
