String transmitter_ID;
String sensor_init_string = "";

volatile unsigned long millis_from_button_pressed = 0;
volatile int prev_button_status = HIGH;
const unsigned int millis_to_wakeup = 4000;
const unsigned int debounce_microseconds = 20000;
const byte s2 = 0b000111;
void(* resetFunc) (void) = 0;
byte device_ID_length = device_ID.length();

void button_interrupt() {
}
void prepare_after_wake_up() {
  pinMode(hc12_set_pin, OUTPUT);
#if  reciever == true
  pinMode(alarm_pin, OUTPUT);
#else
  pinMode(pir_pin, INPUT);
#endif
  pinMode(button_pin, INPUT);
  digitalWrite(alarm_pin, LOW);
  hc12_init();
}


// Показываем статус аккумуляторы: если ОК - длинное включение светодиода, если средне - среднее, малый заряд - короткое моргание

void update_battery_voltage() {
  pinMode(bat_voltage_measure_enable_pin, OUTPUT);
  digitalWrite(bat_voltage_measure_enable_pin, HIGH);
  pinMode(bat_voltage_measure_pin,INPUT);
  delay(50);
  battery_voltage = analogRead(bat_voltage_measure_pin);  //* 0.0064453; //1024.0 * 3.3  *2;
  delay(50);
  digitalWrite(bat_voltage_measure_enable_pin, LOW);
}
void turn_5v_on() {
  pinMode(DC5_enable_pin, OUTPUT);
  digitalWrite(DC5_enable_pin, HIGH);
}

void turn_5v_off() {
  pinMode(DC5_enable_pin, OUTPUT);
  digitalWrite(DC5_enable_pin, LOW);
}


void activate_power_bank() {
  pinMode(power_bank_activation_pin, OUTPUT);
  digitalWrite(power_bank_activation_pin, HIGH);
  delay(1000);
  digitalWrite(power_bank_activation_pin, LOW);
  pinMode(power_bank_activation_pin, INPUT);
  digitalWrite(power_bank_activation_pin, LOW);

}


void sleep_if_button_5s_pressed() {
  if (digitalRead(button_pin) == LOW) {
    delay(5000);
    if (digitalRead(button_pin) == LOW) {
      
      #ifndef reciver
      hc12_wakeup();
      Serial.print(switched_off_cmd + device_ID + 'b' + (String) (battery_voltage * battery_k));
      delay(hc12_SEND_DELAY);
      #endif
      
      blink_red(100);
      delay(100);
      blink_red(100);
      offline_sound();
      resetFunc();
    }
  }

}


void button_2_isr() {
  sleep_disable();
  detachInterrupt (1);
}


void show_battery_status() {
  update_battery_voltage();
  if (battery_voltage > battery_high_voltage) {
    blink_green(blink_duration);
  } else if (battery_voltage > battery_medium_voltage) {
    blink_yellow(blink_duration);
  }
  else {
    blink_red(blink_duration);

  }
}




void goto_sleep() {
  tone(alarm_pin, Mi);
  sleep_delay(mSLEEP_120MS);
  tone(alarm_pin, Re);
  sleep_delay(mSLEEP_120MS);
  tone(alarm_pin, Do);
  sleep_delay(mSLEEP_120MS);
  noTone(alarm_pin);
  resetFunc();
}

String getId(String input) {
  String s = "";
  int i;
  for (i = 1; i < device_ID_length + 1; i++) {
    s += input.charAt(i);
  }
  return s;
}

boolean isValidID(String str) {
  for (byte i = 1; i < device_ID_length + 1; i++)
  {
    if (isDigit(str.charAt(i))) return true;
  }
  return false;
}


