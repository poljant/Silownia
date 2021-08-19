#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <functional>
#include "silownia.h"
#include "secrets.h"
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>
#include "WebPagers.h"
//#include <WiFiManager.h>
#include <pgmspace.h>
#include <Wire.h>

#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_PCF8574.h>
#include <stdio.h>

#include <Adafruit_ADS1X15.h>
Adafruit_ADS1015 ads1015;  	// Construct an ads1015
//Adafruit_ADS1115 ads1115;	// Construct an ads1115


/*
*void setup(void)
{
  ads1015.begin();  // Initialize ads1015 at the default address 0x48
  ads1115.begin(0x49);  // Initialize ads1115 at address 0x49
}
*/

extern ESP8266WebServer server;
String hostname();
extern unsigned long fminutes(unsigned int);
unsigned long aptime;
unsigned int apminutes = 10;
//dane dla AP
const char* ap_ssid = hostname().c_str();   // SSID AP
const char* ap_pass = "12345678";  // password do AP
int ap_channel = 7; //numer kanału dla AP

String version = VERSION;
LiquidCrystal_PCF8574 lcd(0x27);


#define ONEWIRE D3
#define PowerMax D4

#define Alert D0
#define ERROR_U1 D5
#define ERROR_U2 D6
#define ANALOG A0

int mVperAmp = 66; // use 185 for 5A 100 for 20A Module and 66 for 30A Module
int ACSoffset = 835; //2500;
//	  uint16_t
int adc0, adc1, adc2, adc3;
//float volts0, volts1, volts2, volts3;
//float amps0, amps1, amps2, amps3;
/*Wybór adresu magistrali I2C
Aby zmienić 7-bitowy adres z domyślnego 0x48 (1001000) należy połączyć pin ADR z:

0x48 (1001000) ADR -> GND
0x49 (1001001) ADR -> VDD
0x4A (1001010) ADR -> SDA
0x4B (1001011) ADR -> SCL*/
#define Adr D8 // set address ADS1015 if value=0 address is 0x48 if value=1 address is 0x49

#ifdef IP_STATIC
IPAddress IPadr(10,110,3,33);
IPAddress netmask(255,255,0,0);
IPAddress gateway(10,110,0,1);
//////////////////////////////
#endif
double long timed = 1500; //time to delay display
double long timec = 0; //time current to display
#ifdef WEBPAGEWIFISCAN
void WiFiconnect(void) {

	WiFi.mode(WIFI_AP_STA);
	//WiFi.begin(ap_ssid, ap_pass, ap_channel);

#ifdef IP_STATIC
	WiFi.config(IPadr, gateway, netmask);
	WiFi.begin(myssid, mypass);
#else
		WiFi.begin();
#endif

	int i = 0;
	while ((WiFi.status() != WL_CONNECTED) && (i <= 20)) {
		delay(500);
		i += 1;
		Serial.print(".");
	}
	if (WiFi.status() == WL_CONNECTED) {
		Serial.println("");
		Serial.print("WiFi connected. IP ");
		Serial.println(WiFi.localIP());          // IP server
//	Serial.println(WiFi.macAddress());      // MAC address
	} else {
		WiFi.mode(WIFI_AP_STA);
		WiFi.begin(ap_ssid, ap_pass); // ap_channel);
		Serial.print("WiFi not connect : ");
		Serial.println(WiFi.status());
	}

}
#endif
//format bytes
String formatBytes(size_t bytes) {
	if (bytes < 1024) {
		return String(bytes) + "B";
	} else if (bytes < (1024 * 1024)) {
		return String(bytes / 1024.0) + "KB";
	} else if (bytes < (1024 * 1024 * 1024)) {
		return String(bytes / 1024.0 / 1024.0) + "MB";
	} else {
		return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
	};
};
float Ub = 12;
float Ib = -5;
float Iz = 10;
float Io = Iz-Ib;
int A0Read = 0;
//napięcia dla akumulatora ołowiowego
float Umin = 10.5;  // minimalne napięcie rozładowania
float Ubuf = 13.5;  // napięcie pracy buforowej
float Umax = 14.5;  // maksymalne napięcie łądowania
///////////////////////////////////////////
String hostname(void) {
	uint32_t chipId = ESP.getChipId();
	char uid[20];
	sprintf_P(uid, PSTR("%s_%02x%02x%02x"), HOSTNAME,
	        (uint16_t) ((chipId >> 16) & 0xff),
			(uint16_t) ((chipId >> 8) & 0xff), ((uint16_t) chipId & 0xff));
//	Serial.println(uid);
	return String(uid);
}
void setup(){
	Wire.begin(5, 4); //SDA, ACL
	Serial.begin(115200);

    //ustaw bity kontrolujące zwarcie na wyjściach
	pinMode(ERROR_U1, INPUT_PULLUP);
	pinMode(ERROR_U2, INPUT_PULLUP);
//	  ads1015.begin();  // Initialize ads1015 at the default address 0x48
		lcd.begin(16, 2);
		lcd.home();
		lcd.clear();
		lcd.setBacklight(255);
		lcd.setCursor(0, 0);
		lcd.print(F("   Silownia!   "));
		lcd.setCursor(0, 1);
		lcd.print(F("Uruchamianie..."));

#ifdef WEBPAGEWIFISCAN
	WiFiconnect();
#else
#include <WiFiManager.h>
  WiFiManager wifiManager;
  //wifiManager.resetSettings();
#ifdef IP_STATIC
  wifiManager.setSTAStaticIPConfig(IPadr, netmask, gateway);
#endif
  wifiManager.autoConnect(hostname().c_str());
  delay(100);
#endif
	MDNS.begin(hostname(), WiFi.localIP());
//	DEBUG_OUTLN(hostname());
//	DEBUG_OUTLN(WiFi.localIP());
	// ustaw adres 0x48 na ADS1015
    pinMode(Adr, OUTPUT);
    digitalWrite(Adr, 0);
	ads1015.begin();  // Initialize ads1015 at the default address 0x48
//led.begin(LED_PIN, true);
	setservers();
	aptime = fminutes(apminutes);
}

void loop(){
//	int ix = 0;
	char s1[10];
	char s2[10];
	char s3[10];
	char s4[10];
/*	String s1;
	String s2;*/
	server.handleClient();
	if (WiFi.status() != WL_CONNECTED) {
		WiFi.mode(WIFI_AP_STA); //tryb AP+STATION
//		led.setOn(); // włącz LED gdy nie jest połączenie z WiFi
		aptime = fminutes(apminutes);  // licz czas trwania AP po połaczeniu
	} else {
		if (aptime <= millis()){  // gdy minął czas trwania AP przełącz na STATION
		WiFi.mode(WIFI_STA); //tryb STATION
		}
//		led.setOff(); // wyłącz LED gdy jest połączenie z WiFi
	}
	MDNS.update();
	if (timec <= millis()) {
		timec = millis() + timed;
	A0Read = analogRead(ANALOG);
	Ub = (16.4 *  A0Read /1024);

	  adc0 = ads1015.readADC_SingleEnded(0);
//	  adc1 = ads1015.readADC_SingleEnded(1);
	  adc2 = ads1015.readADC_SingleEnded(2);
//	  adc3 = ads1015.readADC_SingleEnded(3);

/*	  volts0 = ads1015.computeVolts(adc0);
	  volts1 = ads1015.computeVolts(adc1);
	  volts2 = ads1015.computeVolts(adc2);
	  volts3 = ads1015.computeVolts(adc3);

	  amps0 = ((volts0 - ACSoffset) / mVperAmp);
	  amps1 = ((volts1 - ACSoffset) / mVperAmp);
	  amps2 = ((volts2 - ACSoffset) / mVperAmp);
	  amps3 = ((volts3 - ACSoffset) / mVperAmp);*/

	  Iz = -(adc0-ACSoffset)/19.214; //amps0;
	  Ib = (adc2-ACSoffset)/19.214; //amps2;
//	  Ub = adc1;
	  Io = Iz-Ib;
	  //Io = ib/10;

//	snprintf_P(s, sizeof(s), PSTR("%s is %i years old"), name, age);
	snprintf(s1,sizeof(s1),"Ub%.1fV",Ub);
	snprintf(s2,sizeof(s2),"Ib%.1fA",Ib);
	snprintf(s3,sizeof(s3),"Iz%.1fA",Iz);
	snprintf(s4,sizeof(s4),"Io%.1fA",Io);

	lcd.home();
	lcd.clear();
	lcd.setBacklight(255);
	lcd.setCursor(0, 0);
	lcd.print(s1);
	lcd.setCursor(8, 0);
	lcd.print(s2);
	lcd.setCursor(0, 1);
	lcd.print(s3);
	lcd.setCursor(8, 1);
	lcd.print(s4);
	}
/*	  Serial.println("-----------------------------------------------------------");
	  Serial.print("AIN0: "); Serial.print(adc0); Serial.print("  "); Serial.print(volts0); Serial.println("V");
	  Serial.print("AIN1: "); Serial.print(adc1); Serial.print("  "); Serial.print(volts1); Serial.println("V");
	  Serial.print("AIN2: "); Serial.print(adc2); Serial.print("  "); Serial.print(volts2); Serial.println("V");
	  Serial.print("AIN3: "); Serial.print(adc3); Serial.print("  "); Serial.print(volts3); Serial.println("V");

	  delay(1000);*/
}
