#include <SoftwareSerial.h>
#define hm10_conn_pin                   4
#define hm10_key_pin                    6
#define hm10_rx_pin                     8
#define hm10_tx_pin                     9

#define hm10_central "AT+ROLE0"
#define hm10_perepherial "AT+ROLE0"
#define hm10_name_cmd "AT+NAME"
#define hm10_reset "AT+RESET"
#define hm10_renew "AT+RENEW"

#define hm10_sleep_cmd "AT+SLEEP"

#define hm10_no_auto_sleep_cmd "AT+PWRM1"
#define hm10_auto_sleep_cmd "AT+PWRM0"




#define hm10_name_prefix String("GRUZ_")


SoftwareSerial hm10(hm10_tx_pin, hm10_rx_pin);

void hm10_wakeup() {
  pinMode(hm10_key_pin, OUTPUT);
  hm10.begin(9600);
  digitalWrite(hm10_key_pin, LOW);
  delay(1000);
  digitalWrite(hm10_key_pin, HIGH);
  sleep_delay(mSLEEP_250MS);  
  hm10.println(hm10_no_auto_sleep_cmd);
  sleep_delay(mSLEEP_250MS); 
  hm10.println(hm10_perepherial);
  sleep_delay(mSLEEP_250MS);
  hm10.println(hm10_reset);
  sleep_delay(mSLEEP_500MS);
}
