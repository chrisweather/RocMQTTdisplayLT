/*########################################################################################
                             Roc-MQTT-DisplayLT
Dynamic Passenger Information for Model Railroad Stations controlled by Rocrail via MQTT.
An ESP8266-01 controller drives an 0.91" I2C OLED display. 
Several ESPs can run in parallel so the total number of displays is not limited.

Version 1.00  January 04, 2022

Copyright (c) 2020-2022 Christian Heinrichs. All rights reserved.
https://github.com/chrisweather/RocMQTTdisplayLT

##########################################################################################


Message Format sent from Rocrail text fields via MQTT
#####################################################

  Format: ZZAMSG#Targets#Template#Station#Track#Destination#Departure#Train#TrainType#Message#Spare1#Spare2#

    ZZAMSG:          Identifier for relevant MQTT messages
    Targets:         Identifier of the display, e.g. D01 = Display01
    Template:        Identifier for Template, T0 = Template 0, T1 = Template 1, ..., can include only one Template e.g.: T0

    0 - Station:     Name of the station
    1 - Track:       Track number
    2 - Destination: Destination city
    3 - Departure:   Departure time
    4 - Train:       Train number
    5 - TrainType:   Train Type for Logo selection
    6 - Message:     Flexible messages to display either static in the middle of the display or as a ticker at the top.
                     A message in the middle: TPL_xscroll must be 0
                     Example 1: ZZAMSG#D01#T0#Bhf01#1#####Zugdurchfahrt###
                     A message ticker at the top: TPL_xscroll must be 1
    7 - Spare1:      For future features
    8 - Spare2:      For future features

                     Rocrail dynamic text variables can be used https://wiki.rocrail.net/doku.php?id=text-gen-de#dynamischer_text
                     Example 2: ZZAMSG#D01#T0#Bhf01#1#####%lcid%###

                     For dynamic time use:
                     NTP time:     ZZAMSG#D01#T0#Bhf01#1#####{ntptime}###
                     Rocrail time: ZZAMSG#D01#T0#Bhf01#1#####{rrtime}###

                     Clear Display D01
                     Example 3: ZZAMSG#D01###########

    Example 4: ZZAMSG#D01#T0#Hamburg-Hbf#1#Bonn#10:22#ICE 597#ICE####
    Example 5: ZZAMSG#D01#T1#Bhf01#1#Köln-Bonn#10:22#ICE 597#ICE#5min Verspätung###
    Example 6: ZZAMSG#D01D02#T0#Station01#1#Bonn#10:22#IC 56#IC#5min delayed###

    Example 7: Stationname: ZZAMSG#D01#T6#Bogenhausen#########


########################################################################################*/

#include <Arduino.h>
#include <string>
#include <time.h>
#include <LittleFS.h>          // LittleFS file system https://github.com/esp8266/Arduino/tree/master/libraries/LittleFS
#include "config.h"            // Roc-MQTT-Display configuration file
#include "template.h"          // Roc-MQTT-Display template file
#include "web.h"               // Roc-MQTT-Display web file
#include <ArduinoOTA.h>        // ArduinoOTA by Juraj Andrassy https://github.com/jandrassy/ArduinoOTA
#include "EspMQTTClient.h"     // EspMQTTClient by Patrick Lapointe https://github.com/plapointe6/EspMQTTClient
#include <U8g2lib.h>           // U8g2lib by Oliver Kraus https://github.com/olikraus/u8g2
using namespace std;

// More U8G2 Display Constructors are listed in the U8G2 Wiki: https://github.com/olikraus/u8g2/wiki/u8g2setupcpp
// Please uncomment only one constructor! Only one display type can be handled by one Roc-MQTT-DisplayLT controller.

// ### 128x32 ### OLED I2C Display, Define OLED Display as disp (D2: SDA, D1: SCL)
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C disp(U8G2_R0, U8X8_PIN_NONE, 0, 3);

// ### 128x64 ### OLED I2C Display, Define OLED Display as disp (D2: SDA, D1: SCL)
//U8G2_SSD1306_128X64_NONAME_F_HW_I2C disp(U8G2_R0, U8X8_PIN_NONE);

// ### 64x48 ### OLED I2C Display, Define OLED Display as disp (D2: SDA, D1: SCL)
//U8G2_SSD1306_64X48_ER_F_HW_I2C disp(U8G2_R0, U8X8_PIN_NONE);

// ### 96x16 ### OLED I2C Display, Define OLED Display as disp (D2: SDA, D1: SCL)
//U8G2_SSD1306_96X16_ER_F_HW_I2C disp(U8G2_R0, U8X8_PIN_NONE);


u8g2_uint_t offset1;  // current offset for the scrolling text
u8g2_uint_t width1;  // pixel width of the scrolling text (must be < 128 unless U8G2_16BIT is defined)

// Define WIFI/MQTT Client
EspMQTTClient client( sec.WIFI_SSID, sec.WIFI_PW, config.MQTT_IP, sec.MQTT_USER, sec.MQTT_PW, config.WIFI_DEVICENAME, config.MQTT_PORT );
//EspMQTTClient client;

// Define global variables
unsigned long lastMsg = 0;    // used by ScreenSaver
unsigned long lastNTP = 0;    // used by NTP
time_t now;
tm tm;
String ntptime = "00:00";
String rrtime = "00:00";

String ZZA1_Targets =     "";
String ZZA1_Template =    "";
String ZZA1_Station =     "";
String ZZA1_Track =       "";
String ZZA1_Destination = "";
String ZZA1_Departure =   "";
String ZZA1_Train =       "";
String ZZA1_Type =        "";
String ZZA1_MessageO =    "";
String ZZA1_Message =     "";
String ZZA1_MessageLoop = "";


// SETUP, runs once at startup
void setup()
{
  Serial.begin(38400);
  while (!Serial) continue;
  delay(500);

  Serial.print(F("\n\n\nStarting Roc-MQTT-DisplayLT..."));

  // Initialize LittleFS File System
  if(!LittleFS.begin()){
    Serial.println(F("LittleFS Mount Failed"));
    return;
  }

  TPL = 0;
  // Load config from file
  Serial.print(F("\nLoading configuration from \n"));
  Serial.println(configfile);
  loadConfiguration(configfile, config);

  // Save config to file
  Serial.print(F("\nSaving configuration to \n"));
  Serial.println(configfile);
  saveConfiguration(configfile, config);

  config.DISPWIDTH = disp.getDisplayWidth();
  config.DISPHEIGHT = disp.getDisplayHeight();

  // Load sec from file
  Serial.print(F("\nLoading sec from \n"));
  Serial.println(secfile);
  loadSecData(secfile, sec);

  // Load template data from file
  Serial.print(F("\nLoading template data from \n"));
  Serial.println(templatefile);
  loadTemplate(templatefile, templ);
/*
  Save template data to file
  Serial.print(F("\nSaving template data to "));
  Serial.println(templatefile);
  saveTemplate(templatefile, templ);

  TPL = 0;
  // Save template00 to file
  Serial.print(F("\nSaving template00 to "));
  Serial.println(template00);
  saveTemplateFile(template00);

  TPL = 1;
  // Save template01 to file
  Serial.print(F("\nSaving template01 to "));
  Serial.println(template01);
  saveTemplateFile(template01);

  TPL = 2;
  // Save template02 to file
  Serial.print(F("\nSaving template02 to "));
  Serial.println(template02);
  saveTemplateFile(template02);

  TPL = 3;
  // Save template03 to file
  Serial.print(F("\nSaving template03 to "));
  Serial.println(template03);
  saveTemplateFile(template03);

  TPL = 4;
  // Save template04 to file
  Serial.print(F("\nSaving template04 to "));
  Serial.println(template04);
  saveTemplateFile(template04);

  TPL = 5;
  // Save template05 to file
  Serial.print(F("\nSaving template05 to "));
  Serial.println(template05);
  saveTemplateFile(template05);

  TPL = 6;
  // Save template06 to file
  Serial.print(F("\nSaving template06 to "));
  Serial.println(template06);
  saveTemplateFile(template06);

  TPL = 7;
  // Save template07 to file
  Serial.print(F("\nSaving template07 to "));
  Serial.println(template07);
  saveTemplateFile(template07);

  TPL = 8;
  // Save template08 to file
  Serial.print(F("\nSaving template08 to "));
  Serial.println(template08);
  saveTemplateFile(template08);

  TPL = 9;
  // Save template09 to file
  Serial.print(F("\nSaving template09 to "));
  Serial.println(template09);
  saveTemplateFile(template09);
*/

  // Load template0x from file
  Serial.print(F("\nLoading templates from \n"));

  TPL = 0;
  Serial.println(template00);
  loadTemplateFile(template00);

  TPL = 1;
  Serial.println(template01);
  loadTemplateFile(template01);

  TPL = 2;
  Serial.println(template02);
  loadTemplateFile(template02);

  TPL = 3;
  Serial.println(template03);
  loadTemplateFile(template03);

  TPL = 4;  
  Serial.println(template04);
  loadTemplateFile(template04);

  TPL = 5;
  Serial.println(template05);
  loadTemplateFile(template05);

  TPL = 6;
  Serial.println(template06);
  loadTemplateFile(template06);

  TPL = 7;
  Serial.println(template07);
  loadTemplateFile(template07);

  TPL = 8;
  Serial.println(template08);
  loadTemplateFile(template08);

  TPL = 9;
  Serial.println(template09);
  loadTemplateFile(template09);
  TPL = 0;
  delay(500);

  if (config.MQTT_DEBUG == 1){
    // Dump configuration files
    Serial.print(F("\nPrint "));
    Serial.println(secfile);
    printFile(secfile);
  
    Serial.print(F("\nPrint "));
    Serial.println(configfile);
    printFile(configfile);

    Serial.print(F("\nPrint "));
    Serial.println(templatefile);
    printFile(templatefile);
  
    Serial.print(F("\nPrint "));
    Serial.println(template00);
    printFile(template00);

    Serial.print(F("\nPrint "));
    Serial.println(template01);
    printFile(template01);

    Serial.print(F("\nPrint "));
    Serial.println(template02);
    printFile(template02);

    Serial.print(F("\nPrint "));
    Serial.println(template03);
    printFile(template03);

    Serial.print(F("\nPrint "));
    Serial.println(template04);
    printFile(template04);

    Serial.print(F("\nPrint "));
    Serial.println(template05);
    printFile(template05);

    Serial.print(F("\nPrint "));
    Serial.println(template06);
    printFile(template06);

    Serial.print(F("\nPrint "));
    Serial.println(template07);
    printFile(template07);

    Serial.print(F("\nPrint "));
    Serial.println(template08);
    printFile(template08);

    Serial.print(F("\nPrint "));
    Serial.println(template09);
    printFile(template09);
  }

  // Switch off ESP8266-01 onboard LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);

  Serial.println(F("\nRoc-MQTT-DisplayLT"));
  Serial.println(config.VER);
  Serial.print(F("\nController: http://"));
  Serial.println(config.WIFI_DEVICENAME);
  Serial.print(F("  Display size: "));
  Serial.print(config.DISPWIDTH);
  Serial.print(" x ");
  Serial.println(config.DISPHEIGHT);

  //client.setWifiCredentials(sec.WIFI_SSID, sec.WIFI_PW);
  //client.setMqttClientName(config.WIFI_DEVICENAME);
  //client.setMqttServer(config.MQTT_IP, sec.MQTT_USER, sec.MQTT_PW, config.MQTT_PORT);

  // Optional functionalities of EspMQTTClient
  client.enableDebuggingMessages(config.MQTT_DEBUG);
  client.setMaxPacketSize(config.MQTT_MSGSIZE);
  client.setKeepAlive(config.MQTT_KEEPALIVE1);
  client.setMqttReconnectionAttemptDelay(config.MQTT_RECONDELAY);
  client.setWifiReconnectionAttemptDelay(config.WIFI_RECONDELAY);

  // Initialize NTP time client
  configTime(config.NTP_TZ, config.NTP_SERVER);

  // Initialize Display
  DisplayInit();

  // Initialize OTA
  ArduinoOTA.setPort(config.OTA_PORT);
  ArduinoOTA.setHostname(config.OTA_HOSTNAME);
  ArduinoOTA.setPassword(sec.OTA_PW);
  ArduinoOTA.setPasswordHash(sec.OTA_HASH);

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }
    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("OTA Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println(F("\nOTA End"));
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("OTA Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("OTA Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println(F("OTA Authentication Failed"));
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println(F("OTA Begin Failed"));
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println(F("OTA Connect Failed"));
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println(F("OTA Receive Failed"));
    } else if (error == OTA_END_ERROR) {
      Serial.println(F("OTA End Failed"));
    }
  });
  ArduinoOTA.begin();
  Serial.print(F("\nOTA Ready -> Port: "));
  Serial.print(config.OTA_HOSTNAME);
  Serial.print(F(" at "));
  Serial.println(WiFi.localIP());
  Serial.print(F("\nFOR CONFIGURATION OPEN: http://"));
  Serial.println(WiFi.localIP());


  // Initialize WEBSERVER
  webserver.on("/", []() {             // Define the handling function for / path
    loadRoot();
  });

  webserver.on("/index.htm", []() {    // Define the handling function for /index.htm path
    loadRoot();
  });

  webserver.on("/favicon.ico", []() {  // Define the handling function for favicon requests
    webserver.send(204);
  });

  webserver.on("/rmd.css", []() {      // Define the handling function for css file requests
    loadCSS();
  });

  webserver.on("/config", []() {       // Define the handling function for the /config path
    loadCfg();
  });

  webserver.on("/tpl1", []() {     // Define the handling function for the /tpl1 path
    loadTpl1();
  });

  webserver.on("/tpl2", []() {     // Define the handling function for the /tpl2 path
    loadTpl2();
  });

  webserver.on("/tpl2sel", []() {     // Define the handling function for the /tpl2sel path
    handleTpl2Select();
  });

  webserver.on("/sec", []() {          // Define the handling function for the /sec path
    loadSec();
  });

  webserver.on("/submitcfg", []() {    // Define the handling function for the /submitcfg path
    webserver.send(204);
    handleCfgSubmit();
    loadCfg();
  });

  webserver.on("/submittpl1", []() {    // Define the handling function for the /submittpl1 path
    webserver.send(204);
    handleTpl1Submit();
    loadTpl1();
  });

  webserver.on("/submittpl2", []() {    // Define the handling function for the /submittpl2 path
    webserver.send(204);
    handleTpl2Submit();
    loadTpl2();
  });

  webserver.on("/submitsec", []() {    // Define the handling function for the /submitsec path
    webserver.send(204);
    handleSecSubmit();
    loadSec();
  });

  webserver.on("/download", []() {    // Define the handling function for the /download path
    //webserver.send(204);
    downloadFile();
  });

  webserver.on("/restart", []() {      // Define the handling function for the /restart path
    webserver.send(204);
    delay(300);
    restartESP();
  });

  webserver.on("/ota", []() {      // Define the handling function for the /ota path
    webserver.send(204);
    delay(500);
    stopLittleFS();
  });

  webserver.onNotFound([]() {          // Define the handling function for Site Not Found response
    loadNotFound();
  });

  webserver.begin();                   // Start the webserver
  Serial.println(F("\nWebserver started and listening for requests"));
}


// Restart the controller
void restartESP()
{
  ESP.restart();
}


// Initialize all connected displays
void DisplayInit()
{
    int i = 0;
    disp.sendBuffer();  // Send buffer to display
    disp.clearBuffer();
    disp.firstPage();
    do {
      disp.begin();  // Initialize display i
      disp.setFlipMode(DPL_flip[i]);
      //disp.setDisplayRotation(U8G2_R2);  // U8G2_R0, U8G2_R1, U8G2_R2, U8G2_R3, U8G2_MIRROR  DPL_rotation[i]
      disp.setContrast(255);
      disp.enableUTF8Print();
      disp.setFont(fontno[6]);
      disp.setFontMode(0);
      disp.setCursor(3,13);
      disp.print(F("Roc-MQTT-DisplayLT"));
      disp.setCursor(3,31);
      disp.print(config.VER);
      disp.nextPage();
      delay(500 + (config.STARTDELAY / 2));
      disp.clearDisplay();
      disp.setContrast(DPL_contrast[i]);
      disp.setFont(fontno[6]);
      disp.setCursor(0,13);
      disp.print(F("http://"));
      disp.setCursor(0,29);
      disp.print(config.WIFI_DEVICENAME);
      disp.nextPage();
      delay(500 + (config.STARTDELAY));
      disp.clearDisplay();
      disp.setCursor(0,13);
      disp.print(F("Display ID: "));
      disp.print(DPL_id[i]);
      Serial.print(F("Display ID: "));
      Serial.println(DPL_id[i]);
    } while (disp.nextPage());
  //}
  delay(500 + (config.STARTDELAY / 2));
}


// *** Write buffer to Display 1 ***
void send2display1(void)
{ 
  // Template number
  uint8_t t = ZZA1_Template.toInt();
  if (t < 0 || t > 9){
    t = 0;
  }
  if (DPL_side[0] == 1){
    if (TPL_side[t] == 0 && TPL_side[t+1] == 1){
      t = t+1;
    }
  }

  disp.sendBuffer();  // Send buffer to display
  u8g2_uint_t x;
  disp.firstPage();
  
  // Message
  // *** Message only, in the middle of the display ***
  if (TPL_6scroll[t] != 1){
    if (ZZA1_Message.length() > 1){
      disp.setFont(fontno[TPL_6font[t]]);
      // Switch to narrow font for longer messages
      if (disp.getUTF8Width(ZZA1_Message.c_str()) > TPL_6maxwidth[t]){
        disp.setFont(fontno[TPL_6font2[t]]);
      }
      disp.setFontMode(TPL_6fontmode[t]);
      disp.setDrawColor(TPL_6drawcolor[t]);
      
      disp.setCursor(TPL_6posx[t] + (TPL_6maxwidth[t] / 2) - (disp.getUTF8Width(ZZA1_Message.c_str()) / 2), TPL_6posy[t]);
      disp.print(ZZA1_Message);
    }
  }
  else {
    // *** Scrolling message on top ***
    if (ZZA1_Message.length() > 1){
      disp.setFont(fontno[TPL_6font[t]]);
      width1 = disp.getUTF8Width(ZZA1_MessageLoop.c_str());
      disp.setFontMode(TPL_6fontmode[t]);
      disp.setDrawColor(TPL_6drawcolor[t]);
      disp.drawBox(TPL_6boxx[t], TPL_6boxy[t], TPL_6boxw[t], TPL_6boxh[t]);
      disp.setFontMode(TPL_6fontmode2[t]);
      disp.setDrawColor(TPL_6drawcolor2[t]);
      disp.drawBox(TPL_6box2x[t], TPL_6box2y[t], TPL_6box2w[t], TPL_6box2h[t]);
 
      x = offset1;
      do {
        disp.drawUTF8(x, TPL_6posy[t], ZZA1_MessageLoop.c_str());  // !!!!!!!!!!!!!
        x += width1;
      } while( x < disp.getDisplayWidth());
    }
  }
  disp.setFontMode(TPL_6fontmode[t]);
  disp.setDrawColor(TPL_6drawcolor[t]);

  // Station
  disp.setFont(fontno[TPL_0font[t]]);
  //Serial.println(ZZA1_Station);
  //if (int a = ZZA1_Station.indexOf("\n") != -1){
    //Serial.println("line break");
    //Serial.println(a);
  //}
  //int start09 = start08 + 1 + pld.substring(start08).indexOf("#");
  
  // Switch to narrow font for longer station names
  if (disp.getUTF8Width(ZZA1_Station.c_str()) > TPL_0maxwidth[t]){
    disp.setFont(fontno[TPL_0font2[t]]);
  }
  //disp.setCursor((disp.getDisplayWidth() / 2) - (disp.getUTF8Width(ZZA2_Station.c_str()) / 2), TPL_3posy[t]);
  //disp.drawUTF8(TPL_0posx[t], TPL_0posy[t], ZZA2_Station.c_str());
  disp.drawUTF8((disp.getDisplayWidth() / 2) - (disp.getUTF8Width(ZZA1_Station.c_str()) / 2) + TPL_0posx[t], TPL_0posy[t], ZZA1_Station.c_str());

  // Track
  disp.setFont(fontno[TPL_1font[t]]);
  disp.setCursor(TPL_1posx[t],TPL_1posy[t]);
  disp.print(ZZA1_Track);

  // Destination
  disp.setFont(fontno[TPL_2font[t]]);
  // Switch to narrow font for longer destination names
  if (disp.getUTF8Width(ZZA1_Destination.c_str()) > TPL_2maxwidth[t]){
    disp.setFont(fontno[TPL_2font2[t]]);
  }
  disp.drawUTF8(TPL_2posx[t], TPL_2posy[t], ZZA1_Destination.c_str());

  // Departure
  disp.setFont(fontno[TPL_3font[t]]);
  //disp.setCursor(disp.getDisplayWidth() - disp.getUTF8Width(ZZA1_Departure.c_str()), TPL_3posy[t]);
  disp.setCursor(TPL_3posx[t], TPL_3posy[t]);
  disp.print(ZZA1_Departure);

  // Train
  disp.setFont(fontno[TPL_4font[t]]);
  //disp.setCursor(disp.getDisplayWidth() - disp.getUTF8Width(ZZA1_Train.c_str()), TPL_4posy[t]);
  disp.setCursor(TPL_4posx[t], TPL_4posy[t]);
  disp.print(ZZA1_Train);

  // Logo
  if ( ZZA1_Type != "" ) {
    switchLogo(t, ZZA1_Type);
  }
  
  disp.nextPage();

  offset1-=1;
  if ( (u8g2_uint_t)offset1 < (u8g2_uint_t)-width1 )
    offset1 = 0;
}


// Display a logo based on TrainType field
void switchLogo(uint8_t t, String ZZA_Type)
{
  if (ZZA_Type == logoId[0]){
    disp.drawXBM( TPL_5logox[t], TPL_5logoy[t], logow[0], logoh[0], logo0);
  }
  else if (ZZA_Type == logoId[1]){
    disp.drawXBM( TPL_5logox[t], TPL_5logoy[t], logow[1], logoh[1], logo1);
  }
  else if (ZZA_Type == logoId[2]){
    disp.drawXBM( TPL_5logox[t], TPL_5logoy[t], logow[2], logoh[2], logo2);
  }
  else if (ZZA_Type == logoId[3]){
    disp.drawXBM( TPL_5logox[t], TPL_5logoy[t], logow[3], logoh[3], logo3);
  }
  else if (ZZA_Type == logoId[4]){
    disp.drawXBM( TPL_5logox[t], TPL_5logoy[t], logow[4], logoh[4], logo4);
  }
  else if (ZZA_Type == logoId[5]){
    disp.drawXBM( TPL_5logox[t], TPL_5logoy[t], logow[5], logoh[5], logo5);
  }
  else if (ZZA_Type == logoId[6]){
    disp.drawXBM( TPL_5logox[t], TPL_5logoy[t], logow[6], logoh[6], logo6);
  }
  else if (ZZA_Type == logoId[7]){
    disp.drawXBM( TPL_5logox[t], TPL_5logoy[t], logow[7], logoh[7], logo7);
  }
  else if (ZZA_Type == logoId[8]){
    disp.drawXBM( TPL_5logox[t], TPL_5logoy[t], logow[8], logoh[8], logo8);
  }
  else if (ZZA_Type == logoId[9]){
    disp.drawXBM( TPL_5logox[t], TPL_5logoy[t], logow[9], logoh[9], logo9);
  }
}


// Toggle ScreenSaver for all displays
void screenSaver(int s)
{
  disp.sendBuffer();  // Send buffer to display
  disp.setPowerSave(s);
}


void updateTime() {
  time(&now);                         // read the current time
  localtime_r(&now, &tm);             // update the structure tm with the current time

  if (millis() - lastNTP < 20000){
    // Format time, add leading 0's
    ntptime = ("0" + String(tm.tm_hour)).substring( String(tm.tm_hour).length() - 1,  String(tm.tm_hour).length() +1) + ":" + ("0" + String(tm.tm_min)).substring( String(tm.tm_min).length() - 1,  String(tm.tm_min).length() +1);
    lastNTP = millis();
  }
  else {
    ntptime = "no NTP time";
  }
}


void runCmd(){
  ZZA1_Message = ZZA1_MessageO;
  ZZA1_Message.replace("{ntptime}", ntptime);
  ZZA1_Message.replace("{rrtime}", rrtime);
  ZZA1_MessageLoop = " +++ " + ZZA1_Message;
  width1 = disp.getUTF8Width(ZZA1_MessageLoop.c_str());
}


// This function is called once WIFI and MQTT are connected
void onConnectionEstablished()
{
  // Subscribe MQTT client to topic: "rocrail/service/info/clock" to receive Rocrail time
  //client.subscribe("rocrail/service/info/clock", [](const String & payload1) {
  client.subscribe(config.MQTT_TOPIC1, [](const String & payload1) {
    //Serial.println(payload1);
    // <clock divider="1" hour="18" minute="40" wday="5" mday="12" month="2" year="2021" time="1613151626" temp="20" bri="255" lux="0" pressure="0" humidity="0" cmd="sync"/>
    String h = payload1.substring(payload1.indexOf("hour") + 6, payload1.indexOf("minute") - 2);
    if (h.length() < 2){
      h = "0" + h;
    }
    String m = payload1.substring(payload1.indexOf("minute") + 8, payload1.indexOf("wday") - 2);
    if (m.length() < 2){
      m = "0" + m;
    }
    rrtime = h + ":" + m;
    runCmd();
  }, 1);

  // Subscribe MQTT client to topic: "rocrail/service/info/tx" to receive messages sent by Rocrail text fields
  client.subscribe(config.MQTT_TOPIC2, [](const String & payload2) {
    //Serial.println(payload2);
    String pld = payload2.substring(payload2.indexOf("ZZAMSG"), payload2.length() - 4);

    if (pld.substring(0, 6) == "ZZAMSG"){
      int start01 = pld.indexOf("ZZAMSG#") + 7;                         // ZZAMSG identifier
      int start02 = start01 + 1 + pld.substring(start01).indexOf("#");  // Target Displays as defined in config.h e.g. D01-D08
      int start03 = start02 + 1 + pld.substring(start02).indexOf("#");  // Template T0-T9
      int start04 = start03 + 1 + pld.substring(start03).indexOf("#");  // Station Name
      int start05 = start04 + 1 + pld.substring(start04).indexOf("#");  // Track Number
      int start06 = start05 + 1 + pld.substring(start05).indexOf("#");  // Destination Name
      int start07 = start06 + 1 + pld.substring(start06).indexOf("#");  // Departure Time
      int start08 = start07 + 1 + pld.substring(start07).indexOf("#");  // Train Number
      int start09 = start08 + 1 + pld.substring(start08).indexOf("#");  // Train Type e.g. ICE, IC, ...
      int start10 = start09 + 1 + pld.substring(start09).indexOf("#");  // Message Text

      // Deactivate / Reset ScreenSaver
      screenSaver(0);
      lastMsg = millis();

      // Display 1
      if (pld.substring(start01, start02 -1).indexOf(DPL_id[0]) != -1){
        ZZA1_Targets = pld.substring(start01, start02 -1);
        ZZA1_Template = pld.substring(start02 + 1, start03 -1);
        ZZA1_Station = pld.substring(start03, start04 -1);
        ZZA1_Track = pld.substring(start04, start05 -1);
        ZZA1_Destination = pld.substring(start05, start06 -1);
        ZZA1_Departure = pld.substring(start06, start07 -1);
        ZZA1_Train = pld.substring(start07, start08 -1);
        ZZA1_Type = pld.substring(start08, start09 -1);
        ZZA1_MessageO = pld.substring(start09, start10 -1);
        ZZA1_Message = ZZA1_MessageO;
        if (pld.indexOf("{") > 0){
          ZZA1_Message.replace("{ntptime}", ntptime);
          ZZA1_Message.replace("{rrtime}", rrtime);
        }
        ZZA1_MessageLoop = " +++ " + ZZA1_Message;
        width1 = disp.getUTF8Width(ZZA1_MessageLoop.c_str());
      }
    }
    else {
      Serial.println(F("Error - No valid ZZAMSG Message"));
      Serial.println(pld);
    }
  }, 1);
}


// Main loop
void loop()
{
  ArduinoOTA.handle();       // OTA

  webserver.handleClient();  // WEBSERVER handling of incoming requests

  client.loop();             // WIFI, MQTT

  updateTime();              // NTP update time information

  if ( millis() > (lastMsg + (config.SCREENSAVER * 60000)) && config.SCREENSAVER > 0 ){
    screenSaver(1);          // Activate ScreenSaver
  }

  send2display1();
}
