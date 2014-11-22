#include <Arduino.h>
#define LED_PIN        13  // pin tied to onboard LED.

/*
 * Spin provides some feedback that the application is actualy still running.
 * The Serial interface is sent a series of characters and backspace.
 * The onboard led is turned on/off.
 */
void spin(int howLong) {
  char spins[] = { '-', '/', '.', '\\', '*',
                   '_', '(', ',', ')', '+'};
  int i;
  for(i = 0; i < 10; i ++) {
    if (i % 2 > 0)
      digitalWrite(LED_PIN, HIGH);
    else
      digitalWrite(LED_PIN, LOW);

    Serial.print(spins[i]);
    delay(howLong / 10);
    Serial.print(char(8));
  }
}

void spinLcdMsg(LiquidCrystal lc, int howLong, String msg) {

  char spins[] = { '-', '/', '.', '\\', '*',
                   '_', '(', ',', ')', '+'};
  int i;
  for(i = 0; i < 10; i ++) {
    lc.setCursor(0, 0);

    if (i % 2 > 0) {
      lc.print(spins[i]);
      lc.print(msg);
      digitalWrite(LED_PIN, HIGH);
    }
    else {
      digitalWrite(LED_PIN, LOW);
    }

    delay(howLong / 10);

  }

}

static char spins[] = { '-', '/', '.', '\\', '*',
			'_', '(', ',', ')', '+'};

static int spinNum = 0;

void spinner(LiquidCrystal lc, String msg) {
  if (spinNum >= sizeof(spins))
    spinNum = 0;

  if (spinNum % 2 > 0)
    digitalWrite(LED_PIN, HIGH);
  else
    digitalWrite(LED_PIN, LOW);

  lc.setCursor(0, 0);

  lc.print(spins[spinNum]);
  lc.print(msg);
  spinNum++;
}
