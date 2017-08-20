void device_sleep() { 
  led_off(red_led_pin);
  led_off(green_led_pin);

  byte adcsra_save = ADCSRA;
  /*
  for (int i = 0; i < 20; i++ ) {
    pinMode(i, OUTPUT);
  }
  */
  digitalWrite(power_bank_activation_pin, LOW);
  
  turn_5v_off();
  hc12_sleep();
#ifdef reciever
  hm10_sleep();
 /* 
  pinMode(hm10_key_pin,OUTPUT);
  pinMode(alarm_pin,OUTPUT);
  digitalWrite(hm10_key_pin,HIGH);
  */
#endif
  pinMode(button_pin, INPUT);
  //wdt_disable();
  boolean state_at_start = digitalRead(button_pin);
  while (sleeping) {
    noInterrupts ();
    sleep_enable();
    attachInterrupt (1, button_2_isr, LOW);
    MCUSR = 0;
    noInterrupts ();
    // сбрасываем различные флаги
    byte adcsra_save = ADCSRA;
    ADCSRA = 0;  // запрещаем работу АЦП
    //Disable ADC
    power_all_disable ();   // выключаем все модули
    set_sleep_mode (SLEEP_MODE_PWR_DOWN);   // устанавливаем режим сна
    sleep_enable();
    interrupts ();
    sleep_cpu ();
    sleep_disable();
    power_all_enable();
    delay(millis_to_wakeup);
    if (digitalRead(button_pin) == LOW && state_at_start == HIGH) {
      sleeping = false;
    }
    if (digitalRead(button_pin) == HIGH ) {
      state_at_start == HIGH;
    }
  }
#ifdef reciever
  t();
#endif
  detachInterrupt (digitalPinToInterrupt(button_pin));     // останавливаем прерывание LOW
  ADCSRA = adcsra_save;  // останавливаем понижение питания
  power_all_enable ();   // включаем все модули
  blink_green(100);
  delay(100);
  blink_green(100);
  prepare_after_wake_up();

}



void device_init() {
  turn_5v_on();
  analogReference(DEFAULT);
  pinMode(power_plugged_pin, INPUT);
  pinMode(button_pin, INPUT);
  check_leds();
  show_battery_status();
#if reciever==true
//  hm10_wakeup();
  hm10.println(hm10_name_cmd + hm10_name_prefix + device_ID);
  sleep_delay(mSLEEP_250MS);
  
  String tr_ID=read_StringEE(transmitter_ID_eeprom_address, device_ID_length);
  if (isValidID(tr_ID)){
    transmitter_ID = tr_ID;
  }
  else {
    transmitter_ID = device_ID;
    write_StringEE(transmitter_ID_eeprom_address, transmitter_ID);
  }
  Serial.print(looking_for_transmitter_cmd + transmitter_ID);
  delay(hc12_SEND_DELAY);
  delay(1000);
  hm10.print(battery_voltage);
  if (Serial.available()) {
    String s = Serial.readString();
    if (s.indexOf(id_cmd + transmitter_ID) > -1) {
      hm10.print(s);
      sensor_init_string = s;
      sensor_started();
    }
  }
#else
  pinMode(pir_pin, INPUT);
  digitalWrite(pir_pin, LOW);
  attachInterrupt (digitalPinToInterrupt(button_pin), button_interrupt, FALLING);   // позволяем заземлить pin 2 для выхода из сна
#endif
}


