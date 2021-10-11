/*
 * templates.h
 *
 *  Created on: 22 sie 2021
 *      Author: jant
 */
#pragma once

#include "silownia.h"

PROGMEM const char HTML_HEADERS[] =	"<!DOCTYPE html><html><head><title>%s</title>"
		"<meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
		"<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css\" >"
		"</head><body style=\"text-align: center;color: white; background: black;font-size: 1.5em;\">\n";

PROGMEM const char HTML_FOOTER[] = 	"<p>Jan Trzciński &copy; 2021 VII Ver. %s</p></td></tr></body></html>";

PROGMEM const char HTML_PAGE1[] = "<h1>%s</h1>"
#ifdef POLISH
		"<p> Napięcie baterii: %.1fV</p>"
		"<p> Prąd baterii: %.1fA</p>"
		"<p> Prąd zasilania: %.1fA</p>"
		"<p> Prąd obciążenia: %.1fA</p>"
#ifdef WEBPAGEWIFISCAN
		"<p><a href = \"/wifiscan\"><button class=\"btn btn-info\">Skanuj WiFi</button></a></p>"
#endif
		"<p><a href = \"/\"><button class=\"btn btn-info\">Odświerz</button></a></p>";
#else
		"<p> Battery voltage: %.1fV</p>"
		"<p> Battery current: %.1fA</p>"
		"<p> Supply current: %.1fA</p>"
		"<p> Load current: %.1fA</p>"
#ifdef WEBPAGEWIFISCAN
		"<p><a href = \"/wifiscan\"><button class=\"btn btn-info\">WiFi Scan</button></a></p>"
#endif
		"<p><a href = \"/\"><button class=\"btn btn-info\">Reload</button></a></p>";
#endif

PROGMEM const char HTML_WIFISCAN[] =
		"<table  align=\"center\" border=\"2\" >"
		"<thead ><tr><th> </th><th style = \"text-align: center;\">SSID</th>"
		"<th>kanał</th><th style = \"text-align: center;\">MAC</th>"
#ifdef POLISH

		"<th style = \"text-align: center;\">RSSI</th><th>zabezp</th><th>ukryta</th><tr></thead><tbody>";
#else

		"<th style = \"text-align: center;\">RSSI</th><th>encryption</th><th>hidden</th><tr></thead><tbody>";
#endif

PROGMEM  const char HTML_WIFISCAN_TAB[] = "<tr><td><form action=\"/wifiset\" metod=\"post\">"
		 "<labe><input id=\"SSID\" type=\"radio\" name=\"SSID\" value=\"%s\"></label>"
		 "</td><td>%s</td><td>%i</td><td>%s</td><td>%i dBm</td><td>%s</td><td>%s</td></tr>";

PROGMEM  const char HTML_WIFISCAN_TAB_END[] = "</tbody></table><div><p></p> </div>";

PROGMEM const char HTML_WIFISCAN1[] =
#ifdef POLISH
		"<p>Jesteś połączony z siecią </p>"
#else
		"<p>Connected with WiFi </p>"
#endif
		"<table align=\"center\" border=\"2\" ><tbody>"
		"<thead ><tr><th style = \"text-align: center;\">SSID</th>"
#ifdef POLISH
		"<th>kanał</th><th style = \"text-align: center;\">MAC</th>"
#else
		"<th>channel</th><th style = \"text-align: center;\">MAC</th>"
#endif
		"<th style = \"text-align: center;\">RSSI</th></thead><tbody>"
		"<td>%s</td><td>%i</td><td>%s</td><td>%i dBm</td></tbody></table>"
		"<label>IP: %s</label>"
#ifdef POLISH
		"<p>Aby zmienić sieć WiFi.</p>";
#else
		"<p>To change the WiFi network.</p>";
#endif

PROGMEM const char HTML_WIFISCAN1A[] =
#ifdef POLISH
		 "<p>Brak połączenia z siecią WiFi.</p>"
#else
		 "<p>No connection with WiFi.</p>"
#endif
		 "<label>IP AP: %s</label>";


PROGMEM const char HTML_WIFISCAN1B[] =
#ifdef POLISH
		"<p>Wybierz sieć powyżej i podaj hasło</p>"
		"<label for=\"password\">Hasło do WiFi:</label>"
#else
		"<p>Select the WiFi network above and enter the password</p>"
		"<label for=\"password\">WiFi password:</label>"
#endif
		"<input style=\"color: black;\" type=\"password\" name=\"password\" />"
#ifdef POLISH
		"<input style=\"text-align: center;color: black;\" type=\"submit\" value=\"Połącz z WiFi.\"/></form>"
		"<p><a href = \"/wifiscan\"><button class=\"btn btn-info\">Skanuj ponownie</button></a></p>"
		"<p><a href = \"/\"><button class=\"btn btn-info\">Strona główna</button></a></p>";
#else
		"<input style=\"text-align: center;color: black;\" type=\"submit\" value=\"Connect.\"/></form>"
		"<p><a href = \"/wifiscan\"><button class=\"btn btn-info\">Scan again</button></a></p>"
		"<p><a href = \"/\"><button class=\"btn btn-info\">Home</button></a></p>";
#endif

