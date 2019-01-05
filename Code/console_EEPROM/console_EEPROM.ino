#include <EEPROM.h>

typedef unsigned char byte;

template <class T> int EEPROM_writeAnything(int ee, const T& value)
{
    const byte* p = (const byte*)(const void*)&value;
    int i;
    for (i = 0; i < sizeof(value); i++)
    EEPROM.write(ee++, *p++);
    return i;
}

template <class T> int EEPROM_readAnything(int ee, T& value)
{
    byte* p = (byte*)(void*)&value;
    int i;
    for (i = 0; i < sizeof(value); i++)
    *p++ = EEPROM.read(ee++);
    return i;
}

typedef struct {
  int i1;
  int i2;
  int i3;
  int i4;
} configIPaddress;

typedef struct config_t
{
    int written;
    configIPaddress IP;
    int perMin;
    int promo1_time;
    int promo1_value;
    int promo2_time;
    int promo2_value;
    int promo3_time;
    int promo3_value;
    int promo4_time;
    int promo4_value;
    String username;
    String password;
} CONFIG;

CONFIG writeConfig;
CONFIG readConfig;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  writeConfig.IP.i1 = 192;
  writeConfig.IP.i2 = 168;
  writeConfig.IP.i3 = 8;
  writeConfig.IP.i4 = 1;
  writeConfig.promo1_time = 5;
  writeConfig.promo1_value = 60;
  writeConfig.promo2_time = 10;
  writeConfig.promo2_value = 180;
  writeConfig.promo3_time = 30;
  writeConfig.promo3_value = 300;
  writeConfig.promo4_time = 50;
  writeConfig.promo4_value = 1440;
  writeConfig.username = "admin";
  writeConfig.password = "password";
  EEPROM_writeAnything(0,writeConfig);
  EEPROM_readAnything(0,readConfig);

  Serial.println(readConfig.IP.i1,DEC);
  Serial.println(readConfig.IP.i2,DEC);
  Serial.println(readConfig.IP.i3,DEC);
  Serial.println(readConfig.IP.i4,DEC);
  Serial.println(readConfig.promo1_time,DEC);
  Serial.println(readConfig.promo1_value,DEC);
  Serial.println(readConfig.promo2_time,DEC);
  Serial.println(readConfig.promo2_value,DEC);
  Serial.println(readConfig.promo3_time,DEC);
  Serial.println(readConfig.promo3_value,DEC);
  Serial.println(readConfig.promo4_time,DEC);
  Serial.println(readConfig.promo4_value,DEC);
  Serial.println(readConfig.username);
  Serial.println(readConfig.password);

}

void loop() {
  // put your main code here, to run repeatedly:

}
