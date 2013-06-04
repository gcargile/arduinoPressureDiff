#include <LiquidCrystal.h>
#include <pt.h>

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

#define WELL_NAME "GoatWell"
#define WELL_DIAMETER_INCHES 30
#define WELL_DEPTH_INCHES 120

#define APP_NAME "APD v1.0"

int last_a_read = 0;
int delta = 0;
int high = 0;
int low = 0;
int dirty = false;

Well well;

static struct pt ptFeedback, ptRead;

LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);

static int threadFeedback(struct pt *pt, long timeout) {
  static long t1 = 0;
  PT_BEGIN(pt);
  while (true) {
    PT_WAIT_UNTIL(pt, (millis() - t1) > timeout);
    t1 = millis();

    //do feeback now;
    //lcd.clear();
    //lcd.setCursor(0, 0);
    //lcd.print(getAppDescription());
    spinner(lcd, getAppDescription());
    if (dirty) {
      lcd.setCursor(0, 1);
      lcd.print(fmtDepth());
      lcd.setCursor(0, 2);
      lcd.print(fmtStats());
      lcd.setCursor(0, 3);
      lcd.print(fmtGallons());
      dirty = false;
    }
    //Serial.println(fmtGallons());
    //Serial.print(displayWell());
    //Serial.println("");

  }
  PT_END(pt);
}

static int threadRead(struct pt *pt, long timeout) {
  static long t1 = 0;
  PT_BEGIN(pt);
  while (true) {
    PT_WAIT_UNTIL(pt, (millis() - t1) > timeout);
    t1 = millis();

    //do read now;
    int current_a_read = read_pressure_diff();
    if (isNewValue(current_a_read)) {
      last_a_read = current_a_read;
      dirty = true;

      // new delta if we have a new low point
      if (delta < current_a_read)
	delta = current_a_read;

      saveStats(getDepthInches(current_a_read - delta));
      well.level = getDepthInches(current_a_read);
    }
  }
  PT_END(pt);
}

void setup() {

  lcd.begin(20, 4);

  // Initialize LCD..
  lcd.noAutoscroll();
  lcd.clear();  

  // Initialize values for Well
  well.name = WELL_NAME;
  well.depth = WELL_DEPTH_INCHES;
  well.diameter = WELL_DIAMETER_INCHES;
  well.level = 0;

  // Establish a delta (0 pressure reading)
  delta = read_pressure_diff();

  // Onboard LED is just a heartbeat.
  pinMode(LED_PIN, OUTPUT);

  // Only for debug.
  Serial.begin(BAUD); // Open serial port
  Serial.println(getConfigurationMessage());

  // Initialize ProtoThreads Library.
  PT_INIT(&ptFeedback);
  PT_INIT(&ptRead);
}

void loop() {
  threadRead(&ptRead, 700);
  threadFeedback(&ptFeedback, 500);
}

String getAppDescription() {
  String retval;
  retval.concat(APP_NAME);
  retval.concat(" ");
  retval.concat(well.name);
  return retval;
}

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
  return ab > MIN_CHANGE;
}

void saveStats(int newValue) {
  
  if (newValue > high)
    high = newValue;
  
  if (newValue < low)
    low = newValue;
}

String fmtStats() {
  String retval;
  retval.concat("D:");
  retval.concat(delta);
  retval.concat(" ");
  retval.concat("H:");
  retval.concat(high);
  retval.concat(" ");
  retval.concat("L:");
  retval.concat(low);
  return retval;
}

String fmtDepth() {
  String retval;
  retval.concat("Depth: ");
  retval.concat(well.level);
  retval.concat(" inches");
  return retval;
}

String fmtWater(int gallons) {
  String retval;
  retval.concat("Gals: ");
  retval.concat(gallons);
  return retval;
}

String fmtGallons() {
  return fmtWater(getWellGallons(well));
}

String displayWell() {
  String retval;
  retval.concat(fmtDepth());
  retval.concat(newLine());
  retval.concat(fmtGallons());
  retval.concat(newLine());
  return retval;
}

float getSensorVolts(int aValue) {
  return aToVolts(aValue, 0, VOUT_MAX - VOUT_MIN);
}

int getDepthInches(int analogRead) {
  
  //Serial.print("RAW Analog value:");
  //Serial.print(analogRead);
  //Serial.print(" ");
  
  if (analogRead >= delta) {
    analogRead = analogRead - delta;
    //Serial.print("Delta Analog value:");
    //Serial.print(analogRead);
    //Serial.print(" ");
  }

  float volts = getSensorVolts(analogRead);
  //Serial.print(volts);
  //Serial.print("v");


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
  retval.concat("Delta Read:");
  retval.concat(delta);
  retval.concat(newLine());

  retval.concat(fillLine('*'));

  return retval;
}
