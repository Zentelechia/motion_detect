
byte life_counter = 0;
byte reciever_send=0;
unsigned long m;

volatile unsigned long last_polling = 0;
unsigned long  powerbank_last_activation_time = 0;
unsigned long  powerbank_activation_interval = 3L * 3600UL * 1000UL; //3 hours

void idle_1s() {
    delay(1000);
}

void no_polling() {
  hm10.print(no_polling_cmd + transmitter_ID);
  delay(hm10_send_delay);
  sound_no_polling();
}

void device_add_sensor() {
  boolean sensor_connected = false;
  boolean exit_mode = false;
  int previous_button;
  if (digitalRead(button_pin) == LOW) {
    previous_button = LOW;
    while (!exit_mode && !sensor_connected) {
      tone(alarm_pin, 300);
      sleep_delay(mSLEEP_15MS);
      noTone(alarm_pin);
      sleep_delay(mSLEEP_250MS);

      if (digitalRead(button_pin) == HIGH) {
        previous_button = HIGH;
      }

      if (digitalRead(button_pin) == LOW && previous_button == HIGH) {
        exit_mode = true;
      }

      if (Serial.available()) {
        String s = Serial.readString();
        if (s.charAt(0) == id_cmd && isValidID(s)) {
          transmitter_ID = getId(s);
          write_StringEE(transmitter_ID_eeprom_address, transmitter_ID);
          sensor_connected = true;
          sensor_started();
        }
      }
    }
  }
}
volatile boolean life = false;


void setup() {
  device_sleep();
  device_add_sensor();
  device_init();
  powerbank_last_activation_time = millis();
  last_polling = millis();
}
void loop() {
  //work
  //->polling
  //->alarm
  //->sensor_init
  
  
  m=millis();
  if (m - powerbank_last_activation_time > powerbank_activation_interval) {
   activate_power_bank();
    powerbank_last_activation_time = m;
  }
  if ((m - last_polling) > polling_timeout) {
    last_polling=m;
    no_polling();
  }
  if (hm10.available()) {   
    String bls = hm10.readString();
    delay(100);
    if (bls == (pir_cmd + transmitter_ID)) {
      sound_alarm();
    }
    if (bls == "voltage") {
      hm10.print("power" + (String) + digitalRead(power_plugged_pin) + 'v' + (String) (battery_voltage * battery_k) + sensor_init_string);
      delay(hm10_send_delay);
    }
    if (bls == "ping") {
      Serial.print("ping" + transmitter_ID);
    }
  }
  if (Serial.available()) {
    blink_yellow(mSLEEP_30MS);
    String s = Serial.readString();
    if (s.indexOf(pir_cmd + transmitter_ID) > -1) {
      hm10.print(s);
      delay(hm10_send_delay);
      last_polling = m;
      sound_alarm();
    }
    if (s.indexOf(id_cmd + transmitter_ID) > -1) {
      hm10.print(s);
      delay(hm10_send_delay);
//      Serial.print(looking_for_transmitter_cmd + transmitter_ID);
      delay(hc12_SEND_DELAY);
      last_polling = m;
      sensor_init_string = s;
      sensor_started();
    }
    if (s.indexOf(polling_cmd + transmitter_ID) > -1) {
      hm10.print(s);
      delay(hm10_send_delay);
      last_polling = m;
//      delay(1000);
      sound_polling();
    }
  }
  sleep_if_button_5s_pressed();
  idle_1s();
  life_counter++;
  if (life_counter == 8) {
    show_battery_status();
    life_counter = 0;
  }
  if (reciever_send%30==0){
      String m= (String) + "power"+digitalRead(power_plugged_pin) + 'v' + (String) (battery_voltage * battery_k);
     hm10.print(m);
     delay(hm10_send_delay);
   }
   reciever_send++;
}




