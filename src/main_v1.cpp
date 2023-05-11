#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

//0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN 100
#define SERVOMAX 300


int numbers[6] = {0, 1, 2, 3, 4, 5}; // array contenente i numeri possibili
int usedNumbers[6] = {0, 0, 0, 0, 0, 0}; // array per tenere traccia dei numeri già estratti
int count = 0; // contatore per tenere traccia del numero di numeri estratti

void setup() {
  pwm.begin();
  pwm.setPWMFreq(60);
  delay(100);
  Serial.begin(9600);

  int seed = analogRead(A0);
  randomSeed(seed);
}

void loop() {
  if (count < 6) {
    int index = -1;
    while (index == -1) {
      int num = random(0, 6); // estrai un numero casuale tra 0 e 5
      if (usedNumbers[num] == 0) { // se il numero non è stato già estratto
      
        usedNumbers[num] = 1; // segna il numero come già estratto
        index = num; // memorizza l'indice del numero estratto
        Serial.print("Ho estratto il numero: ");
        Serial.println(index);
        count++; // incrementa il contatore dei numeri estratti
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
    count = 0; // resetta il contatore dei numeri estratti
  }
}

// funzione per ripristinare l'array usedNumbers
void resetUsedNumbers() {
  Serial.println("RESET");
  for (int i = 0; i < 6; i++) {
    usedNumbers[i] = 0;
  }
}
