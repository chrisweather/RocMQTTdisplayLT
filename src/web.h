// Roc-MQTT-Display-LT WEBSERVER
// Version 1.01
// Copyright (c) 2020-2022 Christian Heinrichs. All rights reserved.
// https://github.com/chrisweather/RocMQTTdisplay

#ifndef WEB_H
#define WEB_H
#include "ESP8266WebServer.h"  // https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer
#include <LittleFS.h>          // LittleFS file system https://github.com/esp8266/Arduino/tree/master/libraries/LittleFS
#include "config.h"            // Roc-MQTT-Display-LT configuration file

// Define WEBSERVER
ESP8266WebServer webserver(80);
String buf = "";

// ROOT
void loadRoot()
{
  File htmlRoot = LittleFS.open( "/index.htm", "r" );
  buf = htmlRoot.readString();
  buf.replace("%VER%", config.VER);
  buf.replace("%WIFI_DEVICENAME%", String(config.WIFI_DEVICENAME));
  webserver.send( 200, "text/html", buf );
  htmlRoot.close();
  buf = "";
}

// CSS
void loadCSS()
{
  File htmlCSS = LittleFS.open( "/rmd.css", "r" );
  webserver.streamFile( htmlCSS, "text/css" );
  htmlCSS.close();
}

// 404 - NotFound
void loadNotFound()
{
  File html404 = LittleFS.open( "/404.htm", "r" );
  webserver.streamFile( html404, "text/html" );
  html404.close();
}

// CONFIGURATION
void loadCfg()
{
  File htmlCfg = LittleFS.open( "/config.htm", "r" );
  buf = htmlCfg.readString();
  buf.replace("%WIFI_DEVICENAME%", String(config.WIFI_DEVICENAME));
  buf.replace("%WIFI_RECONDELAY%", String(config.WIFI_RECONDELAY));
  buf.replace("%OTA_HOSTNAME%", String(config.OTA_HOSTNAME));
  buf.replace("%OTA_PORT%", String(config.OTA_PORT));
  buf.replace("%NTP_SERVER%", String(config.NTP_SERVER));
  buf.replace("%NTP_TZ%", String(config.NTP_TZ));
  buf.replace("%MQTT_IP%", String(config.MQTT_IP));
  buf.replace("%MQTT_PORT%", String(config.MQTT_PORT));
  buf.replace("%MQTT_MSGSIZE%", String(config.MQTT_MSGSIZE));
  buf.replace("%MQTT_KEEPALIVE%", String(config.MQTT_KEEPALIVE1));
  buf.replace("%MQTT_RECONDELAY%", String(config.MQTT_RECONDELAY));
  buf.replace("%MQTT_TOPIC1%", String(config.MQTT_TOPIC1));
  buf.replace("%MQTT_TOPIC2%", String(config.MQTT_TOPIC2));
  buf.replace("%MQTT_DEBUG%", String(config.MQTT_DEBUG));
  buf.replace("%DISPSIZE%", String(config.DISPWIDTH) + " x " + String(config.DISPHEIGHT));
  buf.replace("%STARTDELAY%", String(config.STARTDELAY));
  buf.replace("%UPDSPEED%", String(config.UPDSPEED));
  buf.replace("%SCREENSAVER%", String(config.SCREENSAVER));
  buf.replace("%DPL_ID0%", String(DPL_id[0]));
  buf.replace("%DPL_FLIP0%", String(DPL_flip[0]));
  buf.replace("%DPL_CONTRAST0%", String(DPL_contrast[0]));
  buf.replace("%DPL_SIDE0%", String(DPL_side[0]));
  webserver.send( 200, "text/html", buf );
  htmlCfg.close();
  buf = "";
}

void handleCfgSubmit()
{
  if (webserver.args() > 0 ) {
    for ( uint8_t i = 0; i < webserver.args(); i++ ) {
      if (webserver.argName(i) == "f_WIFI_DEVICENAME") { webserver.arg(webserver.argName(i)).toCharArray(config.WIFI_DEVICENAME, sizeof(config.WIFI_DEVICENAME)); }
      if (webserver.argName(i) == "f_WIFI_RECONDELAY") { config.WIFI_RECONDELAY = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_OTA_HOSTNAME") { webserver.arg(webserver.argName(i)).toCharArray(config.OTA_HOSTNAME, sizeof(config.OTA_HOSTNAME)); }
      if (webserver.argName(i) == "f_OTA_PORT") { config.OTA_PORT = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_NTP_SERVER") { webserver.arg(webserver.argName(i)).toCharArray(config.NTP_SERVER, sizeof(config.NTP_SERVER)); }
      if (webserver.argName(i) == "f_NTP_TZ") { webserver.arg(webserver.argName(i)).toCharArray(config.NTP_TZ, sizeof(config.NTP_TZ)); }
      if (webserver.argName(i) == "f_MQTT_IP") { webserver.arg(webserver.argName(i)).toCharArray(config.MQTT_IP, sizeof(config.MQTT_IP)); }
      if (webserver.argName(i) == "f_MQTT_PORT") { config.MQTT_PORT = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_MQTT_MSGSIZE") { config.MQTT_MSGSIZE = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_MQTT_KEEPALIVE") { config.MQTT_KEEPALIVE1 = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_MQTT_RECONDELAY") { config.MQTT_RECONDELAY = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_MQTT_TOPIC1") { webserver.arg(webserver.argName(i)).toCharArray(config.MQTT_TOPIC1, sizeof(config.MQTT_TOPIC1)); }
      if (webserver.argName(i) == "f_MQTT_TOPIC2") { webserver.arg(webserver.argName(i)).toCharArray(config.MQTT_TOPIC2, sizeof(config.MQTT_TOPIC2)); }
      if (webserver.argName(i) == "f_MQTT_DEBUG") { config.MQTT_DEBUG = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_STARTDELAY") { config.STARTDELAY = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_UPDSPEED") { config.UPDSPEED = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_SCREENSAVER") { config.SCREENSAVER = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_DPL_ID0") { webserver.arg(webserver.argName(i)).toCharArray(DPL_id[0], sizeof(DPL_id[0])); }
      if (webserver.argName(i) == "f_DPL_FLIP0") { DPL_flip[0] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_DPL_CONTRAST0") { DPL_contrast[0] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_DPL_SIDE0") { DPL_side[0] = webserver.arg(webserver.argName(i)).toInt(); }
    }
    saveConfiguration(configfile, config);
    delay(2000);
  }
}


// TEMPLATE 1 - Fonts, Logos
void loadTpl1()
{
  File htmlTpl = LittleFS.open( "/tpl1.htm", "r" );
  buf = htmlTpl.readString();
  buf.replace("%WIFI_DEVICENAME%", String(config.WIFI_DEVICENAME));

  int i = 0;
  //String fbuf = "";
  //fbuf = String(&*fontno[i]);
  buf.replace("%TPL_FONT0%", String(fonts[0]));
  buf.replace("%TPL_FONT1%", String(fonts[1]));
  buf.replace("%TPL_FONT2%", String(fonts[2]));
  buf.replace("%TPL_FONT3%", String(fonts[3]));
  buf.replace("%TPL_FONT4%", String(fonts[4]));
  buf.replace("%TPL_FONT5%", String(fonts[5]));
  buf.replace("%TPL_FONT6%", String(fonts[6]));
  buf.replace("%TPL_FONT7%", String(fonts[7]));
  buf.replace("%TPL_FONT8%", String(fonts[8]));
  buf.replace("%TPL_FONT9%", String(fonts[9]));

  buf.replace("%TPL_LOGOID0%", String(logoId[0]));
  buf.replace("%TPL_LOGOID1%", String(logoId[1]));
  buf.replace("%TPL_LOGOID2%", String(logoId[2]));
  buf.replace("%TPL_LOGOID3%", String(logoId[3]));
  buf.replace("%TPL_LOGOID4%", String(logoId[4]));
  buf.replace("%TPL_LOGOID5%", String(logoId[5]));
  buf.replace("%TPL_LOGOID6%", String(logoId[6]));
  buf.replace("%TPL_LOGOID7%", String(logoId[7]));
  buf.replace("%TPL_LOGOID8%", String(logoId[8]));
  buf.replace("%TPL_LOGOID9%", String(logoId[9]));

  buf.replace("%TPL_LOGO0W%", String(logow[0]));
  buf.replace("%TPL_LOGO1W%", String(logow[1]));
  buf.replace("%TPL_LOGO2W%", String(logow[2]));
  buf.replace("%TPL_LOGO3W%", String(logow[3]));
  buf.replace("%TPL_LOGO4W%", String(logow[4]));
  buf.replace("%TPL_LOGO5W%", String(logow[5]));
  buf.replace("%TPL_LOGO6W%", String(logow[6]));
  buf.replace("%TPL_LOGO7W%", String(logow[7]));
  buf.replace("%TPL_LOGO8W%", String(logow[8]));
  buf.replace("%TPL_LOGO9W%", String(logow[9]));

  buf.replace("%TPL_LOGO0H%", String(logoh[0]));
  buf.replace("%TPL_LOGO1H%", String(logoh[1]));
  buf.replace("%TPL_LOGO2H%", String(logoh[2]));
  buf.replace("%TPL_LOGO3H%", String(logoh[3]));
  buf.replace("%TPL_LOGO4H%", String(logoh[4]));
  buf.replace("%TPL_LOGO5H%", String(logoh[5]));
  buf.replace("%TPL_LOGO6H%", String(logoh[6]));
  buf.replace("%TPL_LOGO7H%", String(logoh[7]));
  buf.replace("%TPL_LOGO8H%", String(logoh[8]));
  buf.replace("%TPL_LOGO9H%", String(logoh[9]));

  String lbuf = "";
  
  for (i = 0; i < (logo0size); i++){
    if (uint8_t(logo0[i]) < 16){
      lbuf += String("0x0");
    }
    else {
      lbuf += String("0x");
    }
    lbuf += String(logo0[i], HEX);
    if (i < (logo0size - 1)) lbuf += String(", ");
  }
  buf.replace("%TPL_LOGO0%", String(lbuf));

  lbuf = "";
  for (i = 0; i < (logo1size); i++){
    if (uint8_t(logo1[i]) < 16){
      lbuf += String("0x0");
    }
    else {
      lbuf += String("0x");
    }
    lbuf += String(logo1[i], HEX);
    if (i < (logo1size - 1)) lbuf += String(", ");
  }
  buf.replace("%TPL_LOGO1%", String(lbuf));

  lbuf = "";
  for (i = 0; i < (logo2size); i++){
    if (uint8_t(logo2[i]) < 16){
      lbuf += String("0x0");
    }
    else {
      lbuf += String("0x");
    }
    lbuf += String(logo2[i], HEX);
    if (i < (logo2size - 1)) lbuf += String(", ");
  }
  buf.replace("%TPL_LOGO2%", String(lbuf));

  lbuf = "";
  for (i = 0; i < (logo3size); i++){
    if (uint8_t(logo3[i]) < 16){
      lbuf += String("0x0");
    }
    else {
      lbuf += String("0x");
    }
    lbuf += String(logo3[i], HEX);
    if (i < (logo3size - 1)) lbuf += String(", ");
  }
  buf.replace("%TPL_LOGO3%", String(lbuf));

  lbuf = "";
  for (i = 0; i < (logo4size); i++){
    if (uint8_t(logo4[i]) < 16){
      lbuf += String("0x0");
    }
    else {
      lbuf += String("0x");
    }
    lbuf += String(logo4[i], HEX);
    if (i < (logo4size - 1)) lbuf += String(", ");
  }
  buf.replace("%TPL_LOGO4%", String(lbuf));

  lbuf = "";
  for (i = 0; i < (logo5size); i++){
    if (uint8_t(logo5[i]) < 16){
      lbuf += String("0x0");
    }
    else {
      lbuf += String("0x");
    }
    lbuf += String(logo5[i], HEX);
    if (i < (logo5size - 1)) lbuf += String(", ");
  }
  buf.replace("%TPL_LOGO5%", String(lbuf));

  lbuf = "";
  for (i = 0; i < (logo6size); i++){
    if (uint8_t(logo6[i]) < 16){
      lbuf += String("0x0");
    }
    else {
      lbuf += String("0x");
    }
    lbuf += String(logo6[i], HEX);
    if (i < (logo6size - 1)) lbuf += String(", ");
  }
  buf.replace("%TPL_LOGO6%", String(lbuf));

  lbuf = "";
  for (i = 0; i < (logo7size); i++){
    if (uint8_t(logo7[i]) < 16){
      lbuf += String("0x0");
    }
    else {
      lbuf += String("0x");
    }
    lbuf += String(logo7[i], HEX);
    if (i < (logo7size - 1)) lbuf += String(", ");
  }
  buf.replace("%TPL_LOGO7%", String(lbuf));

  lbuf = "";
  for (i = 0; i < (logo8size); i++){
    if (uint8_t(logo8[i]) < 16){
      lbuf += String("0x0");
    }
    else {
      lbuf += String("0x");
    }
    lbuf += String(logo8[i], HEX);
    if (i < (logo8size - 1)) lbuf += String(", ");
  }
  buf.replace("%TPL_LOGO8%", String(lbuf));

  lbuf = "";
  for (i = 0; i < (logo9size); i++){
    if (uint8_t(logo9[i]) < 16){
      lbuf += String("0x0");
    }
    else {
      lbuf += String("0x");
    }
    lbuf += String(logo9[i], HEX);
    if (i < (logo9size - 1)) lbuf += String(", ");
  }
  buf.replace("%TPL_LOGO9%", String(lbuf));

  webserver.send( 200, "text/html", buf );
  htmlTpl.close();
  buf = "";
}

void handleTpl1Submit()
{
  if (webserver.args() > 0 ) {
    for ( uint8_t i = 0; i < webserver.args(); i++ ) {
      if (webserver.argName(i) == "f_TPL_LOGOID0") { webserver.arg(webserver.argName(i)).toCharArray(logoId[0], sizeof(logoId[0])); }
      if (webserver.argName(i) == "f_TPL_LOGOID1") { webserver.arg(webserver.argName(i)).toCharArray(logoId[1], sizeof(logoId[1])); }
      if (webserver.argName(i) == "f_TPL_LOGOID2") { webserver.arg(webserver.argName(i)).toCharArray(logoId[2], sizeof(logoId[2])); }
      if (webserver.argName(i) == "f_TPL_LOGOID3") { webserver.arg(webserver.argName(i)).toCharArray(logoId[3], sizeof(logoId[3])); }
      if (webserver.argName(i) == "f_TPL_LOGOID4") { webserver.arg(webserver.argName(i)).toCharArray(logoId[4], sizeof(logoId[4])); }
      if (webserver.argName(i) == "f_TPL_LOGOID5") { webserver.arg(webserver.argName(i)).toCharArray(logoId[5], sizeof(logoId[5])); }
      if (webserver.argName(i) == "f_TPL_LOGOID6") { webserver.arg(webserver.argName(i)).toCharArray(logoId[6], sizeof(logoId[6])); }
      if (webserver.argName(i) == "f_TPL_LOGOID7") { webserver.arg(webserver.argName(i)).toCharArray(logoId[7], sizeof(logoId[7])); }
      if (webserver.argName(i) == "f_TPL_LOGOID8") { webserver.arg(webserver.argName(i)).toCharArray(logoId[8], sizeof(logoId[8])); }
      if (webserver.argName(i) == "f_TPL_LOGOID9") { webserver.arg(webserver.argName(i)).toCharArray(logoId[9], sizeof(logoId[9])); }
      
      if (webserver.argName(i) == "f_TPL_LOGO0W") { logow[0] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_LOGO1W") { logow[1] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_LOGO2W") { logow[2] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_LOGO3W") { logow[3] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_LOGO4W") { logow[4] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_LOGO5W") { logow[5] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_LOGO6W") { logow[6] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_LOGO7W") { logow[7] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_LOGO8W") { logow[8] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_LOGO9W") { logow[9] = webserver.arg(webserver.argName(i)).toInt(); }
      
      if (webserver.argName(i) == "f_TPL_LOGO0H") { logoh[0] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_LOGO1H") { logoh[1] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_LOGO2H") { logoh[2] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_LOGO3H") { logoh[3] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_LOGO4H") { logoh[4] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_LOGO5H") { logoh[5] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_LOGO6H") { logoh[6] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_LOGO7H") { logoh[7] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_LOGO8H") { logoh[8] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_LOGO9H") { logoh[9] = webserver.arg(webserver.argName(i)).toInt(); }

    }
    saveTemplate(templatefile, templ);
    delay(2000);
  }
}

// TEMPLATE data - Settings of template 0 - 9
void loadTpl2()
{
  File htmlTpl = LittleFS.open( "/tpl2.htm", "r" );
  buf = htmlTpl.readString();
  buf.replace("%WIFI_DEVICENAME%", String(config.WIFI_DEVICENAME));

  buf.replace("%TPL_ID0%", String(TPL_id[TPL]));
  buf.replace("%TPL_NAME0%", String(TPL_name[TPL]));
  buf.replace("%TPL_SIDE0%", String(TPL_side[TPL]));
  
  buf.replace("%TPL_0FONT0%", String(TPL_0font[TPL]));
  buf.replace("%TPL_0MAXWIDTH0%", String(TPL_0maxwidth[TPL]));
  buf.replace("%TPL_0FONT20%", String(TPL_0font2[TPL]));
  buf.replace("%TPL_0DRAWCOLOR0%", String(TPL_0drawcolor[TPL]));
  buf.replace("%TPL_0FONTMODE0%", String(TPL_0fontmode[TPL]));
  buf.replace("%TPL_0POSX0%", String(TPL_0posx[TPL]));
  buf.replace("%TPL_0POSY0%", String(TPL_0posy[TPL]));
  buf.replace("%TPL_0SCROLL0%", String(TPL_0scroll[TPL]));

  buf.replace("%TPL_1FONT0%", String(TPL_1font[TPL]));
  buf.replace("%TPL_1DRAWCOLOR0%", String(TPL_1drawcolor[TPL]));
  buf.replace("%TPL_1FONTMODE0%", String(TPL_1fontmode[TPL]));
  buf.replace("%TPL_1POSX0%", String(TPL_1posx[TPL]));
  buf.replace("%TPL_1POSY0%", String(TPL_1posy[TPL]));

  buf.replace("%TPL_2FONT0%", String(TPL_2font[TPL]));
  buf.replace("%TPL_2MAXWIDTH0%", String(TPL_2maxwidth[TPL]));
  buf.replace("%TPL_2FONT20%", String(TPL_2font2[TPL]));
  buf.replace("%TPL_2DRAWCOLOR0%", String(TPL_2drawcolor[TPL]));
  buf.replace("%TPL_2FONTMODE0%", String(TPL_2fontmode[TPL]));
  buf.replace("%TPL_2POSX0%", String(TPL_2posx[TPL]));
  buf.replace("%TPL_2POSY0%", String(TPL_2posy[TPL]));
  buf.replace("%TPL_2SCROLL0%", String(TPL_2scroll[TPL]));

  buf.replace("%TPL_3FONT0%", String(TPL_3font[TPL]));
  buf.replace("%TPL_3DRAWCOLOR0%", String(TPL_3drawcolor[TPL]));
  buf.replace("%TPL_3FONTMODE0%", String(TPL_3fontmode[TPL]));
  buf.replace("%TPL_3POSX0%", String(TPL_3posx[TPL]));
  buf.replace("%TPL_3POSY0%", String(TPL_3posy[TPL]));

  buf.replace("%TPL_4FONT0%", String(TPL_4font[TPL]));
  buf.replace("%TPL_4DRAWCOLOR0%", String(TPL_4drawcolor[TPL]));
  buf.replace("%TPL_4FONTMODE0%", String(TPL_4fontmode[TPL]));
  buf.replace("%TPL_4POSX0%", String(TPL_4posx[TPL]));
  buf.replace("%TPL_4POSY0%", String(TPL_4posy[TPL]));

  buf.replace("%TPL_5LOGOX0%", String(TPL_5logox[TPL]));
  buf.replace("%TPL_5LOGOY0%", String(TPL_5logoy[TPL]));

  buf.replace("%TPL_6FONT0%", String(TPL_6font[TPL]));
  buf.replace("%TPL_6MAXWIDTH0%", String(TPL_6maxwidth[TPL]));
  buf.replace("%TPL_6FONT20%", String(TPL_6font2[TPL]));
  buf.replace("%TPL_6DRAWCOLOR0%", String(TPL_6drawcolor[TPL]));
  buf.replace("%TPL_6FONTMODE0%", String(TPL_6fontmode[TPL]));
  buf.replace("%TPL_6POSX0%", String(TPL_6posx[TPL]));
  buf.replace("%TPL_6POSY0%", String(TPL_6posy[TPL]));
  buf.replace("%TPL_6SCROLL0%", String(TPL_6scroll[TPL]));

  buf.replace("%TPL_6BOXX0%", String(TPL_6boxx[TPL]));
  buf.replace("%TPL_6BOXY0%", String(TPL_6boxy[TPL]));
  buf.replace("%TPL_6BOXW0%", String(TPL_6boxw[TPL]));
  buf.replace("%TPL_6BOXH0%", String(TPL_6boxh[TPL]));

  buf.replace("%TPL_6DRAWCOLOR20%", String(TPL_6drawcolor2[TPL]));
  buf.replace("%TPL_6FONTMODE20%", String(TPL_6fontmode2[TPL]));
  buf.replace("%TPL_6BOX2X0%", String(TPL_6box2x[TPL]));
  buf.replace("%TPL_6BOX2Y0%", String(TPL_6box2y[TPL]));
  buf.replace("%TPL_6BOX2W0%", String(TPL_6box2w[TPL]));
  buf.replace("%TPL_6BOX2H0%", String(TPL_6box2h[TPL]));
  
  webserver.send( 200, "text/html", buf );
  htmlTpl.close();
  buf = "";
}

void handleTpl2Select()
{
  if (webserver.args() > 0 ) {
    for ( uint8_t i = 0; i < webserver.args(); i++ ) {
      if (webserver.argName(i) == "f_TPL_NO") { TPL = webserver.arg(webserver.argName(i)).toInt(); }
    }
    loadTpl2();
  }
}

void handleTpl2Submit()
{
  if (webserver.args() > 0 ) {
    for ( uint8_t i = 0; i < webserver.args(); i++ ) {
      if (webserver.argName(i) == "f_TPL_NAME0") { webserver.arg(webserver.argName(i)).toCharArray(TPL_name[TPL], sizeof(TPL_name[TPL])); }
      if (webserver.argName(i) == "f_TPL_SIDE0") { TPL_side[TPL] = webserver.arg(webserver.argName(i)).toInt(); }

      if (webserver.argName(i) == "f_TPL_0FONT0") { TPL_0font[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_0MAXWIDTH0") { TPL_0maxwidth[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_0FONT20") { TPL_0font2[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_0DRAWCOLOR0") { TPL_0drawcolor[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_0FONTMODE0") { TPL_0fontmode[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_0POSX0") { TPL_0posx[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_0POSY0") { TPL_0posy[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_0SCROLL0") { TPL_0scroll[TPL] = webserver.arg(webserver.argName(i)).toInt(); }

      if (webserver.argName(i) == "f_TPL_1FONT0") { TPL_1font[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_1DRAWCOLOR0") { TPL_1drawcolor[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_1FONTMODE0") { TPL_1fontmode[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_1POSX0") { TPL_1posx[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_1POSY0") { TPL_1posy[TPL] = webserver.arg(webserver.argName(i)).toInt(); }

      if (webserver.argName(i) == "f_TPL_2FONT0") { TPL_2font[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_2MAXWIDTH0") { TPL_2maxwidth[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_2FONT20") { TPL_2font2[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_2DRAWCOLOR0") { TPL_2drawcolor[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_2FONTMODE0") { TPL_2fontmode[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_2POSX0") { TPL_2posx[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_2POSY0") { TPL_2posy[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_2SCROLL0") { TPL_2scroll[TPL] = webserver.arg(webserver.argName(i)).toInt(); }

      if (webserver.argName(i) == "f_TPL_3FONT0") { TPL_3font[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_3DRAWCOLOR0") { TPL_3drawcolor[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_3FONTMODE0") { TPL_3fontmode[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_3POSX0") { TPL_3posx[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_3POSY0") { TPL_3posy[TPL] = webserver.arg(webserver.argName(i)).toInt(); }

      if (webserver.argName(i) == "f_TPL_4FONT0") { TPL_4font[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_4DRAWCOLOR0") { TPL_4drawcolor[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_4FONTMODE0") { TPL_4fontmode[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_4POSX0") { TPL_4posx[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_4POSY0") { TPL_4posy[TPL] = webserver.arg(webserver.argName(i)).toInt(); }

      if (webserver.argName(i) == "f_TPL_5LOGOX0") { TPL_5logox[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_5LOGOY0") { TPL_5logoy[TPL] = webserver.arg(webserver.argName(i)).toInt(); }

      if (webserver.argName(i) == "f_TPL_6FONT0") { TPL_6font[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_6MAXWIDTH0") { TPL_6maxwidth[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_6FONT20") { TPL_6font2[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_6DRAWCOLOR0") { TPL_6drawcolor[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_6FONTMODE0") { TPL_6fontmode[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_6POSX0") { TPL_6posx[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_6POSY0") { TPL_6posy[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_6SCROLL0") { TPL_6scroll[TPL] = webserver.arg(webserver.argName(i)).toInt(); }

      if (webserver.argName(i) == "f_TPL_6BOXX0") { TPL_6boxx[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_6BOXY0") { TPL_6boxy[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_6BOXW0") { TPL_6boxw[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_6BOXH0") { TPL_6boxh[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
  
      if (webserver.argName(i) == "f_TPL_6DRAWCOLOR20") { TPL_6drawcolor2[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_6FONTMODE20") { TPL_6fontmode2[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_6BOX2X0") { TPL_6box2x[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_6BOX2Y0") { TPL_6box2y[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_6BOX2W0") { TPL_6box2w[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
      if (webserver.argName(i) == "f_TPL_6BOX2H0") { TPL_6box2h[TPL] = webserver.arg(webserver.argName(i)).toInt(); }
    }
    switch (TPL) {
    case 0: saveTemplateFile(template00);
            break;
    case 1: saveTemplateFile(template01);
            break;
    case 2: saveTemplateFile(template02);
            break;
    case 3: saveTemplateFile(template03);
            break;
    case 4: saveTemplateFile(template04);
            break;
    case 5: saveTemplateFile(template05);
            break;
    case 6: saveTemplateFile(template06);
            break;
    case 7: saveTemplateFile(template07);
            break;
    case 8: saveTemplateFile(template08);
            break;
    case 9: saveTemplateFile(template09);
    }
    delay(2000);
  }
}

void downloadFile(){
  if (!LittleFS.begin()) {
    Serial.println("LittleFS failed to mount !\r\n");                   
  }
  else {
    String str = "";
    File f = LittleFS.open(webserver.arg(0), "r");
    if (!f) {
      Serial.println("Can't open LittleFS file !\r\n");         
    }
    else {
      char buf[1024];
      int siz = f.size();
      while(siz > 0) {
        size_t len = std::min((int)(sizeof(buf) - 1), siz);
        f.read((uint8_t *)buf, len);
        buf[len] = 0;
        str += buf;
        siz -= sizeof(buf) - 1;
      }
      f.close();
      //webserver.send(200, "text/plain", webserver.arg(0));
      webserver.send(200, "text/plain", str);
      //webserver.send(200, "application/json", str);
    }
  }
}


// SEC
void loadSec()
{
  File htmlSec = LittleFS.open( "/sec.htm", "r" );
  buf = htmlSec.readString();
  buf.replace("%WIFI_DEVICENAME%", String(config.WIFI_DEVICENAME));
  buf.replace("%WIFI_SSID%", String(sec.WIFI_SSID));
  buf.replace("%MQTT_USER%", String(sec.MQTT_USER));

  webserver.send( 200, "text/html", buf );
  htmlSec.close();
  buf = "";
}

void handleSecSubmit()
{
  if (webserver.args() > 0 ) {
    for ( uint8_t i = 0; i < webserver.args(); i++ ) {
      if (webserver.argName(i) == "f_WIFI_SSID") { webserver.arg(webserver.argName(i)).toCharArray(sec.WIFI_SSID, sizeof(sec.WIFI_SSID)); }
      if (webserver.argName(i) == "f_WIFI_PW") { webserver.arg(webserver.argName(i)).toCharArray(sec.WIFI_PW, sizeof(sec.WIFI_PW)); }
      if (webserver.argName(i) == "f_OTA_PW") { webserver.arg(webserver.argName(i)).toCharArray(sec.OTA_PW, sizeof(sec.OTA_PW)); }
      if (webserver.argName(i) == "f_OTA_HASH") { webserver.arg(webserver.argName(i)).toCharArray(sec.OTA_HASH, sizeof(sec.OTA_HASH)); }
      if (webserver.argName(i) == "f_MQTT_USER") { webserver.arg(webserver.argName(i)).toCharArray(sec.MQTT_USER, sizeof(sec.MQTT_USER)); }
      if (webserver.argName(i) == "f_MQTT_PW") { webserver.arg(webserver.argName(i)).toCharArray(sec.MQTT_PW, sizeof(sec.MQTT_PW)); }
    }
    saveSec(secfile, sec);
    delay(2000);
  }
}

#endif
