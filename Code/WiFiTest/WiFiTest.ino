

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>

MDNSResponder mdns;

// Replace with your network credentials
const char* ssid = "SLT_FIBRE";
const char* password = "0112911845";

WiFiServer server(80);

String webPage = "";
String result = "";

int gpio5_pin = 5;
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

void setup(void) {
  //webPage += "HTTP/1.1 200 OK";
  //webPage += "Content-Type: text/html";
  //webPage += "";
  //webPage += "<HTML>";
  webPage += "<HEAD>";
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
  webPage += "<BODY/>";
  //webPage += "</HTML>";

  // preparing GPIOs
  pinMode(gpio5_pin, OUTPUT);
  digitalWrite(gpio5_pin, LOW);
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  /*server.on("/", []() {
    server.send(200, "text/html", webPage);
    });*/

  /*server.on("/submit", []() {
    server.send(200, "text/html", webPage);
    String readString = "";
    while (Serial.available()) {
      char c = Serial.read(); //gets one byte from serial buffer
      readString += c; //makes the String readString
      delay(2); //slow looping to allow buffer to fill with next character
    }
    if (readString.length() > 0) {
      Serial.println(readString); //prints string to serial port out
      int pos1 = readString.indexOf('=');
      int pos2 = readString.indexOf('&');
      String newString;
      newString = readString.substring(pos1 + 1, pos2);
      Serial.print("newString is: ");
      Serial.println(newString);
      int val;
      val = newString.toInt();
      delay(val * 1000);
      digitalWrite(gpio5_pin, HIGH);
      //delay(1000);
      Serial.print("The value sent is: ");
      Serial.println(val);
      readString = ""; //clears variable for new input
      newString = ""; //clears variable for new input
    }
    });*/

  server.begin();
  Serial.println("HTTP server started");
  server.print(webPage);

  readConfig.username = "admin";
  readConfig.password = "admin";
}

void loop(void) {
  WiFiClient client = server.available();

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
          if(writeConfig.username.equals(readConfig.username) && writeConfig.password.equals(readConfig.password)){
            result = "Sucess";
          }
          else{
            result = "Authentication Fail";
          }
          String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
          s += result;
          s += "\r";
          s += "</html>\n";

          if (currentLine.indexOf("submit") != -1) {
            client.print(s);
          }
          else {
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
}
