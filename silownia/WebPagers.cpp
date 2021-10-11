
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
String HTMLHeader() {           //  nagłówek strony //Header page

	char* buff = new char[sizeof(HTML_HEADERS)+33];
	sprintf_P(buff, HTML_HEADERS, HOSTNAME);
	return buff;
}

String HTMLFooter() {             //  stopka strony www  //foot page
	char* buff = new char[sizeof(HTML_FOOTER)+16];
	sprintf_P(buff, HTML_FOOTER, VERSION);
	return buff;
}

String HTMLPage1() {      // pierwsza część strony www // primare page
	char buff[sizeof(HTML_PAGE1)+16];
	sprintf_P(buff, HTML_PAGE1, HOSTNAME, Ub ,Ib ,Iz , Io);
	return buff;
}

#ifdef WEBPAGEWIFISCAN
String HTMLWiFiScan(void){
	char buff[(sizeof(HTML_WIFISCAN_TAB)+64)];
	String hiden="  " ;
	String p="";
//	Serial.println("Start skanowania");

	int n = WiFi.scanNetworks();
  delay(100);
//    Serial.println(n);
//	Serial.println("koniec skanowania");

#ifdef POLISH
	if (n == 0) return F("<p>Brak sieci WiFi.</p>");
	p += F("<div><h3>Skanowanie sieci WiFi</h3></div>");
#else
  if (n == 0) return F("<p>No WiFi networks.</p>");
  p += F("<div><h3>WiFi network scanning.</h3></div>");
#endif
  p += String(HTML_WIFISCAN);
//  Serial.println(p);
//	return "";
	 for ( int i=0; i<n;  i++){	//uint8_t

#ifdef POLISH
	 hiden =(WiFi.isHidden(i) ? "tak" : "nie");//czy sieć ukryta
#else
	 hiden =((WiFi.isHidden(i)) ? "yes" : "no");//WiFi hidden or not
#endif
	sprintf_P(buff, HTML_WIFISCAN_TAB, WiFi.SSID(i).c_str(), WiFi.SSID(i).c_str() ,WiFi.channel(i) ,WiFi.BSSIDstr(i).c_str() , WiFi.RSSI(i),encrypType[WiFi.encryptionType(i)],hiden.c_str());
	 p += String(buff);
	 } //end for

	 p += String(HTML_WIFISCAN_TAB_END);
//	 Serial.println(p);
   return p;
}

String HTMLWiFiScan1(void){
  String p="";
//  String ix="";
	char buff[sizeof(HTML_WIFISCAN1B)+64];

	 // gdy połączenie z WiFi //WiFi connect
	 if(WiFi.status() == WL_CONNECTED){

		sprintf_P(buff, HTML_WIFISCAN1, WiFi.SSID().c_str(), WiFi.channel() ,WiFi.BSSIDstr().c_str() ,WiFi.RSSI() , IPtoStr(WiFi.localIP()).c_str());
		p += String(buff);

	 }else{	//gdy brak połączenia z WiFi.
		 sprintf_P(buff, HTML_WIFISCAN1A, IPtoStr(WiFi.softAPIP()).c_str());
			p += String(buff);

	 }
	 p += String(HTML_WIFISCAN1B);
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
/*		char cget[48];
		sprintf_P(cget,PSTR("{Ub = %.1fV, Ib = %.1fA, Iz = %.1fA, Io = %.1fA}"), Ub,Ib,Iz,Io);*/
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




