/*******************************************************************
    Read YouTube Channel statistics from the YouTube API on
    an ESP8266 and print them to the serial monitor and MAX7219, 8 Digit, 7 Segment.

    Written by Angel Mendez
    YouTube: https://www.youtube.com/channel/UCmKV2ng9JKo0mTUcYzB3Enw
    Twitter: https://twitter.com/funkyjamma
    Twitch: https://www.twitch.tv/funkyjamma
 *******************************************************************/

//Libraries
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "DigitLedDisplay.h"
#include <YoutubeApi.h>
#include <ArduinoJson.h>

// Display
DigitLedDisplay ld = DigitLedDisplay(2, 0, 4);

// User Info Replace
char ssid[] = "Replace_SSID";       // your network SSID (name)
char password[] = "Replace_Password";  // your network key
#define API_KEY "Youtube_Api"  // your google apps API Token
#define CHANNEL_ID "UCmKV2ng9JKo0mTUcYzB3Enw" // makes up the url of channel
//----------------//

WiFiClientSecure client;
YoutubeApi api(API_KEY, client);

// Change time between requests in miliseconds 
unsigned long timeBetweenRequests = 60000;
unsigned long nextRunTime;

long subs = 0;

void setup() {

  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  // Required if you are using ESP8266 V2.5 or above
  client.setInsecure();

  // If you want to enable some extra debugging
  api._debug = true;

  // Set the brightness min:1, max:15
  ld.setBright(10);

  // Set the digit count
  ld.setDigitLimit(8);
}

void loop() {

  if (millis() > nextRunTime)  {
    if(api.getChannelStatistics(CHANNEL_ID))
    {
      // Prints data to the console
      Serial.println("---------Stats---------");
      Serial.print("Subscriber Count: ");
      Serial.println(api.channelStats.subscriberCount);
      Serial.print("View Count: ");
      Serial.println(api.channelStats.viewCount);
      Serial.print("Comment Count: ");
      Serial.println(api.channelStats.commentCount);
      Serial.print("Video Count: ");
      Serial.println(api.channelStats.videoCount);
      Serial.println("------------------------");
      
      // Prints data to the display
      ld.printDigit(api.channelStats.subscriberCount);

    }
    nextRunTime = millis() + timeBetweenRequests;
  }
}
