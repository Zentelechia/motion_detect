#define hc12_set_pin                    A1
#define hc12_enable_pin                 A3

#define hc12_SLEEP     "AT+SLEEP\r\n"
#define hc12_SET_DEFAULT   "AT+DEFAULT\r\n"
#define hc12_SET_FU    "AT+FU4\r\n"
#define hc12_SET_CH    "AT+C055\r\n"
#define hc12_SET_PDB   "AT+P8\r\n"
#define hc12_WAKEUP    "AT\r\n"

#define AT_RESPONSE_DELAY mSLEEP_500MS    //Delay after sending AT cmd to HC-12
#define AT_EXIT_DELAY     mSLEEP_250MS     //Delay after  setting SET pin of HC-12 to LOW or HIGH
#define hc12_SEND_DELAY  mSLEEP_1S    //Delay after sending data (for FU4 - minimum 1s)



void hc12_init() {
#ifdef reflash
  byte hc12_setted = EEPROM.read(hc12_setted_eeprom_address);
  if (hc12_setted != 1) {
#endif
    pinMode(hc12_enable_pin, OUTPUT);
    digitalWrite(hc12_enable_pin,HIGH);
    Serial.begin(9600);
    sleep_delay(hc12_SEND_DELAY);
    digitalWrite(hc12_set_pin, LOW);
    sleep_delay(AT_EXIT_DELAY);
    Serial.print(F(hc12_SET_DEFAULT));
    sleep_delay(AT_RESPONSE_DELAY);
    Serial.print(F(hc12_SET_CH));
    sleep_delay(AT_RESPONSE_DELAY);
    Serial.print(F(hc12_SET_FU));
    sleep_delay(AT_RESPONSE_DELAY);
    Serial.print(F(hc12_SET_PDB));
    sleep_delay(AT_RESPONSE_DELAY);
    EEPROM.write(hc12_setted_eeprom_address, 1);
#ifdef reflash
  }
#endif
  digitalWrite(hc12_set_pin, HIGH);
  sleep_delay(AT_EXIT_DELAY);
  Serial.begin(1200);
}
void hc12_sleep() {
  Serial.begin(1200);
  sleep_delay(AT_EXIT_DELAY);
  pinMode(hc12_set_pin, OUTPUT);
  digitalWrite(hc12_set_pin, LOW);
  delay(AT_EXIT_DELAY);
  Serial.print(F(hc12_SLEEP));
  delay(AT_RESPONSE_DELAY);
  digitalWrite(hc12_set_pin, HIGH);
  delay(AT_EXIT_DELAY);
}
void hc12_wakeup() {
  pinMode(hc12_set_pin, OUTPUT);
  digitalWrite(hc12_set_pin, LOW);
  delay(AT_EXIT_DELAY);
  digitalWrite(hc12_set_pin, HIGH);
  delay(AT_EXIT_DELAY);
}
