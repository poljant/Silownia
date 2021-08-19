/*
 * WebPagers.h
 *
 *  Created on: 6 lis 2019
 *      Author: jant
 */

#ifndef WEBPAGERS_H_
#define WEBPAGERS_H_

#include <ESP8266WebServer.h>
//#include <ESP8266HTTPUpdateServer.h>

String WebPage();
String HTMLHeader();
String HTMLFooter();
String HTMLPage();
String HTMLPage1();
String HTMLPage2();
void setservers(void);



#endif /* WEBPAGERS_H_ */
