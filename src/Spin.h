void spin(int howLong) {
  char spins[] = { '-', '/', '.', '\\', '*',
                   '_', '(', ',', ')', '+'};
  int i;
  for(i = 0; i < 10; i ++) {
    Serial.print(spins[i]);
    delay(howLong / 10);
    Serial.print(char(8));
  }
}

