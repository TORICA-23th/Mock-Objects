#include <TimerTCC0.h>

bool timer_10Hz = false;

void setup() {
  Serial1.begin(115200);

  TimerTcc0.initialize(100000);  //100,000us=10Hz
  TimerTcc0.attachInterrupt(timerIsr);
}

void loop() {
  if (timer_10Hz) {
    timer_10Hz = false;
    Serial1.print("$GPGGA,045104.000,3014.1985,N,09749.2873,W,1,09,1.2,211.6,M,-22.5,M,,0000*62\r\n");
  }
}

void timerIsr() {
  if (timer_10Hz) {
    //Serial.println("100Hz overrun");
  } else {
    timer_10Hz = true;
  }
}
