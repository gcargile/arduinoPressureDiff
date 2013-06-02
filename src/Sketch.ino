#include "Fmap.h"
#include "MPX5100.h"
#include "String.h"
#include "Analog.h"
#include "Conversion.h"
#include "Well.h"
#include "Spin.h"

#define TRANSDUCER_PIN A0
#define LED_PIN        13  // pin tied to onboard LED.

#define MIN_CHANGE 1 // TODO: this needs tweeked.
#define BAUD 115200
#define DELAY_MS 500

#define WELL_DIAMETER_INCHES 30
#define WELL_DEPTH_INCHES 120

int last_a_read;
Well well;

int empty_value() {
  return ANALOG_MIN;
}

int full_value() {
  return ANALOG_MAX;
}

int read_pressure_diff() {
  return analogRead(TRANSDUCER_PIN);
}

int normalize(int value) {
  return constrain(value, empty_value(), full_value());
}

int percentage(int value) {
  return map(value, empty_value(), full_value(), 0, 100);
}

int isNewValue(int test) {
  int diff = last_a_read - test;
  int ab = abs(diff);
  //  Serial.print("abs:");
  //  Serial.print(ab);
  return ab > MIN_CHANGE;
}

void setup() {
  well.depth = WELL_DEPTH_INCHES;
  well.diameter = WELL_DIAMETER_INCHES;
  well.level = 0;

  pinMode(LED_PIN, OUTPUT);

  Serial.begin(BAUD); // Open serial port
  Serial.println(getConfigurationMessage());
  //  Serial.print("Empty:");
  //  Serial.print(aToVolts(ANALOG_MIN));
  //  Serial.print(" Mid:");
  //  Serial.print(aToVolts(512));
  //  Serial.print(" Ful:");
  //  Serial.println(aToVolts(ANALOG_MAX));

}

void loop() {
  
  digitalWrite(LED_PIN, HIGH);

  int current_a_read = read_pressure_diff();

  if (isNewValue(current_a_read)) {
    last_a_read = current_a_read;
    //Serial.println("");
    //Serial.print("current_a_read:");
    //Serial.print(current_a_read);
    //Serial.print(" ");
    //Serial.print("last_a_read:");
    //Serial.print(last_a_read);
  
    //Serial.print("Well at ");
    //Serial.print(percentage(normalize(current_a_read)));
    //Serial.println("%");
    Serial.print(aToVolts(read_pressure_diff(), VOUT_MIN, VOUT_MAX));
    Serial.println("v");
  }
  digitalWrite(LED_PIN, LOW);
  spin(DELAY_MS);

  
}

String getConfigurationMessage() {
  String retval;
  retval.concat(fillLine('*'));
  retval.concat("Water Depth Measure Tool v1.0\n\r");
  retval.concat("ANALOG PIN:");
  retval.concat(TRANSDUCER_PIN);
  retval.concat(newLine());
  retval.concat("Delay each cycle:");
  retval.concat(DELAY_MS);
  retval.concat(newLine());
  retval.concat("Well diameter:");
  retval.concat(well.diameter);
  retval.concat(newLine());
  retval.concat("Well depth:");
  retval.concat(well.depth);
  retval.concat(newLine());
  retval.concat("Capacity CIN:");
  retval.concat(volume(WELL_DEPTH_INCHES, WELL_DIAMETER_INCHES));
  retval.concat(newLine());
  retval.concat("Capacity GAL:");
  retval.concat(gallons(volume(WELL_DEPTH_INCHES, WELL_DIAMETER_INCHES)));
  retval.concat(newLine());
  retval.concat("Gallons/Inch:");
  retval.concat(gallons(volume(1, WELL_DIAMETER_INCHES)));
  retval.concat(newLine());
  retval.concat(fillLine('*'));

  return retval;
}
