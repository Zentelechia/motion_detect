



#define hc12_setted_eeprom_address 0
#define transmitter_ID_eeprom_address 2

#define hm10_name_prefix String("GRUZ_")

#define blink_duration      mSLEEP_500MS
#define blink_long_duration mSLEEP_2S
#define blink_short_duration mSLEEP_120MS

#define Do 261
#define Re 294
#define Mi 330
#define Fa 349
#define Sol 392
#define La 440
#define Si 494

#define looking_for_transmitter_cmd     'R'
#define polling_cmd     'P'
#define pir_cmd         'A'
#define id_cmd          'I'
#define no_polling_cmd  'N'
#define no_polling_stopped_cmd  'S'

#define battery_k 1 //0.0064453  // 3.3/1024*2

#define  battery_high_voltage   627 // 4.1
#define  battery_medium_voltage  566 // 3.7
#define  battery_low_voltage   535 //3.5

unsigned long polling_timeout = (unsigned long) (5UL * 60UL * 1000UL); //UL - окончания для unsigned long, иначе проблема

