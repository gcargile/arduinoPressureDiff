#define INCHES_PER_FOOT 12

int feetFromInches(int inches) {
  return inches / INCHES_PER_FOOT;
}

#define PI 3.14
#define CI_IN_A_GALLON 231

long volume(int depth_in, int diameter_in) {
  return (depth_in * PI * pow((diameter_in / 2), 2));
}

int gallons(long volume) {
  return (volume / CI_IN_A_GALLON);
}


