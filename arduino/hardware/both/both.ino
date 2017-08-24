  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//#define DEBUG true  /// Закомментировать для боевой прошивки 
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// ID устройства - меняется для каждой пары приемник-передатчик
String device_ID = "040059";

/// Для прошивки датчика - поставьте // в начале строки
/// Для прошивки приемника - уберите // в начале строки/

 #define reciever true
 
 
 /// Если после включения первым моргает красный светодиод - уберите // в начале строки
#define wrong_leds true



#include <avr/power.h>
#include <avr/sleep.h>
#include <EEPROM.h>
#include "delays.h"
#include "settings.h"
#include "hardware.h"
#include "hc12.h"

#ifdef reciever
#include "hm10.h"
#endif

#include "eeprom.h"
#include "leds.h"
#include "sounds.h"
#include "functions.h"
#include "states.h"



boolean power_plugged = false;




//Раскоментируйте  следующую строку, если нужно перепроишть hc-12, неуспешно прошитую ранее микроконтроллером (влияет на hc12_init)
#define reflash true

//  ID прошиваемого устройства




#warning  Проверьте ID прошиваемого устройства, убедитесь что поставили актульный номер
#ifdef reciever
#include "reciever.h"
#warning !!!!!!!!   Внимание!!! Это прошивка для ___Приемника.  Закомментируйте строку выше  ____#define reciever_____ true для прошивки Датчика
#else
#include <avr/wdt.h>
#include "transmitter.h"
#warning !!!!!!!!   Внимание!!! Это прошивка для ___Датчика.    Раскомемнтируйте строку выше  ____#define reciever_____ true для прошивки Приемника
#endif
