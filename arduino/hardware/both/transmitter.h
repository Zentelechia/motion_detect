
#ifdef DEBUG
int pass_first_alarms = 1; // (2min=120sec-5 ~startup_time / 8s)
#else
int pass_first_alarms = 13; // (2min=120sec-5 ~startup_time / 8s)
#endif

byte s = s8;

volatile boolean pir_alarm = false;
volatile int sleeps_count = 0;
boolean polling = false;
const int sleep_interval = 10 * 60 ; //sec
const int sleep_cycles = (int) sleep_interval / 8;
unsigned long time;

void pir_interrupt () {
  pir_alarm = true;
  wdt_disable();
}



// прерывание сторожевого таймера
ISR (WDT_vect)
{

  wdt_disable();  // отключаем сторожевой таймер
}

void sleep_and_react (const byte interval) {
  if (pir_alarm && digitalRead(pir_pin) == HIGH && !polling) {
    detachInterrupt (0);
    hc12_wakeup();
    Serial.print(pir_cmd + device_ID);
    delay(hc12_SEND_DELAY);
    hc12_sleep();
    blink_red(1000);
    pir_alarm = false;
  }
  else {
    if (pass_first_alarms > 0) {
      blink_yellow(40);
      delay(40);
      blink_yellow(40);
    }
    else {
        show_battery_status();
    }
    if (polling) {
      polling = false;
      s = s8;
    }
    sleeps_count += 1;
    if (sleeps_count > sleep_cycles) {
      polling = true;
      sleeps_count = 0;
      hc12_wakeup();
      Serial.print(polling_cmd + device_ID + 'b' + (String) (battery_voltage * battery_k));
      delay(hc12_SEND_DELAY);
      hc12_sleep();
      s = s8;
    }
  }
  noInterrupts ();
  MCUSR = 0;                          // сбрасываем различные флаги
  WDTCSR |= 0b00011000;               // устанавливаем WDCE, WDE
  WDTCSR =  0b01000000 | interval;    // устанавливаем WDIE, и соответсвующую задержку
  wdt_reset();

  byte adcsra_save = ADCSRA;
  ADCSRA = 0;  // запрещаем работу АЦП
  //Disable ADC
  power_all_disable ();   // выключаем все модули
  set_sleep_mode (SLEEP_MODE_PWR_DOWN);   // устанавливаем режим сна
  sleep_enable();
  if (pass_first_alarms == 0 && !polling) {
    attachInterrupt (0, pir_interrupt, RISING);   // позволяем заземлить pin 2 для выхода из сна
  }
  attachInterrupt (digitalPinToInterrupt(button_pin), button_interrupt, FALLING);   // позволяем заземлить pin 2 для выхода из сна
  interrupts ();
  sleep_cpu ();
  // переходим в сон и ожидаем прерывание
  detachInterrupt (0);     // останавливаем прерывание LOW
  ADCSRA = adcsra_save;  // останавливаем понижение питания
  power_all_enable ();   // включаем все модули
  if (pass_first_alarms > 0) {
    pass_first_alarms--;
  }
  else {
    led_off(red_led_pin);
  }
}
void wait_for_reciever() {
  boolean reciever_offline = true;
  update_battery_voltage();
  led_on(red_led_pin);
  led_on(green_led_pin);
  Serial.print(id_cmd + device_ID + 'b' + (String) (battery_voltage * battery_k));
  delay(hc12_SEND_DELAY);
  if (Serial.available()) {
    String s = Serial.readString();
    if (s.indexOf(looking_for_transmitter_cmd + device_ID) > -1) {
      reciever_offline = false;
    }
  }
  if (reciever_offline) {
    while (reciever_offline) {
      sleep_if_button_5s_pressed();
      if (Serial.available()) {
        String s = Serial.readString();
        if (s.indexOf(looking_for_transmitter_cmd + device_ID) > -1) {
          led_off(red_led_pin);
          led_off(green_led_pin);
          reciever_offline = false;
          Serial.print(id_cmd + device_ID + 'b' + (String) (battery_voltage * battery_k));
          delay(hc12_SEND_DELAY);
        }
        delay(500);
      }
    }
  }
}

void setup () {
  //sleep
  //init
  //->wait for reciever
  device_sleep();
  wait_for_reciever();
  device_init();
}
void loop()
{
  //work
  //->alarm
  //->polling
  //->shutdown
  //->work_status
  //->
  sleep_and_react (s);
  sleep_if_button_5s_pressed();
}





