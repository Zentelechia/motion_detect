void sound_no_polling() {
  tone(alarm_pin, 2000);
  sleep_delay(mSLEEP_250MS);
  noTone(alarm_pin);
}
void sound_alarm() {
  int i = 0;
  int k = 0;
  int j = 0;
  boolean flag = true;
  pinMode(alarm_pin, OUTPUT);
  led_on(red_led_pin);
  for (k = 0; k < 3; k++) {
    j = 0;
    if (flag) {
      tone(alarm_pin, 300, 200);
      while (j < 300) {
        if (digitalRead(button_pin) == LOW) {
          flag = false;
          break;
          j = 301;
        }
        sleep_delay(mSLEEP_15MS);
        j++;
      }
    }
  }
  while (flag) {

    
    for (i = 300; i < 500; i++) {
      if (digitalRead(button_pin) == LOW) {
        flag = false;
        break;
      }
      tone(alarm_pin, i);
      sleep_delay(mSLEEP_15MS);
    }

    for (i = 500; i > 300; i--) {
      if (digitalRead(button_pin) == LOW) {
        flag = false;
        break;
      }
      tone(alarm_pin, i);
      sleep_delay(mSLEEP_15MS);
    }
     noTone(alarm_pin);
     for (i = 0; i <45; i++) {
      if (digitalRead(button_pin) == LOW) {
        flag = false;
        break;
      }
      sleep_delay(mSLEEP_60MS);
    }
    
  }
  noTone(alarm_pin);
  led_off(red_led_pin);
  pinMode(alarm_pin, INPUT);

}

void sound_polling() {
  tone(alarm_pin, Do);
  delay(120);
  //sleep_delay(mSLEEP_120MS);
  tone(alarm_pin, Re);
  delay(120);
  //sleep_delay(mSLEEP_120MS);
  tone(alarm_pin, Mi);
  sleep_delay(mSLEEP_120MS);
  noTone(alarm_pin);
}

void t() {
  //pinMode(alarm_pin, OUTPUT);
  tone(alarm_pin, 400);
  delay(100);
  noTone(alarm_pin);
  delay(100);
}
void sensor_started() {
//  pinMode(alarm_pin, OUTPUT);
  tone(alarm_pin, Do);
  sleep_delay(mSLEEP_120MS);
  tone(alarm_pin, Mi);
  sleep_delay(mSLEEP_120MS);
  tone(alarm_pin, Sol);
  sleep_delay(mSLEEP_120MS);
  noTone(alarm_pin);
//  pinMode(alarm_pin, INPUT);

}
