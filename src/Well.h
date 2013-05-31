typedef struct {
  int depth;
  int diameter;
  int level;
} Well;

long getWellVolume(Well well) {
  return volume(well.level, well.diameter);
}

int getWellGallons(Well well) {
  return gallons(getWellVolume(well));
}
