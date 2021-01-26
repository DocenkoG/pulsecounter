#include <Arduino.h>

#define PULSE_PIN   23
#define PERIOD      20000
#define MSIZE       1000
#define BSIZE       80

volatile int incident;
volatile int iqty;
int first_incident;
volatile unsigned counter;
unsigned long sum_counter;
unsigned long time_to_report;
unsigned long t1;
unsigned long t2;
unsigned long k;
unsigned long tmeasure1;
unsigned long tmeasure2;
unsigned long tincident;
int i;
unsigned long signal[MSIZE];
unsigned long bsignal[BSIZE];
int value;
int measure;
int b;
int befo;

void fix_interrupt(){
  //counter += 1;
  incident = 1;
  measure = 1;
  //if (iqty) {
  //  measure = 1;
  //}
  //iqty++;
}


void setup() {
  Serial.begin(115200);
  counter = 0;
  sum_counter = 0;
  attachInterrupt(digitalPinToInterrupt(PULSE_PIN), fix_interrupt, FALLING);
  time_to_report = millis() + PERIOD;
  incident = 0;
  first_incident = 1;
  delay(1000);
  Serial.println(".");
  t2 = millis();
  k = 0;
  i = 0;
  measure = 0;
  iqty = 0;
  befo = 1;
}


void loop() {
  if (incident) {
    incident = 0;
    tincident = millis();
    counter += 1;
  }
  if (measure) {
    delay(86);
    tmeasure1 = millis();
    for ( i = 0; i < MSIZE; i++) {
      signal[i] = analogRead(4);
    }
    tmeasure2 = millis();
    Serial.println();
    for ( i = b+1; i < BSIZE; i++) {
      for (int j = 0; j < bsignal[i] / 40; j++) {
        Serial.print("0");
      }
      Serial.print("   ");
      Serial.println(0.000806 * bsignal[i]);
    }

    for ( i = 0; i < b; i++) {
      for (int j = 0; j < bsignal[i] / 40; j++) {
        Serial.print("0");
      }
      Serial.print("   ");
      Serial.println(0.000806 * bsignal[i]);
    }

    Serial.println("Инцидент");

    for ( i = 0; i < MSIZE; i++) {
      for (int j = 0; j < signal[i] / 40; j++) {
        Serial.print("0");
      }
      Serial.print("   ");
      Serial.println(0.000806 * signal[i]);
    }
    Serial.println();
    Serial.print("tincident=");
    Serial.print(tincident);
    Serial.print(" tmeashure1=");
    Serial.print(tmeasure1);
    Serial.print(" tmeashure2=");
    Serial.print(tmeasure2);
    while (true) { /* The end */ }   
  }
  if (befo) {
    bsignal[i] = analogRead(4);
    b = i;
    i++;
    if (i >= BSIZE) {
      i = 0;
    }
  }
    
}