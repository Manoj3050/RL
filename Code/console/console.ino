#include <SPI.h>
#include <Ethernet.h>
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

String webPage;
String result = "";


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip_default(192, 168, 1, 177);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  /*webPage += "<HEAD>";
    webPage += "<TITLE>Arduino GET test page</TITLE>";
    webPage += "</HEAD>";
    webPage += "<BODY>";
    webPage += "<H1>Setup Configuration</H1>";
    webPage += "<FORM ACTION='/' method=get >"; //uses IP/port of web page"
    webPage += "<table style=\"height: 262px;\" border=\"1\" width=\"568\">";
    webPage += "<tbody>";
    webPage += "<tr>";
    webPage += "<td style=\"width: 162.4px;\">RATES</td>";
    webPage += "<td style=\"width: 192.8px;\">Price</td>";
    webPage += "<td style=\"width: 192.8px;\">UptimeLimit</td>";
    webPage += "</tr>";
    webPage += "<tr>";
    webPage += "<td style=\"width: 162.4px;\">Per Min</td>";
    webPage += "<td style=\"width: 192.8px;\">1</td>";
    webPage += "<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"perM\" size=\"25\" type=\"TEXT\" value=\"\" /></td>";
    webPage += "</tr>";
    webPage += "<tr>";
    webPage += "<td style=\"width: 162.4px;\">Promo 1</td>";
    webPage += "<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"pc1\" size=\"25\" type=\"TEXT\" value=\"\" /></td>";
    webPage += "<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"pm1\" size=\"25\" type=\"TEXT\" value=\"\" /></td>";
    webPage += "</tr>";
    webPage += "<tr>";
    webPage += "<td style=\"width: 162.4px;\">Promo 2</td>";
    webPage += "<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"pc2\" size=\"25\" type=\"TEXT\" value=\"\" /></td>";
    webPage += "<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"pm2\" size=\"25\" type=\"TEXT\" value=\"\" /></td>";
    webPage += "</tr>";
    webPage += "<tr>";
    webPage += "<td style=\"width: 162.4px;\">Promo 3</td>";
    webPage += "<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"pc3\" size=\"25\" type=\"TEXT\" value=\"\" /></td>";
    webPage += "<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"pm3\" size=\"25\" type=\"TEXT\" value=\"\" /></td>";
    webPage += "</tr>";
    webPage += "<tr>";
    webPage += "<td style=\"width: 162.4px;\">Promo 4</td>";
    webPage += "<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"pc4\" size=\"25\" type=\"TEXT\" value=\"\" /></td>";
    webPage += "<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"pm4\" size=\"25\" type=\"TEXT\" value=\"\" /></td>";
    webPage += "</tr>";
    webPage += "<tr>";
    webPage += "<td style=\"width: 162.4px;\">IP</td>";
    webPage += "<td style=\"width: 192.8px;\">Username</td>";
    webPage += "<td style=\"width: 192.8px;\">Password</td>";
    webPage += "</tr>";
    webPage += "<tr>";
    webPage += "<td style=\"width: 162.4px;\"><input maxlength=\"3\" name=\"ip1\" size=\"1 type=\" type=\"text\" value=\"\" /><input maxlength=\"3\" name=\"ip2\" size=\"1 type=\" type=\"text\" value=\"\" /><input webpae += \"maxlength=\"3\" name=\"ip3\" size=\"1 type=\" type=\"text\" value=\"\" /><input maxlength=\"3\" name=\"ip4\" size=\"1 type=\" type=\"text\" value=\"\" /></td>\"\n";
    webPage += "<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"user\" size=\"25\" type=\"TEXT\" value=\"\" /></td>";
    webPage += "<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"pass\" size=\"25\" type=\"TEXT\" value=\"\" /></td>";
    webPage += "</tr>";
    webPage += "<tr>";
    webPage += "<td style=\"width: 162.4px;\">Printer</td>";
    webPage += "<td style=\"width: 192.8px;\">Value</td>";
    webPage += "</tr>";
    webPage += "<tr>";
    webPage += "<td style=\"width: 162.4px;\">BaudRate</td>";
    webPage += "<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"baud\" size=\"25\" type=\"TEXT\" value=\"\" /></td>";
    webPage += "</tr>";
    webPage += "</tbody>";
    webPage += "</table>";
    webPage += "<a href=\"submit\"><INPUT TYPE=SUBMIT NAME='submit' VALUE='Start'>";
    webPage += "</FORM>";
    webPage += "<BODY/>";*/
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  EEPROM_readAnything(0, readConfig);
  // if first time, else it should be 31
  if (readConfig.written != 31) {
    //set Default at first time
    Ethernet.begin(mac, ip_default);
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
  }
  else {
    //setReadValues
    IPAddress myip(readConfig.IP.i1, readConfig.IP.i2, readConfig.IP.i3, readConfig.IP.i4);
    Ethernet.begin(mac, ip_default);
  }

  // start the Ethernet connection and the server:

  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        // print it out the serial monitor
        if (c == '\r' || c == '\n') {
          client.flush();
          Serial.println("Client Done");
          Serial.println(currentLine);

          //Check for perMin
          if (currentLine.indexOf("perM") != -1) {
            int s_index = currentLine.indexOf("perM");
            int e_index = currentLine.indexOf("&", s_index);
            String temp = currentLine.substring(s_index + 5, e_index);
            Serial.println(temp);
            writeConfig.perMin = temp.toInt();
          }
          //Check for promo1 credit
          if (currentLine.indexOf("pc1") != -1) {
            int s_index = currentLine.indexOf("pc1");
            int e_index = currentLine.indexOf("&", s_index);
            String temp = currentLine.substring(s_index + 4, e_index);
            Serial.println(temp);
            writeConfig.promo1_credit = temp.toInt();
          }
          //check for promo mins
          if (currentLine.indexOf("pm1") != -1) {
            int s_index = currentLine.indexOf("pm1");
            int e_index = currentLine.indexOf("&", s_index);
            String temp = currentLine.substring(s_index + 4, e_index);
            Serial.println(temp);
            writeConfig.promo1_time = temp.toInt();
          }

          //Check for promo2 credit
          if (currentLine.indexOf("pc2") != -1) {
            int s_index = currentLine.indexOf("pc2");
            int e_index = currentLine.indexOf("&", s_index);
            String temp = currentLine.substring(s_index + 4, e_index);
            Serial.println(temp);
            writeConfig.promo2_credit = temp.toInt();
          }
          //check for promo2 mins
          if (currentLine.indexOf("pm2") != -1) {
            int s_index = currentLine.indexOf("pm2");
            int e_index = currentLine.indexOf("&", s_index);
            String temp = currentLine.substring(s_index + 4, e_index);
            Serial.println(temp);
            writeConfig.promo2_time = temp.toInt();
          }

          //Check for promo3 credit
          if (currentLine.indexOf("pc3") != -1) {
            int s_index = currentLine.indexOf("pc3");
            int e_index = currentLine.indexOf("&", s_index);
            String temp = currentLine.substring(s_index + 4, e_index);
            Serial.println(temp);
            writeConfig.promo3_credit = temp.toInt();
          }
          //check for promo3 mins
          if (currentLine.indexOf("pm3") != -1) {
            int s_index = currentLine.indexOf("pm3");
            int e_index = currentLine.indexOf("&", s_index);
            String temp = currentLine.substring(s_index + 4, e_index);
            Serial.println(temp);
            writeConfig.promo3_time = temp.toInt();
          }

          //Check for promo4 credit
          if (currentLine.indexOf("pc4") != -1) {
            int s_index = currentLine.indexOf("pc4");
            int e_index = currentLine.indexOf("&", s_index);
            String temp = currentLine.substring(s_index + 4, e_index);
            Serial.println(temp);
            writeConfig.promo4_credit = temp.toInt();
          }
          //check for promo4 mins
          if (currentLine.indexOf("pm4") != -1) {
            int s_index = currentLine.indexOf("pm4");
            int e_index = currentLine.indexOf("&", s_index);
            String temp = currentLine.substring(s_index + 4, e_index);
            Serial.println(temp);
            writeConfig.promo4_time = temp.toInt();
          }

          //Check for IP address
          if (currentLine.indexOf("ip1") != -1) {
            int s_index = currentLine.indexOf("ip1");
            int e_index = currentLine.indexOf("&", s_index);
            String temp = currentLine.substring(s_index + 3, e_index);
            writeConfig.IP.i1 = temp.toInt();
          }
          //Check for IP address
          if (currentLine.indexOf("ip2") != -1) {
            int s_index = currentLine.indexOf("ip2");
            int e_index = currentLine.indexOf("&", s_index);
            String temp = currentLine.substring(s_index + 3, e_index);
            writeConfig.IP.i2 = temp.toInt();
          }
          //Check for IP address
          if (currentLine.indexOf("ip3") != -1) {
            int s_index = currentLine.indexOf("ip3");
            int e_index = currentLine.indexOf("&", s_index);
            String temp = currentLine.substring(s_index + 3, e_index);
            writeConfig.IP.i3 = temp.toInt();
          }
          //Check for IP address
          if (currentLine.indexOf("ip4") != -1) {
            int s_index = currentLine.indexOf("ip4");
            int e_index = currentLine.indexOf("&", s_index);
            String temp = currentLine.substring(s_index + 3, e_index);
            writeConfig.IP.i4 = temp.toInt();
          }
          //check for promo mins
          if (currentLine.indexOf("baud") != -1) {
            int s_index = currentLine.indexOf("baud");
            int e_index = currentLine.indexOf("&", s_index);
            String temp = currentLine.substring(s_index + 5, e_index);
            Serial.println(temp);
            writeConfig.baud = temp.toInt();
          }
          //check for promo mins
          if (currentLine.indexOf("user") != -1) {
            int s_index = currentLine.indexOf("user");
            int e_index = currentLine.indexOf("&", s_index);
            String temp = currentLine.substring(s_index + 5, e_index);
            Serial.println(temp);
            writeConfig.username = temp;
          }
          //check for promo mins
          if (currentLine.indexOf("pass") != -1) {
            int s_index = currentLine.indexOf("pass");
            int e_index = currentLine.indexOf("&", s_index);
            String temp = currentLine.substring(s_index + 5, e_index);
            Serial.println(temp);
            writeConfig.password = temp;
          }
          if (writeConfig.username.equals(readConfig.username) && writeConfig.password.equals(readConfig.password)) {
            result = "Sucess";
            writeConfig.written = 31;
            EEPROM_writeAnything(0, writeConfig);
          }
          else {
            result = "Authentication Fail";
          }
          String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
          s += result;
          s += "\r";
          s += "</html>\n";

          if (currentLine.indexOf("submit") != -1) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println();
            client.print(result);
            client.println("<br />");
          }
          else {
            String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
            s += webPage;
            s += "\r";
            s += "</html>\n";
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println();
            client.println();
            client.println(F("<!DOCTYPE html> <html> <head> <title>Coin configuration</title>"));
            client.print("</head> <body> <header> <h1>Coin Configuration</h1> </header> ");
            client.print("<FORM ACTION='/' method=get >");
            client.print("<table style=\"height: 262px;\" border=\"1\" width=\"568\">");
            client.print("<tbody>");
            client.print("<tr>");
            client.print("<td style=\"width: 162.4px;\">RATES</td>");
            client.print("<td style=\"width: 192.8px;\">Price</td>");
            client.print("<td style=\"width: 192.8px;\">UptimeLimit</td>");
            client.print("</tr>");
            client.print("<tr>");
            client.print("<td style=\"width: 162.4px;\">Per Min</td>");
            client.print("<td style=\"width: 192.8px;\">1</td>");
            client.print("<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"perM\" size=\"25\" type=\"TEXT\" value=\"\" /></td>");
            client.print("</tr>");
            client.print("<tr>");
            client.print("<td style=\"width: 162.4px;\">Promo 1</td>");
            client.print("<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"pc1\" size=\"25\" type=\"TEXT\" value=\"\" /></td>");
            client.print("<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"pm1\" size=\"25\" type=\"TEXT\" value=\"\" /></td>");
            client.print("</tr>");
            client.print("<tr>");
            client.print("<td style=\"width: 162.4px;\">Promo 2</td>");
            client.print("<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"pc2\" size=\"25\" type=\"TEXT\" value=\"\" /></td>");
            client.print("<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"pm2\" size=\"25\" type=\"TEXT\" value=\"\" /></td>");
            client.print("</tr>");
            client.print("<tr>");
            client.print("<td style=\"width: 162.4px;\">Promo 3</td>");
            client.print("<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"pc3\" size=\"25\" type=\"TEXT\" value=\"\" /></td>");
            client.print("<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"pm3\" size=\"25\" type=\"TEXT\" value=\"\" /></td>");
            client.print("</tr>");
            client.print("<tr>");
            client.print("<td style=\"width: 162.4px;\">Promo 4</td>");
            client.print("<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"pc4\" size=\"25\" type=\"TEXT\" value=\"\" /></td>");
            client.print("<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"pm4\" size=\"25\" type=\"TEXT\" value=\"\" /></td>");
            client.print("</tr>");
            client.print("<tr>");
            client.print("<td style=\"width: 162.4px;\">IP</td>");
            client.print("<td style=\"width: 192.8px;\">Username</td>");
            client.print("<td style=\"width: 192.8px;\">Password</td>");
            client.print("</tr>");
            client.print("<tr>");
            client.print("<td style=\"width: 162.4px;\"><input maxlength=\"3\" name=\"ip1\" size=\"1 type=\" type=\"text\" value=\"\" /><input maxlength=\"3\" name=\"ip2\" size=\"1 type=\" type=\"text\" value=\"\" /><input webpae += \"maxlength=\"3\" name=\"ip3\" size=\"1 type=\" type=\"text\" value=\"\" /><input maxlength=\"3\" name=\"ip4\" size=\"1 type=\" type=\"text\" value=\"\" /></td>\"\n");
            client.print("<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"user\" size=\"25\" type=\"TEXT\" value=\"\" /></td>");
            client.print("<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"pass\" size=\"25\" type=\"TEXT\" value=\"\" /></td>");
            client.print("</tr>");
            client.print("<tr>");
            client.print("<td style=\"width: 162.4px;\">Printer</td>");
            client.print("<td style=\"width: 192.8px;\">Value</td>");
            client.print("</tr>");
            client.print("<tr>");
            client.print("<td style=\"width: 162.4px;\">BaudRate</td>");
            client.print("<td style=\"width: 192.8px;\"><input maxlength=\"50\" name=\"baud\" size=\"25\" type=\"TEXT\" value=\"\" /></td>");
            client.print("</tr>");
            client.print("</tbody>");
            client.print("</table>");
            client.print("<br />");
            client.print("<a href=\"submit\"><INPUT TYPE=SUBMIT NAME='submit' VALUE='Start'>");
            client.print("</form>");
            client.print("</body>");
            client.print("</html>");
          }
          client.stop();

        }
        else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

      }

    }
  }
  // close the connection:
}
