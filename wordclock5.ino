// Wordclock LedStrip layout: __Horizontal_Led__ or __Diagonal_Led__
#define __Diagontal_Led__ TRUE

#include <WiFiManager.h>
#include <Time.h>
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>    // Needed for weather API lib.
#include <ESP8266HTTPClient.h>   // Needed for weather API lib.
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoOTA.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FS.h>



//output pin  neopixels
#define PIN 5

const char* host = "wordclockframe";     //hostname wordclock.local
unsigned int localPort = 2390;      // local port to listen for UDP packets

String idvariable = "";
String token = "";

// Needed for weather API
ESP8266WiFiMulti WiFiMulti;

ESP8266WebServer server(80);


// Time variables
int softwareVersion=5;
int h;
int h2;
int m;
int sec;
int m2;
int timeZone = 1;     // Central European Time
int temperature;
int windspeed;
int show_weather=0;
int tw_counter=0;

int multicolor=1;
int red=128;
int green=128;
int blue=128;

int temp_sec=4;
int wind_sec=4;

//wordclock words

#if defined (__Horizontal_Leds__)
  // Horizontal led strips:
  int HET[] = {0, 1, 2};
  int IS[] = {4, 5};
  int NU[] = {7, 8};
  int UUR[] = {87, 88, 89};
  int KWART[] = {20, 21, 22, 23, 24};
  int OVER[] = {26, 27, 28, 29};
  int VOOR[] = {36, 37, 38, 39};
  int HALF[] = {31, 32, 33, 34};
  int MVIJF[] = {15, 16, 17, 18};
  int MTIEN[] = {11, 12, 13, 14};
  int EEN[] = {80, 81, 82};
  int TWEE[] = {43, 44, 45, 46};
  int DRIE[] = {56, 57, 58, 59};
  int VIER[] = {76, 77, 78, 79};
  int VIJF[] = {83, 84, 85, 86};
  int ZES[] = {47, 48, 49};
  int ZEVEN[] = {61, 62, 63, 64, 65};
  int ACHT[] = {40, 41, 42, 43};
  int NEGEN[] = {65, 66, 67, 68, 69};
  int TIEN[] = {50, 51, 52, 53};
  int ELF[] = {54, 55, 56};
  int TWAALF[] = {70, 71, 72, 73, 74, 75};

  // digits for temperature 
  int D_DIGIT_T[10][14] = { 
                     {59,58,57,56,60,61,62,63,99,99,99,99,99,99},                  // minus sign (-), the 0 is not needed
                     {22,38,37,42,57,62,77,81,82,83,99,99,99,99},                  // 1
                     {21,22,39,36,43,56,62,78,80,81,82,83,99,99},                  // 2
                     {20,21,22,23,37,41,57,63,76,80,81,82,99,99},                  // 3
                     {23,37,36,41,43,59,56,60,61,62,63,76,83,99},                  // 4
                     {20,21,22,23,39,40,41,42,56,63,79,76,81,82},                  // 5
                     {21,22,39,40,59,58,57,60,63,79,76,81,82,99},                  // 6
                     {20,21,22,23,36,43,57,61,78,81,99,99,99,99},                  // 7
                     {21,22,39,36,40,43,58,57,60,63,79,76,81,82},                  // 8
                     {21,22,39,36,40,43,58,57,56,63,76,81,82,99}                   // 9
                    };

  int D_DIGIT_E[10][14] = { 
                     {26,27,34,31,45,48,54,51,65,68,74,71,86,87},                  // 0
                     {27,33,32,47,52,67,72,86,87,88,99,99,99,99},                  // 1
                     {26,27,34,31,48,51,67,73,85,86,87,88,99,99},                  // 2
                     {25,26,27,28,32,46,52,68,71,85,86,87,99,99},                  // 3
                     {28,32,31,46,48,54,51,65,66,67,68,71,88,99},                  // 4
                     {25,26,27,28,34,45,46,47,51,68,74,71,86,87},                  // 5
                     {26,27,34,45,54,53,52,65,68,74,71,86,87,99},                  // 6
                     {25,26,27,28,31,48,52,66,73,86,99,99,99,99},                  // 7
                     {26,27,34,31,45,48,53,52,65,68,74,71,86,87},                  // 8
                     {26,27,34,31,45,48,53,52,51,68,71,86,87,99}                   // 9
                    };

#else
  // Diagonal led strips:
  int HET[] = {0, 1, 5};
  int IS[] = {14, 15};
  int NU[] = {28, 44};
  int UUR[] = {89, 91, 93};
  int KWART[] = {3, 8, 12, 17, 25};
  int OVER[] = {42, 47, 60, 62};
  int VOOR[] = {9, 11, 18, 24};
  int HALF[] = {41, 48, 59, 63};
  int MVIJF[] = {4, 7, 13, 16};
  int MTIEN[] = {26, 29, 43, 46};
  int EEN[] = {36, 53, 54};
  int TWEE[] = {32, 40, 49, 58};
  int DRIE[] = {20, 22, 33, 39};
  int VIER[] = {35, 37, 52, 55};
  int VIJF[] = {68, 70, 80, 83};
  int ZES[] = {64, 74, 76};
  int ZEVEN[] = {34, 38, 51, 56, 66};
  int ACHT[] = {10, 19, 23, 32};
  int NEGEN[] = {66, 72, 78, 85, 87};
  int TIEN[] = {65, 73, 77, 86};
  int ELF[] = {39, 50, 57};
  int TWAALF[] = {67, 71, 79, 84, 88, 92};

  // digits for temperature 
  int D_DIGIT_T[10][14] = { 
                     {20,22,33,39,21,34,38,51,99,99,99,99,99,99},                  // minus sign (-), the 0 is not needed
                     {12,11,18,23,33,38,52,53,54,68,99,99,99,99},                  // 1
                     { 8,12, 9,24,32,39,38,37,36,53,54,68,99,99},                  // 2
                     { 3, 8,12,17,18,19,33,51,55,36,53,54,99,99},                  // 3
                     {17,18,24,19,32,20,39,21,34,38,51,55,68,99},                  // 4
                     { 3, 8,12,17, 9,10,19,23,39,51,35,55,53,54},                  // 5
                     { 8,12, 9,10,20,22,33,21,51,35,55,53,54,99},                  // 6
                     { 3, 8,12,17,24,32,33,34,37,53,99,99,99,99},                  // 7
                     { 8,12, 9,24,10,32,22,33,21,51,35,55,53,54},                  // 8
                     { 8,12, 9,24,10,32,22,33,39,51,55,53,54,99}                   // 9
                    };

  int D_DIGIT_E[10][14] = { 
                     {42,47,41,63,49,74,57,77,66,85,71,88,83,89},                  // 0
                     {47,48,59,64,73,78,84,83,89,91,99,99,99,99},                  // 1
                     {42,47,41,63,74,77,78,79,80,83,89,91,99,99},                  // 2
                     {30,42,47,60,59,58,73,85,88,80,83,89,99,99},                  // 3
                     {60,59,63,58,74,57,77,66,72,78,85,88,91,99},                  // 4
                     {30,42,47,60,41,49,58,64,77,85,71,88,83,89},                  // 5
                     {42,47,41,49,57,65,73,66,85,71,88,83,89,99},                  // 6
                     {30,42,47,60,63,74,73,72,79,83,99,99,99,99},                  // 7
                     {42,47,41,63,49,74,65,73,66,85,71,88,83,89},                  // 8
                     {42,47,41,63,49,74,65,73,77,85,88,83,89,99}                   // 9
                    };
#endif


int palette[][4][3] = {
                         {  {128, 128, 0},   // geel                     
                            {0, 128, 0},     //groen
                            {128, 0, 0} ,    // rood
                            {0, 0, 128}      // blauw
                         },
                         {
                          {128, 128, 0},    // geel                     
                          {0, 128, 0},      //groen
                          {128, 0, 0},      // rood
                          {128, 128, 128}   // wit
                         },
                         {
                          {128, 128, 0},   // geel                     
                          {0, 128, 0},     //groen
                          {128, 0, 0} ,    // rood
                          {128, 0, 128}    // geen idee
                         },                       
                         {
                          {128, 128, 0},   // geel                     
                          {0, 128, 0},     //groen
                          {128, 0, 0} ,    // rood
                          {0, 0, 128}      // blauw
                         }
                      };

int g_brightness = 128;


Adafruit_NeoPixel strip = Adafruit_NeoPixel(94, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

String getContentType(String filename){
  if(server.hasArg("download")) return "application/octet-stream";
  else if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".svg")) return "image/svg+xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(String path){
  
  if(path.endsWith("/")) path += "index.htm";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){
    if(SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}



void getWeather() {
  // Get the Weather data:

  int success=0;
  
  Serial.print("Get the Weather data, opening connection to openweather server."); 

  while (success<3) {
    // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {
      HTTPClient http;
      Serial.println("[HTTP] begin...\n");

      // Server 188.166.125.149 is api.openweathermap.org  Later DNS toevoegen.
      http.begin("api.openweathermap.org", 80, "/data/2.5/weather?q=Amsterdam,nl&units=metric&appid=48e35f6097caa0ca1901254357f6e6b9"); 

      Serial.println("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();
      if(httpCode) {
        // HTTP header has been send and Server response header has been handled
        Serial.println("[HTTP] GET... code:");
        Serial.println(httpCode);

        // file found at server
        if(httpCode == 200) {
          String payload = http.getString();
          Serial.println(payload);
          success = 5;

          // Extract the temperature from the returned string
          if (payload.indexOf("temp") != -1) {
            String str;
            Serial.println("Temp tag found in payload. Value is:");
            str = payload.substring(payload.indexOf("temp") + 6, payload.indexOf("temp") + 8);
            Serial.println(str); 
            Serial.println("Integer value of the string is:");
            temperature=str.toInt();
            Serial.println(temperature);
          }

          // Extract the windspeed from the returned string
          if (payload.indexOf("speed") != -1) {
            String str;
            Serial.println("Speed tag found in payload. Value is:");
            str = payload.substring(payload.indexOf("speed") + 7, payload.indexOf("speed") + 9);
            Serial.println(str); 
            Serial.println("Integer value of the string is:");
            windspeed=msToBeaufort(str.toFloat());
            Serial.println(windspeed);
           
          }
        }
      } else {
        Serial.println("[HTTP] GET... failed, no connection or no HTTP server\n");
        Serial.println("Sleep 5 seconds and retry");
        delay(5000);
        success++;
      }
    }
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


// Switch all dots off
void clearStrip() {
  for (uint16_t i = 0; i < 94  ; i++) {
    strip.setPixelColor(i, 0, 0, 0 );
  }
}




// Fill the dots one after the other with a color
void setPhrase(uint32_t c, int w[], int s) {
  for (uint16_t i = 0; i < s  ; i++) {
    strip.setPixelColor(w[i], c);
  }
}





// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t dynaColor(byte WheelPos, byte m, byte s) {

  if (multicolor<4) {
    m = random(4);
    return strip.Color(palette[multicolor][m][0],palette[multicolor][m][1],palette[multicolor][m][2]);
  } else {
    return strip.Color(red,green,blue);
  }
}




void displayTime()
{
  
  
  /*setPhrase(strip.Color(0, 0, 128), HET, (sizeof(HET) / sizeof(int)));
  setPhrase(strip.Color(0, 128, 0), IS, (sizeof(IS) / sizeof(int)));
  setPhrase(dynaColor(h,m,sec), NU, (sizeof(NU) / sizeof(int)));
  */

  /* Minutes between 5-10 or 55-60 - Light "VIJF," "MINUTES" */
  if ((m >= 5 && m < 10) || (m >= 55 && m < 60)) {
    setPhrase(dynaColor(h,m,sec), MVIJF, (sizeof(MVIJF) / sizeof(int)));
  }

  /* Minutes between 10-15 or 50-55 - Light "TIEN," "MINUTES" */
  if ((m >= 10 && m < 15) || (m >= 50 && m < 55)) {
    setPhrase(dynaColor(h,m,sec), MTIEN, (sizeof(MTIEN) / sizeof(int)));

  }

  /* Minutes between 15-20 or 45-50 - Light "KWART" */
  if ((m >= 15 && m < 20) || (m >= 45 && m < 50)) {
    setPhrase(dynaColor(h,m,sec), KWART, (sizeof(KWART) / sizeof(int)));
  }

  /* Minutes between 20-25 or 40-45 - Light "TIEN," "MINUTES" */
  if ((m >= 20 && m < 25) || (m >= 40 && m < 45)) {
    setPhrase(dynaColor(h,m,sec), MTIEN, (sizeof(MTIEN) / sizeof(int)));
  }

  /* Minutes between 25-30 or 35-40 - Light "VIJF," "MINUTES" */
  if ((m >= 25 && m < 30) || (m >= 35 && m < 40)) {
    setPhrase(dynaColor(h,m,sec), MVIJF, (sizeof(MVIJF) / sizeof(int)));
  }

  /* Minutes between 20-45 - Light "HALF" */
  if ((m >= 20 && m < 45)) {
    setPhrase(dynaColor(h,m,sec), HALF, (sizeof(HALF) / sizeof(int)));
  }

  /* Minutes between 5-20  35-45 - Light "OVER" */
  if ((m >= 5) && (m < 20) ||  (m >= 35 && m < 45) ) {
    setPhrase(dynaColor(h,m,sec), OVER, (sizeof(OVER) / sizeof(int)));
  }


  /* Minutes between 5-15  35-45 - Light "VOOR" */
  if ((m >= 45) && (m < 60) ||  (m >= 20 && m < 30) ) {
    setPhrase(dynaColor(h,m,sec), VOOR, (sizeof(VOOR) / sizeof(int)));
  }

  /* Minutes between 0-5  Light "UUR" */
  if ((m >= 0 && m < 5) ) {
    setPhrase(dynaColor(h,m,sec), UUR, (sizeof(UUR ) / sizeof(int)));
  }

  /* Minutes between 35-60 - MODIFY CURRENT HOUR VALUE */
  if (m >= 20) {

    h++; //Add 1 from current hour
    /*Set time to twelve for hour around midnight, noon */
    if (h == 0) {
      h = 12;
    }
    /*Corner case for 12:35-12:59 */
    if (h == 13) {
      h = 1;
    }
  }

  int dots = m % 5;

  #if defined (__Horizontal_Leds__)
    for (int itter = 93; itter > 93 - dots; itter--) {
      strip.setPixelColor(itter, 0, 128, 0);
    }
  #else
    if (dots > 0) {
      strip.setPixelColor(69, 0, 128, 0);
    } 
    if (dots > 1) {
      strip.setPixelColor(81, 0, 128, 0);
    }
    if (dots > 2) {
      strip.setPixelColor(82, 0, 128, 0);
    }
    if (dots > 3) {
      strip.setPixelColor(90, 0, 128, 0);
    }
  #endif



  /* Hour=1 - Light "ONE" */
  if (h == 1) {
    setPhrase(dynaColor(h,m,sec), EEN, (sizeof(EEN) / sizeof(int)));
  }

  /* Hour=2 - Light "TWO" */
  if (h == 2) {
    setPhrase(dynaColor(h,m,sec), TWEE, (sizeof(TWEE) / sizeof(int)));
  }

  /* Hour=3 - Light "THREE" */
  if (h == 3) {
    setPhrase(dynaColor(h,m,sec), DRIE, (sizeof(DRIE) / sizeof(int)));
  }

  /* Hour=4 - Light "FOUR" */
  if (h == 4) {
    setPhrase(dynaColor(h,m,sec), VIER, (sizeof(VIER) / sizeof(int)));
  }

  /* Hour=5 - Light "FIVE" */
  if (h == 5) {
    setPhrase(dynaColor(h,m,sec), VIJF, (sizeof(VIJF) / sizeof(int)));
  }

  /* Hour=6 - Light "SIX" */
  if (h == 6) {
    setPhrase(dynaColor(h,m,sec), ZES, (sizeof(ZES) / sizeof(int)));
  }

  /* Hour=7 - Light "SEVEN" */
  if (h == 7) {
    setPhrase(dynaColor(h,m,sec), ZEVEN, (sizeof(ZEVEN) / sizeof(int)));
  }

  /* Hour=8 - Light "EIGHT" */
  if (h == 8) {
    setPhrase(dynaColor(h,m,sec), ACHT, (sizeof(ACHT) / sizeof(int)));
  }

  /* Hour=9 - Light "NINE" */
  if (h == 9) {
    setPhrase(dynaColor(h,m,sec), NEGEN, (sizeof(NEGEN) / sizeof(int)));
  }

  /* Hour=10 - Light "TEN" */
  if (h == 10) {
    setPhrase(dynaColor(h,m,sec), TIEN, (sizeof(TIEN) / sizeof(int)));
  }

  /* Hour=11 - Light "ELEVEN" */
  if (h == 11) {
    setPhrase(dynaColor(h,m,sec), ELF, (sizeof(ELF) / sizeof(int)));
  }

  /* Hour=12 - Light "TWELVE" */
  if (h == 12) {
    setPhrase(dynaColor(h,m,sec), TWAALF, (sizeof(TWAALF) / sizeof(int)));
  }

}


void setBrightness(int b) {
  strip.setBrightness(b);
  strip.show();
}



void animateDots(uint8_t wait) {
  uint16_t i, j;

  clearStrip();

  for (j = 0; j < 256; j++) {
    for (i = 0; i < 94; i=i+2) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
clearStrip();
displayTime();
}


//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 4; q++) {
      for (int i=0; i < strip.numPixels(); i=i+4) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+4) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}



void animateTime(uint8_t wait){

clearStrip();
  
  for (int i = 1 ; i < 11 ; i++)
  {
    for (int j = 1 ; j < 60 ; j++)
    {
      strip.clear();
      h = i;
      m = j;
      
      displayTime(); 
      strip.show();
      delay(wait);
    }
 
  }
  clearStrip();
  displayTime();
  }


void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
}


void setup() {

   strip.begin();
   strip.show(); // Initialize all pixels to 'off'
  
  
  Serial.begin(115200);
  Serial.println("Booting");
  // first bootsequence pixel green

    #if defined (__Horizontal_Leds__)
    strip.setPixelColor(93, 0, 128, 0);
  #else
    strip.setPixelColor(69, 0, 128, 0);
  #endif
  strip.show();
  
  
   WiFiManager wifiManager;
  
  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if(!wifiManager.autoConnect(host)) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  } 

  //if you get here you have connected to the WiFi
  
  // 2nd bootsequence pixel green
  #if defined (__Horizontal_Leds__)
    strip.setPixelColor(92, 0, 128, 0);
  #else
    strip.setPixelColor(81, 0, 128, 0);
  #endif
  strip.show();
  
   // Port defaults to 8266
   ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname(host);

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("End");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\n", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

 // getTime();

 // set the clock to GMT
  // clock will be updated automatically

  setTime(getWebUnixTime());
  setSyncProvider(getWebUnixTime);
  
  
  
  getWeather();

  // 3rd bootsequence pixel green
  #if defined (__Horizontal_Leds__)
    strip.setPixelColor(90, 0, 128, 0);
  #else
    strip.setPixelColor(82, 0, 128, 0);
  #endif
  strip.show();
 
  // Multicast DNS , bonjour like
  // clock will be available as  e.g.  wordclock.local
  MDNS.begin(host);


  //use it to load content from SPIFFS
  server.onNotFound([](){
    if(!handleFileRead(server.uri()))
      server.send(404, "text/plain", "FileNotFoundhhhhh");
  });


  server.on("/animate1", HTTP_GET, [](){
    String json = "{";
    json += "\"Status\":\"OK\"";
    json += "}";
    server.send(200, "text/json", json);
    json = String();
     animateTime(100);
  });

  server.on("/animate2", HTTP_GET, [](){
    String json = "{";
    json += "\"Status\":\"OK\"";
    json += "}";
    server.send(200, "text/json", json);
    json = String();
    animateDots(50);
  });


  server.on("/animate3", HTTP_GET, [](){
    String json = "{";
    json += "\"Status\":\"OK\"";
    json += "}";
    server.send(200, "text/json", json);
    json = String();
    theaterChaseRainbow(100);
  });
  
  server.on("/time", HTTP_GET, [](){
    String json = "{";
    json += "\"time\":\""+String(day())+"/"+String(month())+"/"+String(year())+"  "+String(hour())+":"+String(minute())+":"+String(second())+"\"";
    json += ", \"timezone\":\""+String(timeZone)+"\"";
    json += ", \"softwareversion\":\""+String(softwareVersion)+"\"";
    json += "}";
    server.send(200, "text/json", json);
    json = String();
  });
 
  server.on("/brightness", HTTP_GET, [](){
    Serial.printf("Change the brightness of the clock.");
    String b_value = server.arg("value");
    if (b_value.length() > 0 ) {
      Serial.printf("value argument found.\n");
      g_brightness = b_value.toInt();
      Serial.printf("Integer value: %d\n", g_brightness);
      setBrightness(g_brightness);
    }    
    String json = "{";
    json += "\"Status\":\"OK\"";
    json += "}";
    server.send(200, "text/json", json);
    json = String();
  });

  server.on("/multicolor", HTTP_GET, [](){
    Serial.printf("Change colors on or off.");
    String b_value = server.arg("value");
    if (b_value.length() > 0 ) {
      Serial.printf("value argument found.\n");
      multicolor = b_value.toInt();
      Serial.printf("Integer value: %d\n", multicolor);
      clearStrip();
      displayTime();
      strip.show();
    }    
    String json = "{";
    json += "\"Status\":\"OK\"";
    json += "}";
    server.send(200, "text/json", json);
    json = String();
  });

  server.on("/red", HTTP_GET, [](){
    Serial.printf("Change red color value.");
    String b_value = server.arg("value");
    if (b_value.length() > 0 ) {
      Serial.printf("value argument found.\n");
      red = b_value.toInt();
      Serial.printf("Integer value: %d\n", red);
      clearStrip();
      displayTime();
      strip.show();
    }    
    String json = "{";
    json += "\"Status\":\"OK\"";
    json += "}";
    server.send(200, "text/json", json);
    json = String();
  });

  server.on("/green", HTTP_GET, [](){
    Serial.printf("Change green color value.");
    String b_value = server.arg("value");
    if (b_value.length() > 0 ) {
      Serial.printf("value argument found.\n");
      green = b_value.toInt();
      Serial.printf("Integer value: %d\n", red);
      clearStrip();
      displayTime();
      strip.show();
    }    
    String json = "{";
    json += "\"Status\":\"OK\"";
    json += "}";
    server.send(200, "text/json", json);
    json = String();
  });

  server.on("/blue", HTTP_GET, [](){
    Serial.printf("Change blue color value.");
    String b_value = server.arg("value");
    if (b_value.length() > 0 ) {
      Serial.printf("value argument found.\n");
      blue = b_value.toInt();
      Serial.printf("Integer value: %d\n", red);
      clearStrip();
      displayTime();
      strip.show();
    }    
    String json = "{";
    json += "\"Status\":\"OK\"";
    json += "}";
    server.send(200, "text/json", json);
    json = String();
  });

  server.on("/temp_secs", HTTP_GET, [](){
    Serial.printf("Change the number of seconds to show the temperature.");
    String b_value = server.arg("value");
    if (b_value.length() > 0 ) {
      Serial.printf("value argument found.\n");
      temp_sec = b_value.toInt();
      Serial.printf("Integer value: %d\n", temp_sec);
    }    
    String json = "{";
    json += "\"Status\":\"OK\"";
    json += "}";
    server.send(200, "text/json", json);
    json = String();
  });

  server.on("/wind_secs", HTTP_GET, [](){
    Serial.printf("Change the number of seconds to show the wind speed.");
    String b_value = server.arg("value");
    if (b_value.length() > 0 ) {
      Serial.printf("value argument found.\n");
      wind_sec = b_value.toInt();
      Serial.printf("Integer value: %d\n", wind_sec);
    }    
    String json = "{";
    json += "\"Status\":\"OK\"";
    json += "}";
    server.send(200, "text/json", json);
    json = String();
  });

  server.on("/tzone", HTTP_GET, [](){
    Serial.printf("Change the timezone of the clock.");
    String b_value = server.arg("value");
    if (b_value.length() > 0 ) {
      Serial.printf("value argument found.\n");
      h=hour();
      if (h - timeZone < 0) {
        h = h + 24 - timeZone;
      } else {
        h = h - timeZone;
      }            
      timeZone = b_value.toInt();
      Serial.printf("Integer value: %d\n", timeZone);
      m = minute();      // Returns the minute of current time
      sec = second();     // Returns the second of current time
      h=h+timeZone;
      if (h > 23) {
        h = h - 24;
      }
      setTime(h,m,sec,1,1,2016);
      
      h = hourFormat12();
      clearStrip();
      displayTime();
      strip.show();
    }

    String json = "{";
    json += "\"Status\":\"OK\"";
    json += "}";
    server.send(200, "text/json", json);
    json = String();
  });


  // initialize filesystem
  SPIFFS.begin();
  // start webserver
  server.begin();

 m2=-1;
 h2=-1;
 
}

void show_temperature() {
  int i, j, k;
  
  clearStrip();

  if (temperature < 0) {
    k=0;
    j=(0-temperature) % 10;
  } else {
    j = temperature % 10;
    k = (temperature - j) / 10;
  }
  for (i=0; i<14; i++) {
    strip.setPixelColor(D_DIGIT_E[j][i],128,128,128);
  }

  if (k > 0 || temperature < 0) {
    for (i=0; i<14; i++) {
      strip.setPixelColor(D_DIGIT_T[k][i],128,128,128);
    }
  }

  // Show the 'Degree' dot:
  #if defined (__Horizontal_Leds__)
    strip.setPixelColor(9,128,128,128);
  #else
    strip.setPixelColor(45,128,128,128);
  #endif  
  strip.show();  
}



void show_windspeed() {
  int i, j, k;
  
  clearStrip();

  j = windspeed % 10;
  k = (windspeed - j) / 10;
  for (i=0; i<14; i++) {
    strip.setPixelColor(D_DIGIT_E[j][i],128,128,128);
  }

  if (k > 0) {
    for (i=0; i<14; i++) {
      strip.setPixelColor(D_DIGIT_T[k][i],128,128,128);
    }
  }


  // Show the 'BFT'
  #if defined (__Horizontal_Leds__)
    strip.setPixelColor(6,128,128,128);
    strip.setPixelColor(7,128,128,128);
    strip.setPixelColor(8,128,128,128);
  #else
    strip.setPixelColor(27,128,128,128);
    strip.setPixelColor(28,128,128,128);
    strip.setPixelColor(44,128,128,128);
  #endif
  strip.show();  
}



void loop() {

  ArduinoOTA.handle();

  /* Get current time */
  h = hourFormat12();  // Returns the hour of current time between 1-12
  m = minute();      // Returns the minute of current time
  sec = second();     // Returns the second of current time

   Serial.println(tw_counter);

  if (tw_counter > 10000) {
   // getTime();
    getWeather();
    tw_counter=0;
  }

  if (m!=m2)  {
    clearStrip();
    displayTime();
    m2=m;
    strip.show();
  } else {
    if (sec > 60 - (wind_sec + temp_sec)) {
      if (sec > 60 - temp_sec) {
        show_temperature();
      } else {
        show_windspeed();    
      }
    }
  }
  delay(200);
  tw_counter++;
  
  server.handleClient();
}










