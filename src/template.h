// Roc-MQTT-Display-LT TEMPLATES
// Version 1.01
// Copyright (c) 2020-2022 Christian Heinrichs. All rights reserved.
// https://github.com/chrisweather/RocMQTTdisplay

#ifndef TEMPLATE_H
#define TEMPLATE_H
#include <U8g2lib.h>    // U8g2lib by Oliver Kraus https://github.com/olikraus/u8g2

// TEMPLATES Part 2
// TEMPLATES T0 - T9
char TPL_id[10][3] =    { "T0","T1","T2","T3","T4","T5","T6","T7","T8","T9" }; // do not change TPL_id here!
char TPL_name[10][30] = { "Train Announcement - Side A", "Train Announcement - Side B", "Local Train - Side A", "Local Train - Side B", "Message Only, 1 Line - Side A", "Message Only, 1 Line - Side B", "Stationname, 1 Line - Side A", "Template 7", "Template 8", "Template 9" };

// TEMPLATE SETTINGS       T0,  T1,  T2,  T3,  T4,  T5,  T6,  T7,  T8,  T9
int TPL_side[] =        {   0,   1,   0,   1,   0,   1,   0,   0,   0,   0 };  // 0,1    0=Side A, 1= Side B

// Station (Field 0)       T0,  T1,  T2,  T3,  T4,  T5,  T6,  T7,  T8,  T9
int TPL_0font[] =       {   2,   2,   2,   1,   1,   1,   1,   1,   1,   1 };  // 0-9    font 1
int TPL_0maxwidth[] =   { 116, 116,   0,   0,   0,   0, 127,   0,   0,   0 };  // 0-127  max. text width before font2 is used
int TPL_0font2[] =      {   3,   3,   1,   1,   1,   1,   2,   1,   1,   1 };  // 0-9    font 2
int TPL_0drawcolor[] =  {   1,   1,   1,   1,   1,   1,   1,   1,   1,   1 };  // 0,1,2  Foreground color 0=black, 1=white
int TPL_0fontmode[] =   {   1,   1,   1,   1,   1,   1,   1,   1,   1,   1 };  // 0,1    Background 0=solid, 1=transparent
int TPL_0posx[] =       {   0,   0,   0,   0,   3,   0,   0,   0,   0,   0 };  // x position change from center +-
int TPL_0posy[] =       {   0,   0,  -5,  -5,  -5,  -5,  20,   0,   0,   0 };  // 0-31   y position
int TPL_0scroll[] =     {   0,   0,   0,   0,   1,   0,   0,   0,   0,   0 };  // 0,1    scoll text  0=no, 1=yes

// Track (Field 1)         T0,  T1,  T2,  T3,  T4,  T5,  T6,  T7,  T8,  T9
int TPL_1font[] =       {   1,   1,   1,   1,   1,   1,   1,   1,   1,   1 };  // 0-9
int TPL_1drawcolor[] =  {   1,   1,   1,   1,   1,   1,   1,   1,   1,   1 };  // 0,1,2
int TPL_1fontmode[] =   {   1,   1,   1,   1,   1,   1,   1,   1,   1,   1 };  // 0,1
int TPL_1posx[] =       {   0, 118,   0, 118,   0, 119,   0,   0,   0,   0 };  // 0-127  x left bottom corner of font
int TPL_1posy[] =       {  30,  30,  30,  30,  28,  28,   0,   0,   0,   0 };  // 0-31   y left bottom corner of font

// Destination (Field 2)   T0,  T1,  T2,  T3,  T4,  T5,  T6,  T7,  T8,  T9
int TPL_2font[] =       {   2,   2,   2,   2,   1,   1,   1,   1,   1,   1 };  // 0-9
int TPL_2maxwidth[] =   { 116, 116, 112, 112,   0,   0,   0,   0,   0,   0 };  // 0-127  max text width before font2 is used
int TPL_2font2[] =      {   3,   3,   3,   3,   1,   1,   1,   1,   1,   1 };  // 0-9    for longer Destination names
int TPL_2drawcolor[] =  {   1,   1,   1,   1,   1,   1,   1,   1,   1,   1 };  // 0,1,2
int TPL_2fontmode[] =   {   1,   1,   1,   1,   1,   1,   1,   1,   1,   1 };  // 0,1
int TPL_2posx[] =       {  20,   0,  20,   0,   0,   0,   0,   0,   0,   0 };  // 0-127
int TPL_2posy[] =       {  30,  30,  29,  29,   0,   0,   0,   0,   0,   0 };  // 0-31
int TPL_2scroll[] =     {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 };  // 0,1

// Departure (Field 3)     T0,  T1,  T2,  T3,  T4,  T5,  T6,  T7,  T8,  T9
int TPL_3font[] =       {   4,   4,   4,   4,   1,   1,   1,   1,   1,   1 };  // 0-9
int TPL_3drawcolor[] =  {   1,   1,   1,   1,   1,   1,   1,   1,   1,   1 };  // 0,1,2
int TPL_3fontmode[] =   {   1,   1,   1,   1,   1,   1,   1,   1,   1,   1 };  // 0,1
int TPL_3posx[] =       { 100,   0, 100,   0,   0,   0,   0,   0,   0,   0 };  // 0-127
int TPL_3posy[] =       {   8,   8,   8,   8,   0,   0,   0,   0,   0,   0 };  // 0-31

// Train (Field 4)         T0,  T1,  T2,  T3,  T4,  T5,  T6,  T7,  T8,  T9
int TPL_4font[] =       {   5,   5,   4,   4,   1,   1,   1,   1,   1,   1 };  // 0-9
int TPL_4drawcolor[] =  {   1,   1,   1,   1,   1,   1,   1,   1,   1,   1 };  // 0,1,2
int TPL_4fontmode[] =   {   1,   1,   1,   1,   1,   1,   1,   1,   1,   1 };  // 0,1
int TPL_4posx[] =       {  93,   0,  80,  32,   0,   0,   0,   0,   0,   0 };  // 0-127
int TPL_4posy[] =       {  17,  17,   8,   8,   0,   0,   0,   0,   0,   0 };  // 0-31

// TrainType (Field 5)     T0,  T1,  T2,  T3,  T4,  T5,  T6,  T7,  T8,  T9
int TPL_5logox[] =      {   0, 110,   0, 112,   0, 110,   0,   0,   0,   0 };  // 0-127  x left top corner of logo bitmap
int TPL_5logoy[] =      {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 };  // 0-31   y left top corner of logo bitmap

// Message (Field 6)       T0,  T1,  T2,  T3,  T4,  T5,  T6,  T7,  T8,  T9
//  Text
int TPL_6font[] =       {   6,   6,   1,   1,   2,   2,   1,   1,   1,   1 };  // 0-9
int TPL_6maxwidth[] =   { 116, 116, 116, 116, 117, 117,   0,   0,   0,   0 };  // 0-127  max text width before font2 is used
int TPL_6font2[] =      {   1,   1,   1,   1,   3,   3,   1,   1,   1,   1 };  // 0-9
int TPL_6drawcolor[] =  {   1,   1,   1,   1,   1,   1,   1,   1,   1,   1 };  // 0,1,2
int TPL_6fontmode[] =   {   0,   0,   1,   1,   1,   1,   1,   1,   1,   1 };  // 0,1
int TPL_6posx[] =       {   0,   0,   0,   0,  14,   0,   0,   0,   0,   0 };  // 0-127
int TPL_6posy[] =       {   8,   8,  -5,  -5,  28,  28,   0,   0,   0,   0 };  // 0-31
int TPL_6scroll[] =     {   1,   1,   0,   0,   0,   0,   0,   0,   0,   0 };  // 0,1
//  Scrollbox
int TPL_6boxx[] =       {  20,  36,   0,   0,   0,   0,   0,   0,   0,   0 };  // 0,127
int TPL_6boxy[] =       {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 };  // 0,31
int TPL_6boxw[] =       {  90,  72,   0,   0,   0,   0,   0,   0,   0,   0 };  // 0,127
int TPL_6boxh[] =       {  10,  10,   0,   0,   0,   0,   0,   0,   0,   0 };  // 0,31
//  Blackbox
int TPL_6drawcolor2[] = {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 };  // 0,1
int TPL_6fontmode2[] =  {   1,   1,   1,   1,   1,   1,   1,   1,   1,   1 };  // 0,1
int TPL_6box2x[] =      {  91,   0,   0,   0,   0,   0,   0,   0,   0,   0 };  // 0,127
int TPL_6box2y[] =      {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 };  // 0,31
int TPL_6box2w[] =      { 127,  35,   0,   0,   0,   0,   0,   0,   0,   0 };  // 0,127
int TPL_6box2h[] =      {  10,  10,   0,   0,   0,   0,   0,   0,   0,   0 };  // 0,31

// TEMPLATES Part 1
// FONTS 0 - 9
char fonts[10][40] = { "u8g2_font_7x13_tf", "u8g2_font_crox3hb_tf", "u8g2_font_9x15_tf", "u8g2_font_rosencrantz_nbp_tf", "u8g2_font_helvR08_tf", "u8g2_font_5x7_tf", "u8g2_font_6x10_tf", "u8g2_font_crox4hb_tf", "u8g2_font_crox1hb_tf", "u8g2_font_fur17_tf" };

const uint8_t *fontno[10] = { 
  u8g2_font_7x13_tf,                   // font0, start screen, 
  u8g2_font_crox3hb_tf,                // font1, station, 
  u8g2_font_9x15_tf,                   // font2, destination, message, station, 
  u8g2_font_rosencrantz_nbp_tf,        // font3, destination > 116, message > 116, 
  u8g2_font_helvR08_tf,                // font4, departure, 
  u8g2_font_5x7_tf,                    // font5, train, 
  u8g2_font_6x10_tf,                   // font6, message scroll, 
  u8g2_font_crox4hb_tf,                // font7, track, 
  u8g2_font_crox1hb_tf,                // font8, 
  u8g2_font_fur17_tf                   // font9, station
  };

// Logos in monochrome bitmap .xbm format
// LOGO 0 - 9:            0,     1,     2,     3,     4,     5,     6,     7,     8,     9
char logoId[10][4] = { "DB", "ICE",  "IC",  "RE",  "RB",   "U",   "S",   "A",    "",    "" };  // logo id
uint8_t logow[]    = {   17,    20,    11,    12,    12,    10,    12,    12,    10,    10 };  // logo width in pixel
uint8_t logoh[]    = {   11,     9,     8,    10,    10,    10,    12,    12,    10,    10 };  // logo height in pixel

//  Logo 0 - DB
unsigned char logo0[] = { 0xfe, 0xff, 0x00, 0x01, 0x00, 0x01, 0x7d, 0x3e, 0x01, 0xcd, 0x66, 0x01,
                          0xcd, 0x66, 0x01, 0xcd, 0x3e, 0x01, 0xcd, 0x66, 0x01, 0xcd, 0x66, 0x01,
                          0x7d, 0x3e, 0x01, 0x01, 0x00, 0x01, 0xfe, 0xff, 0x00 };                                 
const int logo0size = sizeof(logo0)/sizeof(logo0[0]);  // size of logo array
unsigned char (*pl0)[logo0size] = &logo0;              // pointer to logo

//  Logo 1 - ICE
unsigned char logo1[] = { 0x18, 0xcf, 0x0f, 0xdc, 0xcf, 0x0f, 0xcc, 0x60, 0x00, 0xec, 0x60, 0x00,
                          0x66, 0xe0, 0x03, 0x66, 0x30, 0x00, 0xe6, 0x30, 0x00, 0xe3, 0xf3, 0x03,
                          0xc3, 0xf3, 0x03 };
const int logo1size = sizeof(logo1)/sizeof(logo1[0]);
unsigned char (*pl1)[logo1size] = &logo1;

//  Logo 2 - IC
unsigned char logo2[] = { 0x98, 0x07, 0xcc, 0x01, 0xec, 0x00, 0x66, 0x00, 0x66, 0x00, 0x66, 0x00,
                          0xe3, 0x00, 0xc3, 0x01 };
const int logo2size = sizeof(logo2)/sizeof(logo2[0]);
unsigned char (*pl2)[logo2size] = &logo2;

//  Logo 3 - RE
unsigned char logo3[] = { 0xfe, 0x07, 0x03, 0x0c, 0xb9, 0x0b, 0xa9, 0x08, 0xbd, 0x09, 0x9d, 0x08,
                          0xd5, 0x08, 0xd5, 0x09, 0x03, 0x0c, 0xfe, 0x07 };
const int logo3size = sizeof(logo3)/sizeof(logo3[0]);
unsigned char (*pl3)[logo3size] = &logo3;

//  Logo 4 - RB
unsigned char logo4[] = { 0xfe, 0x07, 0x03, 0x0c, 0xb9, 0x0b, 0xa9, 0x0a, 0xbd, 0x0b, 0x9d, 0x0a,
                          0xd5, 0x0a, 0xd5, 0x0b, 0x03, 0x0c, 0xfe, 0x07 };
const int logo4size = sizeof(logo4)/sizeof(logo4[0]);
unsigned char (*pl4)[logo4size] = &logo4;

//  Logo 5 - U-Bahn
unsigned char logo5[] = { 0xff, 0x03, 0x33, 0x03, 0x33, 0x03, 0x33, 0x03, 0x33, 0x03, 0x33, 0x03,
                          0x33, 0x03, 0x33, 0x03, 0x87, 0x03, 0xff, 0x03 };
const int logo5size = sizeof(logo5)/sizeof(logo5[0]);
unsigned char (*pl5)[logo5size] = &logo5;

//  Logo 6 - S-Bahn
unsigned char logo6[] = { 0xf0, 0x00, 0x9c, 0x03, 0x06, 0x06, 0xe2, 0x04, 0xe3, 0x0f, 0x87, 0x0f,
                          0x1f, 0x0e, 0x7f, 0x0c, 0xf3, 0x04, 0x62, 0x06, 0x0c, 0x07, 0xf8, 0x01 };
const int logo6size = sizeof(logo6)/sizeof(logo6[0]);
unsigned char (*pl6)[logo6size] = &logo6;

//  Logo 7 - AKN
unsigned char logo7[] = { 0xf0, 0x00, 0x9c, 0x03, 0x0e, 0x07, 0x0e, 0x07, 0x67, 0x0e, 0xe7, 0x0e,
                          0xf7, 0x0e, 0x03, 0x0c, 0xf3, 0x04, 0xfa, 0x05, 0xfe, 0x07, 0xf0, 0x01 };
const int logo7size = sizeof(logo7)/sizeof(logo7[0]);
unsigned char (*pl7)[logo7size] = &logo7;

//  Logo 8 - x
unsigned char logo8[] = { 0x00 };
const int logo8size = sizeof(logo8)/sizeof(logo8[0]);
unsigned char (*pl8)[logo8size] = &logo8;

//  Logo 9 - x
unsigned char logo9[] = { 0x00 };
const int logo9size = sizeof(logo9)/sizeof(logo9[0]);
unsigned char (*pl9)[logo9size] = &logo9;

#endif
