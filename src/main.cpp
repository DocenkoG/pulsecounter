#include <Arduino.h>

#define PULSE_PIN   23
#define PERIOD      10000

volatile int incident;
int first_incident;
volatile unsigned counter;
unsigned long sum_counter;
unsigned long time_to_report;
unsigned long t1;

void fix_interrupt(){
  //counter += 1;
  incident = 1;
}


void setup() {
  Serial.begin(115200);
  Serial.println(".");
  counter = 0;
  sum_counter = 0;
  attachInterrupt(digitalPinToInterrupt(PULSE_PIN), fix_interrupt, FALLING);
  time_to_report = millis() + PERIOD;
  incident = 0;
  first_incident = 1;
}


void loop() {
  if (incident) {
    incident = 0;
    if (first_incident) {
      first_incident = 0;
      counter += 1;
      t1 = millis();
      Serial.print("Инцидент ");
      Serial.print(counter);
      Serial.print(". ");
    }
  }
    
  if (87 < millis() - t1) {
    first_incident = 1;
  }
  
  if (time_to_report <= millis()) {
    sum_counter += counter;
    Serial.println();
    Serial.print(counter);
    Serial.print("; ");
    Serial.print(sum_counter);
    Serial.println();
    counter = 0;
    time_to_report += PERIOD;
  }
}