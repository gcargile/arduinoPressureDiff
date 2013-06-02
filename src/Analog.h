// Arduino A2D limits
#define ANALOG_MIN 0
#define ANALOG_MAX 1023

float aToVolts(int a, float vMin, float vMax) {
  return fmap(a, ANALOG_MIN, ANALOG_MAX, vMin, vMax);
}

