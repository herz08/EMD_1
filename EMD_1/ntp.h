#ifndef __NTP_H_
#define __NTP_H_

#include "time.h"
#include "esp_sntp.h"

char datum[100], zeit[100], monat[4], timeStamp[100], zeitHoMi[100];

long millisNTP = 0;

const char *ntpServer1 = NTP_SERVER_1;
const char *ntpServer2 = NTP_SERVER_2;
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

const char *time_zone = "CET-1CEST,M3.5.0,M10.5.0/3";  // TimeZone rule for Europe/Rome including daylight adjustment rules (optional)

void makeClock(struct tm* timeinfo) {
    strftime (datum,100,"%d.%m.%Y",timeinfo);
    strftime (zeit,100,"%H.%M.%S",timeinfo);
    strftime (zeitHoMi,100,"%H.%M",timeinfo);
    strftime (timeStamp,100,"%H.%M.%S__%d.%m.%Y",timeinfo);
    strftime (monat,4,"%m",timeinfo);
}
void getClockNTP() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.print("NTP                : ");Serial.printf(" No time available (yet)\n");
      return;
    }
    makeClock(&timeinfo);
}

void serialPrintClock() {
    getClockNTP();
    Serial.print("Datum / Uhrzeit    :  ");Serial.printf("%s / %s\n", datum, zeit);
}

void tftPrintClock(){
    getClockNTP();
    tftPrintInit("ESP Date       : %s", datum);
    tftPrintInit("ESP Time       : %s", zeit);
}
int makeMonth(){
    getClockNTP();
    return atoi(monat);
}

// Callback function (gets called when time adjusts via NTP)
void timeavailable(struct timeval *t) {
  Serial.print("NTP adjustment     : ");Serial.printf(" Get time\n");
  serialPrintClock();
}
void initNTP(){
  sntp_set_time_sync_notification_cb(timeavailable);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
  //configTzTime(time_zone, ntpServer1, ntpServer2);
}

#endif // __NTP_H_
