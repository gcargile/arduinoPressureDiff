// Arduino A2D limits
#define ANALOG_MIN 0
#define ANALOG_MAX 1023

// MPX5100 datasheet values
#define VCC_MIN 0.2   
#define VCC_MAX 4.7

/*
 * This function is identical to the Arduino map but uses floats.
 */
float fmap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float aToVolts(int a) {
  return fmap(a, ANALOG_MIN, ANALOG_MAX, VCC_MIN, VCC_MAX);
}

