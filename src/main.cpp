#include <Arduino.h>

#define ANALOG_PIN  4
#define PULSE_PIN   18
#define PERIOD      999000
#define MSIZE       80
#define BSIZE       60
#define ISIZE       10

volatile int incident;
volatile int iqty;
volatile unsigned counter;
unsigned long sum_counter;
unsigned long time_to_report;
unsigned long time_to_measure;
unsigned long t1;
unsigned long t2;
unsigned long tmeasure1;
unsigned long tmeasure2;
unsigned long tinc;
int i;
int k;
int m;
int status;
int signal[MSIZE];
int bsignal[BSIZE];
unsigned long tincident[ISIZE];
int value;
int measure;
int b;
int befo;
float average;
int ignorecounter;

void fix_interrupt(){
  incident = 1;
}


void setup() {
  Serial.begin(115200);
  counter = 0;
  sum_counter = 0;
  attachInterrupt(digitalPinToInterrupt(PULSE_PIN), fix_interrupt, CHANGE);
  time_to_report = millis() + PERIOD;
  time_to_measure = 0UL -1;
  incident = 0;
  delay(1000);
  Serial.println(".");
  t2 = millis();
  k = 0;
  i = 0;
  m = 0;
  measure = 0;
  iqty = 0;
  befo = 1;
  status = 1;
  ignorecounter = 0;
}

void loop() {
  if (incident) {
    incident = 0;
    tinc = micros();
    if (counter < 10) {
      tincident[counter] = tinc;
    } else {
      time_to_measure = millis();
    }

    average = 3.3 / 4096 / 5 * (analogRead(ANALOG_PIN) + analogRead(ANALOG_PIN) + analogRead(ANALOG_PIN) + analogRead(ANALOG_PIN) + analogRead(ANALOG_PIN));
    Serial.print(average);
    if ((status == 1) && (average < 1.5)) {
      status = 0;
    } else if ((status == 0) && (average >= 1.5)) {
      counter++;
      status = 1;
    } else {
      ignorecounter++;
    }
    
    if (counter == 9) {
      time_to_measure = millis();
    }
    Serial.print(" ");
    Serial.println(counter);
  }
  

  if (time_to_report <= millis()) {
    time_to_report += PERIOD;
    sum_counter += counter;
    Serial.println();
    Serial.print(counter);
    Serial.print("; ");
    Serial.print(sum_counter);
    Serial.println();
    counter = 0;
  }

  
  if (time_to_measure <= millis()) {
    tmeasure1 = millis();
    for ( i = k; i < MSIZE; i++) {
      signal[i] = analogRead(ANALOG_PIN);
    }
    tmeasure2 = millis();

    //     ========================================     Вывод результатов измерений
    Serial.println();
    for ( i = b+1; i < BSIZE; i++) {
      for (int j = 0; j < bsignal[i] / 40; j++) {
        Serial.print("0");
      }
      Serial.print("   ");
      Serial.println(0.000806 * bsignal[i]);
    }

    for ( i = 0; i <= b; i++) {
      for (int j = 0; j < bsignal[i] / 40; j++) {
        Serial.print("0");
      }
      Serial.print("   ");
      Serial.println(0.000806 * bsignal[i]);
    }

    Serial.println("Инцидент");

    for ( i = k-5; i < MSIZE; i++) {
      for (int j = 0; j < signal[i] / 40; j++) {
        Serial.print("0");
      }
      Serial.print("   ");
      Serial.println(0.000806 * signal[i]);
    }

    for (size_t n = 0; n < k/5; n++) {
      Serial.println(" ");
      Serial.print("Инцидент ");
      Serial.print(n+1);
      Serial.print(": ");
      Serial.print(tincident[n]);
      Serial.println(" микросек.");
      
      for ( i = 0; i < 5; i++) {
        for (int j = 0; j < signal[i+5*n] / 40; j++) {
          Serial.print("0");
        }
        Serial.print("   ");
        Serial.println(0.000806 * signal[i+5*n]);
      }
    }
    
    Serial.println();
    Serial.print("tincidents: ");
    for (int j = 0; j < counter; j++) {
      Serial.print(tincident[j]);
      Serial.print(" ");
    }
    Serial.println();
    Serial.print(" tmeashure1=");
    Serial.print(tmeasure1);
    Serial.print(" tmeashure2=");
    Serial.print(tmeasure2);
    while (true) { }   
  }
  if (befo) {
    bsignal[m] = analogRead(ANALOG_PIN);
    b = m;
    m++;
    if (m >= BSIZE) {
      m = 0;
    }
  }
}