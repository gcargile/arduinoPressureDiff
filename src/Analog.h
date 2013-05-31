#define ANALOG_MIN 0
#define ANALOG_MAX 1023

#define VCC_MIN 0
#define VCC_MAX 5

#define A_PER_V ANALOG_MAX / VCC_MAX

float aToVolts(int a) {
  return (float)a * ((float)VCC_MAX / (float)ANALOG_MAX);
}

