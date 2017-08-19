#define red_led_pin                     A4
#define green_led_pin                   13

#define common_anode true

#ifdef common_anode
int led_on_state = LOW;
int led_off_state = HIGH;
#else
int led_on_state = HIGH;
int led_off_state = LOW;
#endif


void led_on(int pin) {
  digitalWrite(pin, led_on_state);
}

void led_off(int pin) {
  digitalWrite(pin, HIGH);
}

void blink_red(int duration) {
  led_on(red_led_pin);
  delay(duration);
  led_off(red_led_pin);
}

void blink_green(int duration) {
  led_on(green_led_pin);
  delay(duration);
  led_off(green_led_pin);
}

void blink_yellow(int duration) {
  led_on(red_led_pin);
  led_on(green_led_pin);
  delay(duration);
  led_off(red_led_pin);
  led_off(green_led_pin);
}

void check_leds() {
  blink_green(500);
  blink_red(500);
}

