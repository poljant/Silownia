
/*
 * WebPagers.cpp
 *
 *  Created on: 6 lis 2019
 *      Author: jant
 */
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <pgmspace.h>
#include "silownia.h"
#include "templates.h"

extern float Ub, Ib, Iz, Io;
// login i hasło do sytemu (for http//IP/update)
// login & password to http//IP/update
const char* www_login = "admin";
const char* www_pass = "esp8266";
const char* ssid;
const char* pass;

const char* xpass;
const char* xssid;
const char* modes[] = { "NULL", "STA", "AP", "STA+AP" };
const char* phymodes[] = { "", "B", "G", "N" };
const char* encrypType[] = { "OPEN","WEP", "WPA","WPA", "WPA2", "WEP", "WPA_WPA2","OPEN", "WPA/WPA2/PSK" }; //??????

extern float Ub, Ib, Iz, Io;
const int port = 80;                 // port server www
ESP8266WebServer server(port);
ESP8266HTTPUpdateServer httpUpdate;
//const char cget = "Ub = %.1fV, Ib = %.1fA, Iz = %.1fA, Io = %.1fA";
//unsigned int ilM=10;
unsigned long fminutes(unsigned int ile) {
	return (millis() + (1000 * 60 * ile));
}

// funkcja zamieniająca adres IP na string
//convert IP to String
String IPtoStr(IPAddress IP) {
	String result;
	for (int i = 0; i < 4; ++i) {
		result += String(IP[i]);
		if (i < 3)
			result += '.';
	}
	return result;
}
//covert IP to char
char* IPtoChar(int ip)
{
  char* ip_str = new char[16];
  sprintf(ip_str, "%d.%d.%d.%d",
    (ip 	) & 0xFF,
    (ip >> 8) & 0xFF,
    (ip >> 16) & 0xFF,
    (ip >> 24) & 0xFF);
  return ip_str;
}
/*tring AddComplete(const char pattern, int addlen){
	char buff[sizeof(pattern)+addlen];
	snprintf_P(buff, sizeof(buff),
	        response, sizeof(response),
	return buff.c_str();
}*/
String HTMLHeader() {           //  nagłówek strony //Header page

	char* buff = new char[sizeof(HTML_HEADERS)+33];
	sprintf_P(buff, HTML_HEADERS, HOSTNAME);
/*	String h = F("<!DOCTYPE html>\n"
			"<html>"
			"<head>"
			"<title>");
	h += HOSTNAME;
	h += F(	"</title>"
			"<meta charset=\"utf-8\">"
			"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
			"<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css\" >"
			"</head>"
			"<body style=\"text-align: center;color: white; background: black;font-size: 1.5em;\">\n");*/
	//return h;
	return buff;
}

String HTMLFooter() {             //  stopka strony www  //foot page
	char* buff = new char[sizeof(HTML_FOOTER)+16];
	sprintf_P(buff, HTML_FOOTER, VERSION);
/*//	char buff
	String f = F("<p>Jan Trzciński &copy; 2021 VII Ver.");
	f += VERSION;
	f += ("</p></td></tr>"
			"</body>\n"
			"</html>\n");*/
	return buff;
}

String HTMLPage1() {      // pierwsza część strony www // primare page
	char buff[sizeof(HTML_PAGE1)+16];
	sprintf_P(buff, HTML_PAGE1, HOSTNAME, Ub ,Ib ,Iz , Io);
	return buff;
}

#ifdef WEBPAGEWIFISCAN
String HTMLWiFiScan(void){
	char buff[(sizeof(HTML_WIFISCAN_TAB)+256)];
	//char* ssid = new char[32];
	String ssid;
//	char hiden[3];
	String hiden="" ;
	String p="";
	String ix="";
//	uint8_t n = WiFi.scanNetworks();
	int n = WiFi.scanNetworks();
  delay(100);
#ifdef POLISH
	if (n == 0) return F("<p>Brak sieci WiFi.</p>");
	p +=F("<div><h3>Skanowanie sieci WiFi</h3></div>");
#else
  if (n == 0) return F("<p>No WiFi networks.</p>");
  p += F("<div><h3>WiFi network scanning.</h3></div>");
#endif
  p += HTML_WIFISCAN;
/*  p += F("<table  align=\"center\" border=\"2\" >"
	"<thead ><tr><th> </th><th style = \"text-align: center;\">SSID</th>");
#ifdef POLISH
	p += F("<th>kanał</th><th style = \"text-align: center;\">MAC</th>"
   "<th>RSSI</th><th>zabezp</th><th>ukryta</th><tr>");
#else
  p +=F("<th>channel</th><th style = \"text-align: center;\">MAC</th>"
      "<th style = \"text-align: center;\">RSSI</th><th>encryption</th><th>hidden</th><tr>");
#endif
	p +=F("</thead><tbody>");*/
	 for ( int i=0; i<n;  i++){	//uint8_t
/*	 p += F("<tr><td>"
	 "<form action=\"/wifiset\" metod=\"post\">"
	 "<labe><input id=\"SSID\" type=\"radio\" name=\"SSID\" value=\"");
	 p += WiFi.SSID(i);
	 p += F("\"></label></td>");
	 p +="<td>"+WiFi.SSID(i)+"</td>";	//nazwa sieci //SSID
	 p +="<td>"+String(WiFi.channel(i))+"</td>";	//kanał  //chanell
	 p +="<td>"+WiFi.BSSIDstr(i)+"</td>";	//MAC adress
	 p +="<td>"+String(WiFi.RSSI(i))+" dBm</td>"; //siła sygnału
	 p +="<td>";
//     p +=(String(WiFi.encryptionType(i)));// szyfrowanie
	 p +=(encrypType[WiFi.encryptionType(i)]);// szyfrowanie //encriptonType
	 p += F("</td><td>");*/
#ifdef POLISH
	 hiden =(WiFi.isHidden(i) ? "tak" : "nie");//czy sieć ukryta
#else
	 hiden =((WiFi.isHidden(i)) ? "yes" : "no");//WiFi hidden or not
#endif
/*	 ssid = (WiFi.SSID(i));
	 Serial.println(ssid);
	 Serial.println(WiFi.SSID(i));*/
	sprintf_P(buff, HTML_WIFISCAN_TAB, WiFi.SSID(i).c_str(), WiFi.SSID(i).c_str() ,(WiFi.channel(i)) ,WiFi.BSSIDstr(i).c_str() , (WiFi.RSSI(i)),(encrypType[WiFi.encryptionType(i)]),hiden);
	 p += String(buff);
	 } //end for

	 p += F("</tbody></table><div><p></p> </div>");
   return p;
}

String HTMLWiFiScan1(void){
  String p="";
  String ix="";

	 // gdy połączenie z WiFi //WiFi connect
	 if(WiFi.status() == WL_CONNECTED){
#ifdef POLISH
		p += F("<p>Jesteś połączony z siecią </p>");
#else
    p += F("<p>Connected with WiFi </p>");
#endif
		p += F("<table align=\"center\" border=\"2\" ><tbody>"
		"<thead ><tr><th style = \"text-align: center;\">SSID</th>");
#ifdef POLISH
		p += F("<th>kanał</th><th style = \"text-align: center;\">MAC</th>");
#else
    p += F("<th>channel</th><th style = \"text-align: center;\">MAC</th>");
#endif
		p += F("<th style = \"text-align: center;\">RSSI</th></thead><tbody>");
		p += "<td>"+WiFi.SSID()+"</td>";
		p += "<td>"+String(WiFi.channel())+"</td>";
		p += "<td>"+WiFi.BSSIDstr()+"</td>";
		p += "<td>"+String(WiFi.RSSI())+" dBm</td></tbody></table>";
		p +="<label>IP: ";
		p += IPtoStr(WiFi.localIP());
		p += F("</label>");
#ifdef POLISH
		p += F("<p>Aby zmienić sieć WiFi.</p>");
#else
    p += F("<p>To change the WiFi network.</p>");
#endif
	 }else{	//gdy brak połączenia z WiFi.
#ifdef POLISH
		 p += F("<p>Brak połączenia z siecią WiFi.</p>");
#else
     p += F("<p>No connection with WiFi.</p>");
#endif
		 p += F("<label>IP AP: ");
		 p += IPtoStr(WiFi.softAPIP());
		 p += F("</label>");
	 }	//end if
#ifdef POLISH
	 p += F("<p>Wybierz sieć powyżej i podaj hasło</p>"
	 "<label for=\"password\">Hasło do WiFi:</label>");
#else
   p += F("<p>Select the WiFi network above and enter the password</p>"
   "<label for=\"password\">WiFi password:</label>");
#endif
	 p += F("<input style=\"color: black;\" type=\"password\" name=\"password\" />") ; //required=\"required\"/>";
#ifdef POLISH
	 p += F("<input style=\"text-align: center;color: black;\" type=\"submit\" value=\"Połącz z WiFi.\"/></form>"
	  "<p><a href = \"/wifiscan\"><button class=\"btn btn-info\">Skanuj ponownie</button></a></p>"
      "<p><a href = \"/\"><button class=\"btn btn-info\">Strona główna</button></a></p>");
#else
     p += F("<input style=\"text-align: center;color: black;\" type=\"submit\" value=\"Connect.\"/></form>"
      "<p><a href = \"/wifiscan\"><button class=\"btn btn-info\">Scan again</button></a></p>"
	  "<p><a href = \"/\"><button class=\"btn btn-info\">Home</button></a></p>");
#endif
	return p;
}
String WebPageScan() {
	return (HTMLHeader() + HTMLWiFiScan() + HTMLWiFiScan1() + HTMLFooter());
}
#endif
String WebPage() {       // połącz wszystkie części strony www

	return (HTMLHeader() + HTMLPage1() + HTMLFooter());
}
// funkcja ustawia wszystkie strony www
void setservers() {
	httpUpdate.setup(&server, "/update", www_login, www_pass); // umożliwia aktualizację poprzez WiFi

	server.on("/", []() {      // odświerz stronę www

				server.send(200, "text/html", WebPage());
			});

	server.on("/get", []()      // pobierz dane
			{
//		char cget[]= "{ Ub = %.1fV, Ib = %.1fA, Iz = %.1fA, Io = %.1fA }";
		String sget = "{ Ub = ";
				sget += String(Ub);
				sget += "V, Ib = ";
				sget += String(Ib);
				sget += "A, Iz = ";
				sget += String(Iz);
				sget += ", Io = ";
				sget += String(Io);
				sget +="A}";
				server.send(200, "text/html", sget);
			});
	server.on("/reboot", [] ()
	{
		ESP.restart();
		delay(2000);
		server.send(200, "text/html", WebPage());
	});
//  /// WEBPAGEWIFISCAN
#ifdef WEBPAGEWIFISCAN
	server.on("/wifiscan", [] ()
	{
		server.send(200, "text/html", WebPageScan());
	});
//
//ustanawia połączenie z wybraną siecią wifi
	server.on("/wifiset", [] ()
	{
		//pobierz przysłane dane
			String ESSID = server.arg("SSID");
			String Epass = server.arg("password");

			if (((sizeof(ESSID)>=4) && (sizeof(Epass)>=8))) { //pomiń gdy brak nazwy lub hasła
				//konwertuj ze string do char
				//const char*
				ssid = ESSID.c_str();//e
				//const char*
				pass = Epass.c_str();//e

				WiFi.disconnect();//rozłacz obecne połączenie
				WiFi.mode(WIFI_AP_STA);//ustw tryb AP+STA

				WiFi.begin(ssid,pass);//uruchom łączenie z wybraną siecią wifi
				delay(200);

				int it = 20;//ustal maksymalny czas czekania na 10sek.(20x500ms)
				while ((WiFi.status() != WL_CONNECTED and it>0)) { //  czekaj na połączenie z WiFi

					delay(500);
					it--;
				}

		};
		server.send(200, "text/html", WebPageScan());
	});
#endif

/////end WEBPAGEWIFISCAN
	server.begin();                // Start serwera www
//	DEBUG(F("Server started"));
}




