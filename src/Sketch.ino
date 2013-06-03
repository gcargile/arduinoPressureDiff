#include "Fmap.h"
#include "Water.h"
#include "String.h"
#include "Analog.h"
#include "Conversion.h"
#include "Well.h"
#include "Spin.h"
#include "MPX5100.h"

#define TRANSDUCER_PIN A0

#define MIN_CHANGE 1 // TODO: this needs tweeked.
#define BAUD 115200
#define DELAY_MS 500

#define WELL_DIAMETER_INCHES 30
#define WELL_DEPTH_INCHES 120

int last_a_read;
int delta;

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
  
  delta = read_pressure_diff();

  pinMode(LED_PIN, OUTPUT);

  Serial.begin(BAUD); // Open serial port
  Serial.println(getConfigurationMessage());


}

void loop() {

  int current_a_read = read_pressure_diff();
  if (isNewValue(current_a_read)) {
    last_a_read = current_a_read;
    
    well.level = (int)getDepthInches(last_a_read);

    Serial.print(displayWell());
    Serial.println("");
  }

  spin(DELAY_MS);
  
}

String displayWell() {
  String retval;
  
  retval.concat(" Depth: ");
  retval.concat(well.level);
  retval.concat(" Gallons: ");
  retval.concat(getWellGallons(well));
  return retval;
}

int getDepthInches(int analogRead) {
  
  Serial.print("RAW Analog value:");
  Serial.print(analogRead);
  Serial.print(" ");
  
  if (analogRead >= delta) {
    analogRead = analogRead - delta;
    Serial.print("Delta Analog value:");
    Serial.print(analogRead);
    Serial.print(" ");
  }

  float volts = aToVolts(analogRead, 0, VOUT_MAX - VOUT_MIN);
  Serial.print(volts);
  Serial.print("v");


  float milliVolts = volts * 1000;
  float psi = milliVolts / mV_per_PSI; 
    
  float depthFeet = psi / PSI_PER_FOOT_FRESH;
  float depthInches = depthFeet / INCHES_PER_FOOT;

  return depthInches;
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
  retval.concat("Gallons/Foot:");
  retval.concat(gallons(volume(12, WELL_DIAMETER_INCHES)));
  retval.concat(newLine());
  retval.concat("Test Foot:");
  well.level = 12;
  retval.concat(displayWell());
  retval.concat(newLine());
  retval.concat("Delta Read:");
  retval.concat(delta);
  retval.concat(newLine());

  retval.concat(fillLine('*'));

  return retval;
}
