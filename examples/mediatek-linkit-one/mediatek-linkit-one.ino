

/*
  Web client

 This sketch connects to a website
 using Wi-Fi functionality on MediaTek LinkIt platform.

 Change the macro WIFI_AP, WIFI_PASSWORD, WIFI_AUTH and SITE_URL accordingly.

 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe
 modified 20 Aug 2014
 by MediaTek Inc.
 */

 /*
  * Adapted to use Sensirion Environmental Sensing Shield by
  * Johannes Winkelmann, jwi@sensirion.com
  */

#include <LWiFi.h>
#include <LWiFiClient.h>

#include <sensirion_ess.h>
#include <Wire.h>

#define WIFI_AP "<WIFI_SSID>"
#define WIFI_PASSWORD "<WIFI_PASSWORD>"
#define WIFI_AUTH LWIFI_WPA  // choose from LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP.
#define SITE_URL "dweet.io"
#define BASE_URL "/dweet/for/<thing>"

LWiFiClient c;

SensirionESS ess;

void setup()
{
  LWiFi.begin();
  Serial.begin(115200);

  Wire.begin();
  if (ess.initSensors() == 0) {
      Serial.print("init(): success\n");
  } else {
      Serial.print("init(): failed\n");
  }

  // keep retrying until connected to AP
  Serial.println("Connecting to AP");
  while (0 == LWiFi.connect(WIFI_AP, LWiFiLoginInfo(WIFI_AUTH, WIFI_PASSWORD)))
  {
    delay(1000);
  }

}

void sendAndRead(float temp, float rh, float voc)
{
  // keep retrying until connected to website
  Serial.println("Connecting to WebSite");
  while (0 == c.connect(SITE_URL, 80)) {
    Serial.println("Re-Connecting to WebSite");
    delay(1000);
  }

  // send HTTP request, ends with 2 CR/LF
  Serial.println("send HTTP GET request");
  c.print("GET ");
  c.print(BASE_URL);
  c.print("?temp=");
  c.print(temp);
  c.print("&rh=");
  c.print(rh);
  c.print("&tvoc=");
  c.print(voc);
  c.println(" HTTP/1.1");
  c.println("Host: " SITE_URL);
  c.println("Connection: close");
  c.println();

  // waiting for server response
  Serial.println("waiting HTTP response:");
  while (!c.available()) {
    delay(100);
  }

  // Make sure we are connected, and dump the response content to Serial
  while (c) {
    int v = c.read();
    if (v != -1) {
      Serial.print((char)v);
    } else {
      Serial.println("no more content, disconnect");
      c.stop();
    }
  }
}

float temp, rh, tvoc = -1;

void loop()
{
  temp, rh, tvoc = -1;

  if (ess.measureRHT() != 0) {
    Serial.print("Error while measuring RHT: ");
    Serial.print(ess.getError());
    Serial.print("\n");
  } else {
    temp = ess.getTemperature();
    rh = ess.getHumidity();
  }

  if (ess.measureIAQ() != 0) {
    Serial.print("Error while measuring IAQ: ");
    Serial.print(ess.getError());
    Serial.print("\n");
  } else {
    tvoc = ess.getTVOC();
  }

  sendAndRead(temp, rh, tvoc);

  delay(ess.remainingWaitTimeMS());
}
