#include <TimerTCC0.h>

bool timer_100Hz = false;

void setup() {
  Serial1.begin(115200);

  TimerTcc0.initialize(10000);  //10,000us=100Hz
  TimerTcc0.attachInterrupt(timerIsr);
}

void loop() {
  if (timer_100Hz) {
    timer_100Hz = false;
    Serial1.print("1013.25,23.4,1.23,4.56,7.89,0.123,0.123\n");
    //気圧[hPa],温度[deg],気圧高度[m],差圧[Pa],対気速度[m/ss],AoA[rad],AoS[rad]
  }
}

void timerIsr() {
  if (timer_100Hz) {
    //Serial.println("100Hz overrun");
  } else {
    timer_100Hz = true;
  }
}
