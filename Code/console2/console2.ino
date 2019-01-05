#include <SPI.h>
#include <Ethernet.h>
#include "HTTPserver.h"
#include <EEPROM.h>
// Enter a MAC address and IP address for your controller below.
byte mac[] = {  0x90, 0xA2, 0xDA, 0x00, 0x2D, 0xA1 };

// The IP address will be dependent on your local network:
byte ip_default[] = { 192, 168, 50, 200 };

// the router's gateway address:
byte gateway[] = { 192, 168 , 8, 1 };

// the subnet mask
byte subnet[] = { 255, 255, 255, 0 };


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
  int promo1_credit;
  int promo1_time; // time is in mins
  int promo2_credit;
  int promo2_time;
  int promo3_credit;
  int promo3_time;
  int promo4_credit;
  int promo4_time;
  int baud;
  String username;
  String password;
} CONFIG;

CONFIG writeConfig;
CONFIG readConfig;

String result = "";

bool configDone = false;
// Initialize the Ethernet server library
EthernetServer server(80);

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

// derive an instance of the HTTPserver class with custom handlers
class myServerClass : public HTTPserver
{
    virtual void processPostType        (const char * key, const byte flags);
    virtual void processPathname        (const char * key, const byte flags);
    virtual void processHttpVersion     (const char * key, const byte flags);
    virtual void processGetArgument     (const char * key, const char * value, const byte flags);
    virtual void processHeaderArgument  (const char * key, const char * value, const byte flags);
    virtual void processCookie          (const char * key, const char * value, const byte flags);
    virtual void processPostArgument    (const char * key, const char * value, const byte flags);
};  // end of myServerClass

myServerClass myServer;

// -----------------------------------------------
//  User handlers
// -----------------------------------------------


void myServerClass::processPostType (const char * key, const byte flags)
{

} // end of processPostType

void myServerClass::processPathname (const char * key, const byte flags)
{

}  // end of processPathname

void myServerClass::processHttpVersion (const char * key, const byte flags)
{

}  // end of processHttpVersion

void myServerClass::processGetArgument (const char * key, const char * value, const byte flags)
{
  Serial.println (F("Get argument: "));
  if (strcmp(key, "perM") == 0) {
    writeConfig.perMin = atoi(value);
    Serial.println(writeConfig.perMin);
  } else if (strcmp(key, "pc1") == 0) {
    writeConfig.promo1_credit = atoi(value);
    Serial.println(writeConfig.promo1_credit);
  } else if (strcmp(key, "pm1") == 0) {
    writeConfig.promo1_time = atoi(value);
    Serial.println(writeConfig.promo1_time);
  } else if (strcmp(key, "pc2") == 0) {
    writeConfig.promo2_credit = atoi(value);
    Serial.println(writeConfig.promo2_credit);
  } else if (strcmp(key, "pm2") == 0) {
    writeConfig.promo2_time = atoi(value);
    Serial.println(writeConfig.promo2_time);
  } else if (strcmp(key, "pc3") == 0) {
    writeConfig.promo3_credit = atoi(value);
    Serial.println(writeConfig.promo3_credit);
  } else if (strcmp(key, "pm3") == 0) {
    writeConfig.promo3_time = atoi(value);
    Serial.println(writeConfig.promo3_time);
  } else if (strcmp(key, "pc4") == 0) {
    writeConfig.promo4_credit = atoi(value);
    Serial.println(writeConfig.promo4_credit);
  } else if (strcmp(key, "pm4") == 0) {
    writeConfig.promo4_time = atoi(value);
    Serial.println(writeConfig.promo4_time);
  } else if (strcmp(key, "ip1") == 0) {
    writeConfig.IP.i1 = atoi(value);
    Serial.println(writeConfig.IP.i1);
  } else if (strcmp(key, "ip2") == 0) {
    writeConfig.IP.i2 = atoi(value);
    Serial.println(writeConfig.IP.i2);
  } else if (strcmp(key, "ip3") == 0) {
    writeConfig.IP.i3 = atoi(value);
    Serial.println(writeConfig.IP.i3);
  } else if (strcmp(key, "ip4") == 0) {
    writeConfig.IP.i4 = atoi(value);
    Serial.println(writeConfig.IP.i4);
  } else if (strcmp(key, "baud") == 0) {
    writeConfig.baud = atoi(value);
    Serial.println(writeConfig.baud);
  } else if (strcmp(key, "user") == 0) {
    writeConfig.username = value;
    Serial.println(writeConfig.username);
  } else if (strcmp(key, "pass") == 0) {
    writeConfig.password = value;
    Serial.println(writeConfig.password);
  }
  if (writeConfig.username.equals(readConfig.username) && writeConfig.password.equals(readConfig.password)) {
    result = "Sucess";
    writeConfig.written = 31;
    EEPROM_writeAnything(0, writeConfig);
    configDone = true;
  }
  else {
    result = "Authentication Fail";
    configDone = false;
  }

}  // end of processGetArgument

void myServerClass::processHeaderArgument (const char * key, const char * value, const byte flags)
{

}  // end of processHeaderArgument

void myServerClass::processCookie (const char * key, const char * value, const byte flags)
{

}  // end of processCookie

void myServerClass::processPostArgument (const char * key, const char * value, const byte flags)
{

}  // end of processPostArgument

// -----------------------------------------------
//  End of user handlers
// -----------------------------------------------

void setup ()
{
  // start the Ethernet connection and the server:
  
  Serial.begin(115200);
  Serial.println("Starting server.");
  EEPROM_readAnything(0, readConfig);
  if (readConfig.written != 31) {
    //set Default at first time
    Ethernet.begin(mac, ip_default, gateway, subnet);
    readConfig.perMin = 5;
    readConfig.promo1_credit = 5;
    readConfig.promo1_time = 60;
    readConfig.promo2_credit = 10;
    readConfig.promo2_time = 120;
    readConfig.promo3_credit = 20;
    readConfig.promo3_time = 240;
    readConfig.promo4_credit = 50;
    readConfig.promo4_time = 1440;
    readConfig.baud = 9600;
    readConfig.username = "admin";
    readConfig.password = "admin";
  }
  else {
    //setReadValues
    IPAddress myip(readConfig.IP.i1, readConfig.IP.i2, readConfig.IP.i3, readConfig.IP.i4);
    Ethernet.begin(mac, ip_default, gateway, subnet);
  }
  
  server.begin();
}  // end of setup

void loop ()
{
  // listen for incoming clients
  EthernetClient client = server.available();
  if (!client)
    return;

  myServer.begin (&client);
  while (client.connected() && !myServer.done)
  {
    while (client.available () > 0 && !myServer.done)
      myServer.processIncomingByte (client.read ());

    // do other stuff here

  }  // end of while client connected
  
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.println();
  client.println(F("<!DOCTYPE html> <html> <head> <title>Coin configuration</title>"));
  client.print("</head> <body> <header> <h1>Coin Configuration</h1> </header> ");
  if(configDone == true){
    client.print("Configuration Successful");
    client.println("<br>");
  }
  client.print("<FORM ACTION='/' method=get >");
  client.print("<table style=\"height: 262px;\" border=\"1\" width=\"600\">");
  client.print("<tbody>");
  client.print("<tr>");
  client.print("<td style=\"width: 200px;\">RATES</td>");
  client.print("<td style=\"width: 192.8px;\">Price</td>");
  client.print("<td style=\"width: 192.8px;\">UptimeLimit</td>");
  client.print("</tr>");
  client.print("<tr>");
  client.print("<td style=\"width: 200px;\">Per Min</td>");
  client.print("<td style=\"width: 192.8px;\">1</td>");
  client.print("<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"perM\" size=\"25\" type=\"TEXT\" value=\"\" /></td>");
  client.print("</tr>");
  client.print("<tr>");
  client.print("<td style=\"width: 200px;\">Promo 1</td>");
  client.print("<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"pc1\" size=\"25\" type=\"TEXT\" value=\"\" /></td>");
  client.print("<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"pm1\" size=\"25\" type=\"TEXT\" value=\"\" /></td>");
  client.print("</tr>");
  client.print("<tr>");
  client.print("<td style=\"width: 200px;\">Promo 2</td>");
  client.print("<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"pc2\" size=\"25\" type=\"TEXT\" value=\"\" /></td>");
  client.print("<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"pm2\" size=\"25\" type=\"TEXT\" value=\"\" /></td>");
  client.print("</tr>");
  client.print("<tr>");
  client.print("<td style=\"width: 200px;\">Promo 3</td>");
  client.print("<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"pc3\" size=\"25\" type=\"TEXT\" value=\"\" /></td>");
  client.print("<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"pm3\" size=\"25\" type=\"TEXT\" value=\"\" /></td>");
  client.print("</tr>");
  client.print("<tr>");
  client.print("<td style=\"width: 200px;\">Promo 4</td>");
  client.print("<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"pc4\" size=\"25\" type=\"TEXT\" value=\"\" /></td>");
  client.print("<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"pm4\" size=\"25\" type=\"TEXT\" value=\"\" /></td>");
  client.print("</tr>");
  client.print("<tr>");
  client.print("<td style=\"width: 200px;\">IP</td>");
  client.print("<td style=\"width: 192.8px;\">Username</td>");
  client.print("<td style=\"width: 192.8px;\">Password</td>");
  client.print("</tr>");
  client.print("<tr>");
  client.print("<td style=\"width: 165px;\"><input maxlength=\"3\" name=\"ip1\" size=\"1 type=\" type=\"text\" value=\"\" /><input maxlength=\"3\" name=\"ip2\" size=\"1 type=\" type=\"text\" value=\"\" /><input webpae += \"maxlength=\"3\" name=\"ip3\" size=\"1 type=\" type=\"text\" value=\"\" /><input maxlength=\"3\" name=\"ip4\" size=\"1 type=\" type=\"text\" value=\"\" /></td>\"\n");
  client.print("<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"user\" size=\"25\" type=\"TEXT\" value=\"\" /></td>");
  client.print("<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"pass\" size=\"25\" type=\"TEXT\" value=\"\" /></td>");
  client.print("</tr>");
  client.print("<tr>");
  client.print("<td style=\"width: 200px;\">Printer</td>");
  client.print("<td style=\"width: 192.8px;\">Value</td>");
  client.print("</tr>");
  client.print("<tr>");
  client.print("<td style=\"width: 200px;\">BaudRate</td>");
  client.print("<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"baud\" size=\"25\" type=\"TEXT\" value=\"\" /></td>");
  client.print("</tr>");
  client.print("</tbody>");
  client.print("</table>");
  client.print("<br />");
  client.print("<a href=\"submit\"><INPUT TYPE=SUBMIT NAME='submit' VALUE='Start'>");
  client.print("</form>");
  client.print("</body>");
  client.println("</html>");
  // give the web browser time to receive the databody
  delay(1);
  // close the connection:
  client.stop();

}  // end of loop

