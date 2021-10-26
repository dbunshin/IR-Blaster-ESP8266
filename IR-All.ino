#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

#include "CTBot.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "DHT.h"
//

#include <assert.h>
#include <IRrecv.h>
#include <IRac.h>
#include <IRtext.h>
#include <IRutils.h>

// LED INDICATOR PIN D6
#define LED2W D5
#define LED1B D6
#define LED3G D7

//-----------------------------
// DHT11
#define DHTPIN D3    // Digital pin connected to the DHT sensor at D3//
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "0.id.pool.ntp.org", 25200,60000);
CTBot myBot;
  
String ssid = "xxxx";     // REPLACE mySSID WITH YOUR WIFI SSID
String pass = "xxxx"; // REPLACE myPassword YOUR WIFI PASSWORD, IF ANY
String token = "xxxx"; // sinyalgaib_bot

//const uint16_t kIrLed = 2;  // ESP8266 GPIO pin IR LED to use. 2 (D4) //
const uint16_t kIrLed = 5;  // ESP8266 GPIO pin IR LED to use. 5 (D1) //

IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

// Raw data hasil dari receiver
// TCL TV
uint16_t rawDataTvOnOff[155] = {4078, 3986,  532, 2018,  532, 2020,  530, 2018,  532, 2020,  530, 1020,  530, 1020,  530, 2018,  562, 990,  532, 2018,  530, 1020,  530, 2020,  530, 1020,  530, 1020,  530, 1058,  532, 1006,  530, 1020,  530, 2018,  532, 2020,  530, 1020,  530, 2018,  532, 1018,  530, 2020,  530, 1020,  530, 2020,  530, 8540,  4062, 3986,  532, 2018,  532, 2018,  530, 2020,  532, 2018,  532, 1020,  532, 1020,  532, 2018,  532, 1020,  530, 2018,  532, 1020,  530, 2018,  532, 1020,  530, 1020,  560, 1030,  532, 1004,  532, 1020,  560, 1988,  530, 2020,  532, 1020,  560, 1988,  560, 992,  530, 2018,  532, 1020,  530, 2018,  532, 8566,  4062, 3984,  532, 2018,  562, 1990,  532, 2018,  532, 2018,  532, 1020,  532, 1020,  560, 1988,  532, 1018,  560, 1990,  562, 988,  532, 2020,  532, 1018,  560, 990,  532, 1072,  504, 1020,  558, 992,  562, 1988,  532, 2018,  532, 1020,  530, 2018,  532, 1020,  560, 1990,  532, 1020,  562, 1988,  560};  // NIKAI D5F2A
uint16_t rawDataTvHome[207] = {4052, 4016,  502, 2048,  502, 2048,  562, 1988,  562, 1988,  560, 990,  560, 990,  560, 992,  560, 990,  560, 1990,  560, 992,  500, 1050,  560, 990,  522, 1028,  502, 1142,  560, 976,  562, 990,  562, 1988,  560, 1990,  562, 1988,  560, 1990,  558, 992,  560, 1988,  502, 2048,  500, 2050,  500, 8570,  3998, 4048,  500, 2050,  502, 2048,  500, 2050,  560, 1990,  500, 1050,  560, 990,  500, 1050,  562, 990,  560, 1990,  500, 1050,  532, 1018,  532, 1018,  532, 1018,  502, 1102,  474, 1050,  558, 992,  532, 2020,  560, 1988,  532, 2018,  562, 1988,  500, 1050,  532, 2018,  500, 2050,  562, 1990,  562, 8510,  4030, 4016,  560, 1990,  532, 2020,  530, 2020,  500, 2050,  560, 990,  532, 1020,  532, 1018,  532, 1018,  532, 2018,  500, 1050,  562, 990,  532, 1018,  560, 990,  534, 1084,  500, 1036,  532, 1020,  560, 1988,  532, 2020,  532, 2018,  532, 2016,  534, 1018,  500, 2050,  532, 2018,  540, 2010,  532, 8566,  4000, 4048,  532, 2018,  532, 2018,  532, 2018,  532, 2016,  532, 1018,  532, 1018,  532, 1020,  532, 1018,  532, 2018,  530, 1020,  530, 1020,  532, 1018,  530, 1022,  532, 1070,  502, 1024,  532, 1018,  532, 2018,  532, 2018,  532, 2018,  532, 2018,  532, 1018,  530, 2020,  532, 2020,  530, 2018,  530};  // NIKAI F7F08

// lampu BT Sound
uint16_t rawDataLBTon[71] = {9338, 4490,  640, 538,  614, 588,  592, 586,  594, 562,  640, 538,  616, 562,  616, 562,  616, 1652,  614, 1650,  618, 1650,  616, 1648,  616, 1650,  616, 1648,  618, 1646,  618, 1650,  614, 564,  642, 1622,  618, 562,  616, 564,  616, 560,  616, 564,  616, 562,  618, 562,  618, 560,  620, 560,  618, 1650,  614, 1650,  620, 1648,  618, 1648,  618, 1648,  618, 1648,  592, 1674,  618, 39750,  9324, 2236,  614};  // NEC 1FE807F
uint16_t rawDataLBToff[71] = {9326, 4530,  600, 580,  600, 580,  600, 578,  600, 578,  600, 580,  598, 580,  600, 580,  600, 1666,  600, 1668,  598, 1668,  598, 1668,  598, 1666,  600, 1666,  600, 1666,  600, 1666,  598, 582,  598, 1666,  600, 578,  600, 578,  600, 580,  600, 580,  598, 580,  600, 578,  598, 582,  598, 582,  600, 1666,  600, 1666,  598, 1668,  598, 1668,  598, 1668,  600, 1666,  600, 1666,  598, 39750,  9328, 2248,  598};  // NEC 1FE807F
uint16_t rawDataLedYellow[71] = {9340, 4510,  622, 558,  620, 558,  622, 558,  622, 556,  622, 556,  594, 584,  620, 558,  620, 1644,  620, 1646,  618, 1648,  620, 1644,  620, 1646,  620, 1646,  622, 1644,  622, 1644,  594, 584,  622, 556,  622, 1644,  620, 1646,  620, 1644,  620, 1646,  620, 558,  622, 558,  620, 558,  596, 1670,  620, 558,  620, 558,  596, 584,  594, 584,  620, 1646,  594, 1672,  620, 1644,  624, 39726,  9338, 2226,  620};  // NEC 1FE7887

/// AC DAIKIN
uint16_t rawDataOnOff[137] = {9818, 9816,  9828, 9816,  4626, 2490,  370, 366,  368, 944,  368, 944,  368, 366,  368, 942,  370, 366,  368, 366,  368, 366,  368, 368,  368, 944,  368, 366,  368, 366,  370, 364,  368, 366,  368, 366,  368, 944,  368, 366,  368, 944,  368, 366,  368, 366,  368, 366,  368, 366,  370, 364,  368, 366,  368, 944,  370, 942,  368, 366,  368, 366,  368, 944,  368, 366,  368, 366,  368, 366,  368, 944,  368, 944,  368, 366,  368, 366,  368, 944,  368, 366,  368, 366,  368, 366,  368, 944,  368, 944,  368, 366,  368, 366,  368, 944,  368, 366,  368, 366,  368, 366,  368, 366,  368, 944,  370, 942,  370, 366,  368, 944,  368, 368,  368, 366,  368, 366,  368, 368,  368, 366,  368, 944,  370, 942,  368, 366,  368, 944,  368, 366,  368, 366,  368, 20272,  4626};  // DAIKIN64 2C16131313028216

uint16_t rawData26c[137] = {9820, 9816,  9878, 9766,  4678, 2440,  424, 312,  422, 890,  370, 942,  414, 320,  420, 892,  420, 314,  420, 316,  420, 314,  420, 314,  422, 890,  370, 366,  420, 314,  420, 316,  420, 314,  420, 314,  420, 890,  422, 890,  420, 892,  422, 314,  420, 314,  420, 892,  422, 892,  420, 314,  420, 314,  422, 312,  420, 892,  422, 890,  422, 314,  420, 314,  420, 314,  420, 314,  420, 314,  422, 892,  420, 892,  420, 314,  420, 316,  420, 890,  422, 314,  420, 314,  420, 314,  420, 892,  422, 890,  422, 314,  420, 314,  420, 894,  420, 314,  420, 314,  420, 316,  420, 314,  420, 892,  370, 942,  422, 314,  368, 366,  420, 892,  420, 316,  420, 314,  420, 314,  420, 316,  420, 892,  420, 316,  420, 890,  422, 314,  420, 314,  420, 314,  420, 20220,  4630};  // DAIKIN64 1426131306338216
uint16_t rawData28c[137] = {9788, 9848,  9818, 9826,  4686, 2432,  368, 368,  366, 946,  370, 944,  362, 372,  370, 942,  366, 368,  364, 370,  370, 366,  370, 366,  364, 946,  428, 308,  362, 372,  368, 366,  428, 304,  366, 370,  368, 942,  370, 944,  368, 944,  370, 364,  366, 370,  428, 306,  362, 372,  370, 364,  368, 366,  366, 948,  368, 944,  368, 944,  364, 372,  366, 368,  428, 306,  366, 370,  366, 368,  364, 946,  372, 942,  364, 370,  428, 306,  430, 882,  428, 308,  428, 306,  364, 370,  366, 946,  368, 944,  370, 366,  366, 368,  368, 944,  370, 364,  368, 368,  428, 306,  364, 370,  428, 308,  364, 370,  366, 946,  428, 306,  366, 946,  428, 308,  366, 368,  366, 368,  370, 364,  362, 950,  370, 364,  368, 944,  366, 368,  364, 370,  430, 304,  362, 20278,  4598};  // DAIKIN64 1428131307038216
uint16_t rawData16c[137] = {9880, 9760,  9884, 9760,  4684, 2432,  404, 332,  402, 910,  404, 910,  404, 330,  402, 910,  402, 332,  404, 332,  402, 332,  402, 332,  402, 910,  402, 332,  402, 332,  404, 330,  404, 332,  402, 332,  402, 908,  404, 332,  404, 908,  404, 330,  404, 332,  402, 332,  402, 910,  402, 332,  404, 330,  402, 332,  402, 910,  402, 332,  402, 332,  402, 910,  404, 330,  402, 332,  404, 332,  402, 910,  402, 910,  404, 330,  404, 332,  404, 910,  402, 330,  402, 332,  404, 330,  402, 912,  402, 910,  402, 332,  404, 332,  402, 910,  404, 332,  402, 330,  402, 332,  404, 332,  404, 908,  404, 908,  404, 330,  404, 910,  402, 332,  402, 332,  402, 334,  402, 332,  402, 332,  402, 910,  402, 332,  402, 910,  402, 910,  404, 332,  402, 910,  402, 20216,  4682};  // DAIKIN64 B416131312228216
uint16_t rawData30c[137] = {9826, 9816,  9800, 9846,  4658, 2460,  404, 332,  402, 910,  364, 948,  402, 332,  404, 908,  404, 330,  370, 364,  402, 332,  402, 332,  402, 910,  344, 392,  344, 390,  344, 392,  404, 330,  372, 362,  402, 910,  364, 370,  400, 334,  344, 392,  364, 948,  372, 362,  404, 910,  364, 370,  344, 392,  342, 392,  402, 910,  374, 362,  402, 332,  402, 910,  400, 334,  402, 332,  402, 334,  402, 910,  366, 946,  344, 392,  364, 370,  404, 908,  344, 390,  402, 910,  398, 338,  364, 948,  402, 910,  364, 370,  404, 332,  400, 914,  400, 334,  402, 910,  342, 392,  342, 392,  342, 392,  344, 392,  342, 392,  400, 912,  342, 970,  342, 392,  402, 332,  404, 332,  402, 332,  400, 912,  402, 332,  402, 912,  344, 392,  404, 908,  402, 332,  402, 20222,  4676};  // DAIKIN64 5430535312288216

uint16_t rawDataTurboOn[137] = {9880, 9738,  9906, 9738,  4710, 2428,  406, 330,  406, 908,  430, 880,  430, 304,  430, 882,  406, 328,  430, 304,  432, 302,  404, 330,  404, 908,  404, 332,  430, 304,  404, 910,  404, 908,  404, 330,  404, 330,  404, 910,  404, 330,  430, 304,  404, 908,  430, 882,  430, 882,  432, 302,  404, 330,  404, 330,  430, 882,  430, 304,  430, 304,  404, 908,  404, 328,  404, 330,  430, 304,  404, 908,  404, 908,  402, 332,  404, 330,  404, 910,  404, 330,  430, 304,  404, 330,  430, 882,  430, 882,  406, 328,  404, 330,  406, 908,  430, 304,  406, 330,  404, 332,  430, 302,  404, 910,  430, 882,  430, 304,  404, 908,  404, 330,  402, 332,  402, 332,  404, 332,  404, 330,  430, 882,  402, 332,  402, 332,  404, 910,  402, 910,  430, 882,  404, 20216,  4708};  // DAIKIN64 E416131312393216
uint16_t rawDataTurboOff[137] = {9858, 9780,  9858, 9784,  4684, 2434,  402, 332,  402, 910,  402, 910,  402, 332,  402, 910,  402, 334,  402, 332,  402, 332,  402, 332,  404, 908,  402, 334,  372, 362,  400, 334,  400, 334,  402, 332,  372, 940,  404, 908,  404, 330,  374, 360,  374, 938,  402, 910,  402, 910,  402, 332,  372, 362,  372, 364,  400, 912,  400, 334,  402, 332,  374, 940,  402, 334,  402, 332,  400, 334,  400, 912,  374, 940,  372, 362,  374, 360,  374, 940,  402, 332,  372, 362,  402, 332,  402, 910,  402, 910,  402, 332,  372, 362,  402, 910,  400, 334,  370, 364,  372, 362,  402, 332,  402, 910,  400, 912,  374, 362,  404, 908,  402, 332,  404, 330,  402, 334,  372, 364,  400, 332,  372, 940,  402, 332,  372, 940,  402, 910,  400, 334,  372, 364,  372, 20268,  4660};  // DAIKIN64 3416131312398216

// Swing On 26c
uint16_t rawDataSwingOn[137] = {9844, 9776,  9868, 9776,  4670, 2466,  396, 340,  396, 916,  396, 916,  396, 338,  420, 892,  394, 340,  396, 338,  368, 368,  394, 340,  392, 920,  394, 340,  394, 342,  396, 338,  396, 338,  396, 338,  394, 918,  396, 916,  394, 340,  394, 340,  392, 342,  396, 916,  396, 338,  396, 340,  394, 340,  394, 918,  396, 918,  396, 338,  394, 342,  394, 918,  396, 340,  396, 338,  396, 340,  394, 918,  396, 916,  396, 340,  396, 340,  394, 916,  394, 340,  392, 342,  392, 342,  394, 918,  392, 920,  394, 340,  392, 342,  392, 920,  394, 342,  392, 342,  392, 342,  394, 342,  390, 920,  392, 920,  394, 342,  390, 344,  392, 920,  392, 342,  394, 342,  392, 920,  394, 342,  392, 920,  390, 344,  392, 342,  390, 344,  390, 920,  392, 922,  394, 20248,  4646};  // DAIKIN64 C526131313118216
uint16_t rawDataSwingOff[137] = {9844, 9796,  9848, 9796,  4648, 2468,  394, 342,  390, 920,  392, 922,  394, 340,  392, 920,  366, 368,  368, 368,  394, 340,  392, 344,  394, 920,  392, 342,  392, 340,  392, 342,  394, 340,  366, 368,  392, 918,  392, 920,  392, 342,  418, 316,  392, 342,  392, 920,  392, 342,  394, 342,  390, 342,  392, 920,  392, 920,  394, 340,  392, 344,  392, 918,  392, 342,  392, 342,  394, 340,  392, 920,  392, 918,  394, 340,  420, 316,  394, 918,  392, 342,  392, 342,  392, 344,  392, 920,  392, 920,  398, 338,  366, 368,  366, 946,  394, 342,  394, 342,  390, 342,  392, 342,  390, 922,  394, 918,  392, 342,  394, 342,  392, 920,  394, 340,  392, 344,  390, 342,  394, 340,  392, 920,  392, 344,  390, 920,  394, 918,  392, 342,  394, 918,  394, 20228,  4668};  // DAIKIN64 B426131313118216

uint16_t rawDataFan[137] = {9794, 9844,  9800, 9844,  4620, 2496,  344, 392,  404, 906,  344, 968,  344, 390,  374, 938,  366, 368,  402, 332,  344, 392,  344, 390,  404, 330,  346, 966,  344, 390,  350, 384,  404, 330,  374, 360,  374, 938,  404, 330,  376, 360,  344, 390,  344, 390,  344, 390,  366, 368,  406, 328,  366, 370,  344, 390,  366, 370,  372, 940,  366, 368,  404, 908,  344, 392,  344, 390,  404, 330,  366, 946,  344, 968,  344, 392,  344, 390,  404, 908,  366, 370,  344, 390,  344, 390,  366, 946,  344, 968,  404, 332,  368, 366,  344, 968,  344, 390,  404, 330,  344, 390,  372, 362,  402, 910,  358, 956,  344, 390,  344, 390,  368, 944,  342, 392,  342, 390,  344, 390,  364, 372,  342, 970,  364, 370,  342, 392,  404, 330,  344, 968,  364, 946,  344, 20274,  4620};  // DAIKIN64 C426131314008416
uint16_t rawDataCold[137] = {9824, 9814,  9878, 9766,  4674, 2442,  420, 314,  420, 892,  376, 936,  422, 314,  420, 892,  416, 318,  420, 314,  342, 392,  420, 316,  372, 938,  344, 390,  376, 358,  342, 392,  342, 392,  374, 362,  342, 970,  372, 362,  376, 358,  374, 360,  374, 362,  374, 362,  372, 362,  344, 392,  374, 360,  376, 358,  416, 320,  374, 936,  376, 358,  376, 936,  376, 358,  374, 360,  376, 360,  374, 936,  376, 936,  376, 360,  374, 360,  372, 938,  420, 316,  374, 358,  376, 360,  372, 940,  376, 936,  342, 392,  374, 360,  374, 938,  374, 360,  374, 360,  420, 314,  376, 358,  374, 938,  374, 938,  372, 362,  418, 318,  374, 938,  420, 314,  376, 358,  374, 360,  420, 314,  372, 938,  376, 360,  374, 360,  374, 938,  376, 360,  374, 938,  374, 20248,  4650};  // DAIKIN64 A426131314008216

// swing fan full On 16c
uint16_t rawDataAll16[137] = {9884, 9738,  9912, 9756,  4690, 2428,  408, 328,  406, 908,  406, 906,  406, 328,  404, 908,  432, 302,  404, 332,  402, 330,  404, 330,  404, 908,  406, 330,  406, 328,  434, 300,  436, 878,  430, 304,  406, 328,  406, 906,  428, 884,  406, 330,  432, 302,  406, 906,  432, 880,  406, 328,  404, 330,  404, 330,  406, 906,  406, 328,  406, 330,  406, 906,  406, 330,  406, 328,  404, 330,  404, 908,  406, 906,  406, 328,  406, 328,  404, 908,  406, 328,  404, 906,  434, 300,  404, 908,  432, 880,  406, 328,  432, 304,  408, 904,  406, 330,  432, 878,  406, 330,  404, 330,  406, 906,  406, 906,  432, 302,  404, 908,  432, 302,  404, 330,  406, 328,  406, 906,  406, 330,  432, 880,  404, 330,  410, 326,  404, 330,  406, 328,  406, 330,  406, 20216,  4710};  // DAIKIN64 516535312332216

//-----------------------------

// ==================== start of TUNEABLE PARAMETERS ====================
// An IR detector/demodulator is connected to GPIO pin 14
// e.g. D5 on a NodeMCU board.
// Note: GPIO 16 won't work on the ESP8266 as it does not have interrupts.
const uint16_t kRecvPin = 4; // IR RECEIVER PIN D2

// The Serial connection baud rate.
// i.e. Status message will be sent to the PC at this baud rate.
// Try to avoid slow speeds like 9600, as you will miss messages and
// cause other problems. 115200 (or faster) is recommended.
// NOTE: Make sure you set your Serial Monitor to the same speed.
const uint32_t kBaudRate = 115200;

// As this program is a special purpose capture/decoder, let us use a larger
// than normal buffer so we can handle Air Conditioner remote codes.
const uint16_t kCaptureBufferSize = 1024;

// kTimeout is the Nr. of milli-Seconds of no-more-data before we consider a
// message ended.
// This parameter is an interesting trade-off. The longer the timeout, the more
// complex a message it can capture. e.g. Some device protocols will send
// multiple message packets in quick succession, like Air Conditioner remotes.
// Air Coniditioner protocols often have a considerable gap (20-40+ms) between
// packets.
// The downside of a large timeout value is a lot of less complex protocols
// send multiple messages when the remote's button is held down. The gap between
// them is often also around 20+ms. This can result in the raw data be 2-3+
// times larger than needed as it has captured 2-3+ messages in a single
// capture. Setting a low timeout value can resolve this.
// So, choosing the best kTimeout value for your use particular case is
// quite nuanced. Good luck and happy hunting.
// NOTE: Don't exceed kMaxTimeoutMs. Typically 130ms.
#if DECODE_AC
// Some A/C units have gaps in their protocols of ~40ms. e.g. Kelvinator
// A value this large may swallow repeats of some protocols
const uint8_t kTimeout = 50;
#else   // DECODE_AC
// Suits most messages, while not swallowing many repeats.
const uint8_t kTimeout = 15;
#endif  // DECODE_AC
// Alternatives:
// const uint8_t kTimeout = 90;
// Suits messages with big gaps like XMP-1 & some aircon units, but can
// accidentally swallow repeated messages in the rawData[] output.
//
// const uint8_t kTimeout = kMaxTimeoutMs;
// This will set it to our currently allowed maximum.
// Values this high are problematic because it is roughly the typical boundary
// where most messages repeat.
// e.g. It will stop decoding a message and start sending it to serial at
//      precisely the time when the next message is likely to be transmitted,
//      and may miss it.

// Set the smallest sized "UNKNOWN" message packets we actually care about.
// This value helps reduce the false-positive detection rate of IR background
// noise as real messages. The chances of background IR noise getting detected
// as a message increases with the length of the kTimeout value. (See above)
// The downside of setting this message too large is you can miss some valid
// short messages for protocols that this library doesn't yet decode.
//
// Set higher if you get lots of random short UNKNOWN messages when nothing
// should be sending a message.
// Set lower if you are sure your setup is working, but it doesn't see messages
// from your device. (e.g. Other IR remotes work.)
// NOTE: Set this value very high to effectively turn off UNKNOWN detection.
const uint16_t kMinUnknownSize = 12;

// How much percentage lee way do we give to incoming signals in order to match
// it?
// e.g. +/- 25% (default) to an expected value of 500 would mean matching a
//      value between 375 & 625 inclusive.
// Note: Default is 25(%). Going to a value >= 50(%) will cause some protocols
//       to no longer match correctly. In normal situations you probably do not
//       need to adjust this value. Typically that's when the library detects
//       your remote's message some of the time, but not all of the time.
const uint8_t kTolerancePercentage = kTolerance;  // kTolerance is normally 25%

// Legacy (No longer supported!)
//
// Change to `true` if you miss/need the old "Raw Timing[]" display.
#define LEGACY_TIMING_INFO false
// ==================== end of TUNEABLE PARAMETERS ====================

// Use turn on the save buffer feature for more complete capture coverage.
IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);
decode_results results;  // Somewhere to store the results

// This section of code runs only once at start-up.
void setup() {
// LED
  pinMode(LED1B, OUTPUT);
  pinMode(LED2W, OUTPUT);
  pinMode(LED3G, OUTPUT);
  
//--
  timeClient.begin();
  irsend.begin();
  Serial.begin(115200);

// telegram
  Serial.println("Memulai Telegram Bot...");

  // connect the ESP8266 to the desired access point
  myBot.wifiConnect(ssid, pass);

  // set the telegram bot token
  myBot.setTelegramToken(token);

  // check if all things are ok
  if (myBot.testConnection())
    Serial.println("\n Koneksi OK Gan!");
  else
    Serial.println("\n Koneksi Bermasalah .. hiks..");
//--
		
#if defined(ESP8266)
  Serial.begin(kBaudRate, SERIAL_8N1, SERIAL_TX_ONLY);
#else  // ESP8266
  Serial.begin(kBaudRate, SERIAL_8N1);
#endif  // ESP8266
  while (!Serial)  // Wait for the serial connection to be establised.
    delay(50);
  // Perform a low level sanity checks that the compiler performs bit field
  // packing as we expect and Endianness is as we expect.
  assert(irutils::lowLevelSanityCheck() == 0);

  Serial.printf("\n" D_STR_IRRECVDUMP_STARTUP "\n", kRecvPin);
#if DECODE_HASH
  // Ignore messages with less than minimum on or off pulses.
  irrecv.setUnknownThreshold(kMinUnknownSize);
#endif  // DECODE_HASH
  irrecv.setTolerance(kTolerancePercentage);  // Override the default tolerance.
  irrecv.enableIRIn();  // Start the receiver
}

// The repeating section of the code
void loop() 
{
	
///----------------------
  timeClient.update();

// DHT11
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  
  //int temp = dht.readTemperature();
  //float temp = dht.readTemperature();
  
  String humid = "Kelembapan / Humidity : " + String(h) + " % \n";
  String tempstat = "Suhu / Temperature : " + String(t) + "°C ";
  delay (50);
  
// a variable to store telegram message data
  TBMessage msg;

  // if there is an incoming message...
  if (myBot.getNewMessage(msg)) {

/*  // restart esp8266 -- not working
  if (msg.text.equalsIgnoreCase("/restart")) {
      delay (200);
      ulang();
      Serial.println(F("Restarting.."));
    }
*/
    
    // remote telegram
      if (msg.text.equalsIgnoreCase("/Status")) {
      LedKedip();
      delay (200);
      myBot.sendMessage(msg.sender.id, tempstat);  
      delay (200);    
      myBot.sendMessage(msg.sender.id, humid);
      Serial.println(F("Temperature and Humidity"));
    }
        
      else if (msg.text.equalsIgnoreCase("/On")) {
      delay (200);
      irsend.sendRaw(rawDataOnOff, 137,38); // set Power On/Off @38khz
      delay (200);
      myBot.sendMessage(msg.sender.id, "On");
      Serial.println(F("On/Off"));
    }
    
      else if (msg.text.equalsIgnoreCase("/Off")) {
      delay (200);      
      irsend.sendRaw(rawDataOnOff, 137,38); // set Power On/Off @38khz
      delay (200);
      myBot.sendMessage(msg.sender.id, "Off");
      Serial.println(F("On/Off"));
    }
    
      else if (msg.text.equalsIgnoreCase("/26")) {
      myBot.sendMessage(msg.sender.id, "Set to 26C");
      delay (200);
      digitalWrite(LED1B,HIGH);
      delay (1000);
      digitalWrite(LED1B,LOW);      
      irsend.sendRaw(rawData26c, 137,38); // set to 26C @38khz
      delay (200);
      myBot.sendMessage(msg.sender.id, tempstat);
      Serial.println(F("Set to 26C"));
    }

      else if (msg.text.equalsIgnoreCase("/28")) {
      myBot.sendMessage(msg.sender.id, "Set to 28C");  
      delay (200);
      digitalWrite(LED1B,HIGH);
      delay (1000);
      digitalWrite(LED1B,LOW);        
      irsend.sendRaw(rawData28c, 137,38); // set to 28C @38khz
      delay (200);
      myBot.sendMessage(msg.sender.id, tempstat);
      Serial.println(F("Set to 28C"));
    }

      else if (msg.text.equalsIgnoreCase("/30")) {
      myBot.sendMessage(msg.sender.id, "Set to 30C");    
      delay (200);
      digitalWrite(LED1B,HIGH);
      delay (1000);
      digitalWrite(LED1B,LOW);      
      irsend.sendRaw(rawData30c, 137,38); // set to 30C @38khz
      delay (200);
      myBot.sendMessage(msg.sender.id, tempstat);
      Serial.println(F("Set to 30C"));
    }

      else if (msg.text.equalsIgnoreCase("/all16")) {
      myBot.sendMessage(msg.sender.id, "Set to All 16C");    
      delay (200);
      digitalWrite(LED1B,HIGH);
      delay (1000);
      digitalWrite(LED1B,LOW); 
      irsend.sendRaw(rawDataAll16, 137,38); // set to 30C @38khz
      delay (200);
      myBot.sendMessage(msg.sender.id, tempstat);
      Serial.println(F("Set to All 16C"));
    }
        
      else if (msg.text.equalsIgnoreCase("/16")) {
      myBot.sendMessage(msg.sender.id, "Set to 16C");     
      delay (200);
      digitalWrite(LED1B,HIGH);
      delay (1000);
      digitalWrite(LED1B,LOW); 
      irsend.sendRaw(rawData16c, 137,38); // set to 16C @38khz
      delay (200);
      myBot.sendMessage(msg.sender.id, tempstat);
      Serial.println(F("Set to 16C"));
    }

      else if (msg.text.equalsIgnoreCase("/turbon")) {
      myBot.sendMessage(msg.sender.id, "Turbo On");     
      delay (200);
      digitalWrite(LED1B,HIGH);
      delay (1000);
      digitalWrite(LED1B,LOW); 
      irsend.sendRaw(rawDataTurboOn, 137,38);
      delay (200);
      Serial.println(F("Turbo On"));
    }

      else if (msg.text.equalsIgnoreCase("/turboff")) {
      myBot.sendMessage(msg.sender.id, "Turbo Off");     
      delay (200);
      digitalWrite(LED1B,HIGH);
      delay (1000);
      digitalWrite(LED1B,LOW); 
      irsend.sendRaw(rawDataTurboOff, 137,38); 
      delay (200);
      Serial.println(F("Turbo Off"));
    }

      else if (msg.text.equalsIgnoreCase("/swingon")) {
      myBot.sendMessage(msg.sender.id, "Swing On");     
      delay (200);
      digitalWrite(LED1B,HIGH);
      delay (1000);
      digitalWrite(LED1B,LOW); 
      irsend.sendRaw(rawDataSwingOn, 137,38); // 1 kondisi
      delay (200);
      Serial.println(F("Swing On"));
    }

      else if (msg.text.equalsIgnoreCase("/swingoff")) {
      myBot.sendMessage(msg.sender.id, "Swing Off");     
      delay (200);
      digitalWrite(LED1B,HIGH);
      delay (1000);
      digitalWrite(LED1B,LOW); 
      irsend.sendRaw(rawDataSwingOff, 137,38); // 1 kondisi
      delay (200);
      Serial.println(F("Swing Off"));
    }

      else if (msg.text.equalsIgnoreCase("/fan")) {
      myBot.sendMessage(msg.sender.id, "Mode Fan");     
      delay (200);
      digitalWrite(LED3G,HIGH);
      delay (1000);
      digitalWrite(LED3G,LOW); 
      irsend.sendRaw(rawDataFan, 137,38); 
      delay (200);
      myBot.sendMessage(msg.sender.id, tempstat);
      Serial.println(F("Mode Fan"));
    }
    
      else if (msg.text.equalsIgnoreCase("/cold")) {
      myBot.sendMessage(msg.sender.id, "Mode Cold");     
      delay (200);
      digitalWrite(LED1B,HIGH);
      digitalWrite(LED2W,HIGH);
      delay (1000);
      digitalWrite(LED1B,LOW);
      digitalWrite(LED2W,LOW);
      irsend.sendRaw(rawDataCold, 137,38); 
      delay (200);
      myBot.sendMessage(msg.sender.id, tempstat);
      Serial.println(F("Mode Cold"));
    }  
    
/// TV TCL ///
      else if (msg.text.equalsIgnoreCase("/TvOn")) {
      myBot.sendMessage(msg.sender.id, "Tv On");     
      delay (200);
      digitalWrite(LED2W,HIGH);
      delay (1000);
      digitalWrite(LED2W,LOW);
      irsend.sendRaw(rawDataTvOnOff, 155,38); 
      delay (200);
      Serial.println(F("Tv On"));
    }
      else if (msg.text.equalsIgnoreCase("/TvOff")) {
      myBot.sendMessage(msg.sender.id, "Tv Off");     
      delay (200);
      digitalWrite(LED2W,HIGH);
      delay (1000);
      digitalWrite(LED2W,LOW);      
      irsend.sendRaw(rawDataTvOnOff, 155,38); 
      delay (200);
      Serial.println(F("Tv Off"));
    }
    
      else if (msg.text.equalsIgnoreCase("/TvHome")) {
      myBot.sendMessage(msg.sender.id, "Tv Home");     
      delay (200);
      digitalWrite(LED2W,HIGH);
      delay (1000);
      digitalWrite(LED2W,LOW);      
      irsend.sendRaw(rawDataTvHome, 207,38); 
      delay (200);
      Serial.println(F("Tv Home"));
    }

//// LBT ///
      else if (msg.text.equalsIgnoreCase("/LBTon")) {
      myBot.sendMessage(msg.sender.id, "Lampu BT On");     
      delay (200);
      digitalWrite(LED2W,HIGH);
      delay (1000);
      digitalWrite(LED2W,LOW);      
      irsend.sendRaw(rawDataLBTon, 71,38); 
      delay (200);
      Serial.println(F("Lampu BT"));
    }
      else if (msg.text.equalsIgnoreCase("/LBToff")) {
      myBot.sendMessage(msg.sender.id, "Lampu BT Off");     
      delay (200);
      digitalWrite(LED2W,HIGH);
      delay (1000);
      digitalWrite(LED2W,LOW);      
      irsend.sendRaw(rawDataLBToff, 71,38);
      delay (200);
      Serial.println(F("Lampu BT Off"));
    }
      else if (msg.text.equalsIgnoreCase("/lbtcolor")) {
      myBot.sendMessage(msg.sender.id, "Ganti Warna Lampu BT");     
      delay (200);
      digitalWrite(LED2W,HIGH);
      delay (1000);
      digitalWrite(LED2W,LOW);      
      irsend.sendRaw(rawDataLedYellow, 71,38);
      delay (200);
      Serial.println(F("Ganti Warna Lampu BT"));
    }    

// LED internal Switch
      else if (msg.text.equalsIgnoreCase("/ledBon")) {
      myBot.sendMessage(msg.sender.id, "LED Blue On");     
      delay (100);
      digitalWrite(LED1B,HIGH);
      delay (100);
      Serial.println(F("LED Blue On"));
    }
      else if (msg.text.equalsIgnoreCase("/ledBoff")) {
      myBot.sendMessage(msg.sender.id, "LED Blue Off");     
      delay (100);
      digitalWrite(LED1B,LOW); 
      delay (100);
      Serial.println(F("LED Blue Off"));
    }
      else if (msg.text.equalsIgnoreCase("/ledWon")) {
      myBot.sendMessage(msg.sender.id, "LED White On");     
      delay (100);
      digitalWrite(LED2W,HIGH);
      delay (100);
      Serial.println(F("LED White On"));
    }
      else if (msg.text.equalsIgnoreCase("/ledWoff")) {
      myBot.sendMessage(msg.sender.id, "LED White Off");     
      delay (100);
      digitalWrite(LED2W,LOW); 
      delay (100);
      Serial.println(F("LED White Off"));
    }
      else if (msg.text.equalsIgnoreCase("/ledGon")) {
      myBot.sendMessage(msg.sender.id, "LED Green On");     
      delay (100);
      digitalWrite(LED3G,HIGH);
      delay (100);
      Serial.println(F("LED Green On"));
    }
      else if (msg.text.equalsIgnoreCase("/ledGoff")) {
      myBot.sendMessage(msg.sender.id, "LED Green Off");     
      delay (100);
      digitalWrite(LED3G,LOW); 
      delay (100);
      Serial.println(F("LED Green Off"));
    }    
// all LED
      else if (msg.text.equalsIgnoreCase("/ledAllon")) {
      myBot.sendMessage(msg.sender.id, "LED All On");     
      delay (100);
      digitalWrite(LED1B,HIGH);
      digitalWrite(LED2W,HIGH);            
      digitalWrite(LED3G,HIGH);
      delay (100);
      Serial.println(F("LED All On"));
    }
      else if (msg.text.equalsIgnoreCase("/ledAlloff")) {
      myBot.sendMessage(msg.sender.id, "LED All Off");     
      delay (100);
      digitalWrite(LED1B,LOW); 
      digitalWrite(LED2W,LOW);
      digitalWrite(LED3G,LOW);
      delay (100);
      Serial.println(F("LED All Off"));
    }
    // call LedJalan
      else if (msg.text.equalsIgnoreCase("/ledjalan")) {
      myBot.sendMessage(msg.sender.id, "LED Jalan 6x");     
      delay (100);
      LedJalan();
      LedJalan();
      LedJalan();
      LedJalan();
      LedJalan();
      LedJalan();
      delay (100);
      Serial.println(F("LED Jalan 6x"));
    }
          
// call functions
      else if (msg.text.equalsIgnoreCase("/ftc")) {
      myBot.sendMessage(msg.sender.id, "Fan to Cold 5 minutes");     
      delay (200);
      digitalWrite(LED3G,HIGH);
      delay (1000);
      digitalWrite(LED3G,LOW); 
      FanToCold();
      delay (200);
      myBot.sendMessage(msg.sender.id, tempstat);
      Serial.println(F("Fan to Cold 5 minutes"));
    }
      else if (msg.text.equalsIgnoreCase("/ftc15")) {
      myBot.sendMessage(msg.sender.id, "Fan to Cold 15 minutes");     
      delay (200);
      digitalWrite(LED3G,HIGH);
      delay (1000);
      digitalWrite(LED3G,LOW); 
      FanToColdLM();
      delay (200);
      myBot.sendMessage(msg.sender.id, tempstat);
      Serial.println(F("Fan to Cold 15 minutes"));
    }

      else if (msg.text.equalsIgnoreCase("/ftc10")) {
      myBot.sendMessage(msg.sender.id, "Fan to Cold 10 minutes");     
      delay (200);
      digitalWrite(LED3G,HIGH);
      delay (1000);
      digitalWrite(LED3G,LOW); 
      FanToColdSM();
      delay (200);
      myBot.sendMessage(msg.sender.id, tempstat);
      Serial.println(F("Fan to Cold 10 minutes"));      
    }

      else if (msg.text.equalsIgnoreCase("/30ke26")) {
      myBot.sendMessage(msg.sender.id, "30c to 26c");     
      delay (100);
      digitalWrite(LED3G,HIGH);
      delay (500);
      digitalWrite(LED3G,LOW);
      delay (500);
      digitalWrite(LED2W,HIGH);
      delay (500);
      digitalWrite(LED2W,LOW);      
      TigaKDua();
      delay (200);
      digitalWrite(LED1B,HIGH);
      delay (500);
      digitalWrite(LED1B,LOW);
      delay (200);
      myBot.sendMessage(msg.sender.id, tempstat);
      Serial.println(F("30c to 26c"));      
    }        
      else if (msg.text.equalsIgnoreCase("/stabil27")) {
      delay (200);
      myBot.sendMessage(msg.sender.id, tempstat);
      delay (200);
      myBot.sendMessage(msg.sender.id, "Stabilkan di 27C");     
      delay (200);
      SuhuBalance27();
      delay (200);
      Serial.println(F("\n Stabilkan di 27C"));
    }     
    else {                                                  
      // generate the non commands message for the sender
      String reply;
      reply = (String)"Selamat datang " + msg.sender.username + (String)" di Bot Pengendali Sinyal Gaib";
      myBot.sendMessage(msg.sender.id, reply);             // and send it
    }
    
  }

///----------------------
	
  // Check if the IR code has been received.
  if (irrecv.decode(&results)) {
    // Display a crude timestamp.
    uint32_t now = millis();
    Serial.printf(D_STR_TIMESTAMP " : %06u.%03u\n", now / 1000, now % 1000);
    // Check if we got an IR message that was to big for our capture buffer.
    if (results.overflow)
      Serial.printf(D_WARN_BUFFERFULL "\n", kCaptureBufferSize);
    // Display the library version the message was captured with.
    Serial.println(D_STR_LIBRARY "   : v" _IRREMOTEESP8266_VERSION_ "\n");
    // Display the tolerance percentage if it has been change from the default.
    if (kTolerancePercentage != kTolerance)
      Serial.printf(D_STR_TOLERANCE " : %d%%\n", kTolerancePercentage);
    // Display the basic output of what we found.
    Serial.print(resultToHumanReadableBasic(&results));
    // Display any extra A/C info if we have it.
    String description = IRAcUtils::resultAcToString(&results);
    if (description.length()) Serial.println(D_STR_MESGDESC ": " + description);
    yield();  // Feed the WDT as the text output can take a while to print.
#if LEGACY_TIMING_INFO
    // Output legacy RAW timing info of the result.
    Serial.println(resultToTimingInfo(&results));
    yield();  // Feed the WDT (again)
#endif  // LEGACY_TIMING_INFO
    // Output the results as source code
    Serial.println(resultToSourceCode(&results));
    Serial.println();    // Blank line between entries
    yield();             // Feed the WDT (again)
  }
}
  
//---

/* // not working
void ulang() {
  delay (2000);
  ESP.restart();
}
*/

void TigaKDua() {
  irsend.sendRaw(rawDataFan, 137,38);
  delay (500);
  irsend.sendRaw(rawData30c, 137,38);
  delay (10000); // 10 secs
  irsend.sendRaw(rawData26c, 137,38);
}

void FanToCold() {
  irsend.sendRaw(rawDataFan, 137,38);
  delay (5*60000); // 5 mins
  irsend.sendRaw(rawDataCold, 137,38);
}

void FanToColdSM() {
  irsend.sendRaw(rawDataFan, 137,38);
  delay (10*60000); // 10 mins
  irsend.sendRaw(rawDataCold, 137,38);
}

void FanToColdLM() {
  irsend.sendRaw(rawDataFan, 137,38);
  delay (15*60000); // 15 mins
  irsend.sendRaw(rawDataCold, 137,38);
}

void LedJalan() {
  // led cepet bolak balik
  // 100ms
  digitalWrite(LED1B, HIGH);
  delay(100);
  digitalWrite(LED1B, LOW);
  delay(100);

  digitalWrite(LED2W, HIGH);
  delay(100);
  digitalWrite(LED2W, LOW);
  delay(100);

  digitalWrite(LED3G, HIGH);
  delay(100);
  digitalWrite(LED3G, LOW);
  delay(100);

  digitalWrite(LED1B, HIGH);
  delay(100);
  digitalWrite(LED1B, LOW);
  delay(100);
  
  digitalWrite(LED2W, HIGH);
  delay(100);
  digitalWrite(LED2W, LOW);
  delay(100);

  digitalWrite(LED3G, HIGH);
  delay(100);
  digitalWrite(LED3G, LOW);
  delay(100);

  // 200ms
  digitalWrite(LED1B, HIGH);
  delay(200);
  digitalWrite(LED1B, LOW);
  delay(200);

  digitalWrite(LED2W, HIGH);
  delay(200);
  digitalWrite(LED2W, LOW);
  delay(200);

  digitalWrite(LED3G, HIGH);
  delay(200);
  digitalWrite(LED3G, LOW);
  delay(200);

  digitalWrite(LED1B, HIGH);
  delay(200);
  digitalWrite(LED1B, LOW);
  delay(200);
  
  digitalWrite(LED2W, HIGH);
  delay(200);
  digitalWrite(LED2W, LOW);
  delay(200);

  digitalWrite(LED3G, HIGH);
  delay(200);
  digitalWrite(LED3G, LOW);
  delay(200);

  // 300ms
  digitalWrite(LED1B, HIGH);
  delay(300);
  digitalWrite(LED1B, LOW);
  delay(300);

  digitalWrite(LED2W, HIGH);
  delay(300);
  digitalWrite(LED2W, LOW);
  delay(300);

  digitalWrite(LED3G, HIGH);
  delay(300);
  digitalWrite(LED3G, LOW);
  delay(300);

  digitalWrite(LED1B, HIGH);
  delay(300);
  digitalWrite(LED1B, LOW);
  delay(300);
  
  digitalWrite(LED2W, HIGH);
  delay(300);
  digitalWrite(LED2W, LOW);
  delay(300);

  digitalWrite(LED3G, HIGH);
  delay(300);
  digitalWrite(LED3G, LOW);
  delay(300);  
  
  // solo 100 ms
  digitalWrite(LED1B, HIGH);
  delay(100);
  digitalWrite(LED1B, LOW);
  delay(100);
  digitalWrite(LED1B, HIGH);
  delay(100);
  digitalWrite(LED1B, LOW);
  delay(100);  
  digitalWrite(LED1B, HIGH);
  delay(100);
  digitalWrite(LED1B, LOW);
  delay(100);

  digitalWrite(LED2W, HIGH);
  delay(100);
  digitalWrite(LED2W, LOW);
  delay(100);
  digitalWrite(LED2W, HIGH);
  delay(100);
  digitalWrite(LED2W, LOW);
  delay(100);  
  digitalWrite(LED2W, HIGH);
  delay(100);
  digitalWrite(LED2W, LOW);
  delay(100);

  digitalWrite(LED3G, HIGH);
  delay(100);
  digitalWrite(LED3G, LOW);
  delay(100);
  digitalWrite(LED3G, HIGH);
  delay(100);
  digitalWrite(LED3G, LOW);
  delay(100);  
  digitalWrite(LED3G, HIGH);
  delay(100);
  digitalWrite(LED3G, LOW);
  delay(100);   

  digitalWrite(LED3G, HIGH);
  delay(100);
  digitalWrite(LED3G, LOW);
  delay(100);
  digitalWrite(LED3G, HIGH);
  delay(100);
  digitalWrite(LED3G, LOW);
  delay(100);  
  digitalWrite(LED3G, HIGH);
  delay(100);
  digitalWrite(LED3G, LOW);
  delay(100);

  digitalWrite(LED2W, HIGH);
  delay(100);
  digitalWrite(LED2W, LOW);
  delay(100);
  digitalWrite(LED2W, HIGH);
  delay(100);
  digitalWrite(LED2W, LOW);
  delay(100);  
  digitalWrite(LED2W, HIGH);
  delay(100);
  digitalWrite(LED2W, LOW);
  delay(100);

  digitalWrite(LED1B, HIGH);
  delay(100);
  digitalWrite(LED1B, LOW);
  delay(100);
  digitalWrite(LED1B, HIGH);
  delay(100);
  digitalWrite(LED1B, LOW);
  delay(100);  
  digitalWrite(LED1B, HIGH);
  delay(100);
  digitalWrite(LED1B, LOW);
  delay(100);
  
  // bareng 300-500 ms
  delay(300);  
  digitalWrite(LED1B, HIGH);
  digitalWrite(LED2W, HIGH);
  digitalWrite(LED3G, HIGH);  
  delay(300);
  digitalWrite(LED1B, LOW);
  digitalWrite(LED2W, LOW);  
  digitalWrite(LED3G, LOW);  
  delay(300);

  delay(400);  
  digitalWrite(LED1B, HIGH);
  digitalWrite(LED2W, HIGH);
  digitalWrite(LED3G, HIGH);  
  delay(400);
  digitalWrite(LED1B, LOW);
  digitalWrite(LED2W, LOW);  
  digitalWrite(LED3G, LOW);  
  delay(400);

  delay(500);  
  digitalWrite(LED1B, HIGH);
  digitalWrite(LED2W, HIGH);
  digitalWrite(LED3G, HIGH);  
  delay(500);
  digitalWrite(LED1B, LOW);
  digitalWrite(LED2W, LOW);  
  digitalWrite(LED3G, LOW);  
  delay(500);

  delay(300);  
  digitalWrite(LED1B, HIGH);
  digitalWrite(LED2W, HIGH);
  digitalWrite(LED3G, HIGH);  
  delay(300);
  digitalWrite(LED1B, LOW);
  digitalWrite(LED2W, LOW);  
  digitalWrite(LED3G, LOW);  
  delay(300);

  delay(200);  
  digitalWrite(LED1B, HIGH);
  digitalWrite(LED2W, HIGH);
  digitalWrite(LED3G, HIGH);  
  delay(200);
  digitalWrite(LED1B, LOW);
  digitalWrite(LED2W, LOW);  
  digitalWrite(LED3G, LOW);  
  delay(200);

  delay(100);  
  digitalWrite(LED1B, HIGH);
  digitalWrite(LED2W, HIGH);
  digitalWrite(LED3G, HIGH);  
  delay(100);
  digitalWrite(LED1B, LOW);
  digitalWrite(LED2W, LOW);  
  digitalWrite(LED3G, LOW);  
  delay(100);

  delay(50);  
  digitalWrite(LED1B, HIGH);
  digitalWrite(LED2W, HIGH);
  digitalWrite(LED3G, HIGH);  
  delay(50);
  digitalWrite(LED1B, LOW);
  digitalWrite(LED2W, LOW);  
  digitalWrite(LED3G, LOW);  
  delay(50);

  delay(50);  
  digitalWrite(LED1B, HIGH);
  digitalWrite(LED2W, HIGH);
  digitalWrite(LED3G, HIGH);  
  delay(50);
  digitalWrite(LED1B, LOW);
  digitalWrite(LED2W, LOW);  
  digitalWrite(LED3G, LOW);  
  delay(50);

  delay(50);  
  digitalWrite(LED1B, HIGH);
  digitalWrite(LED2W, HIGH);
  digitalWrite(LED3G, HIGH);  
  delay(50);
  digitalWrite(LED1B, LOW);
  digitalWrite(LED2W, LOW);  
  digitalWrite(LED3G, LOW);  
  delay(50);

  delay(150);  
  digitalWrite(LED1B, HIGH);
  digitalWrite(LED2W, HIGH);
  digitalWrite(LED3G, HIGH);  
  delay(150);
  digitalWrite(LED1B, LOW);
  digitalWrite(LED2W, LOW);  
  digitalWrite(LED3G, LOW);  
  delay(150);

  delay(250);  
  digitalWrite(LED1B, HIGH);
  digitalWrite(LED2W, HIGH);
  digitalWrite(LED3G, HIGH);  
  delay(250);
  digitalWrite(LED1B, LOW);
  digitalWrite(LED2W, LOW);  
  digitalWrite(LED3G, LOW);  
  delay(250);       
}

void LedKedip (){
  delay(150);  
  digitalWrite(LED1B, HIGH);
  digitalWrite(LED2W, HIGH);
  digitalWrite(LED3G, HIGH);  
  delay(150);
  digitalWrite(LED1B, LOW);
  digitalWrite(LED2W, LOW);  
  digitalWrite(LED3G, LOW);  
  delay(150);

  delay(50);  
  digitalWrite(LED1B, HIGH);
  digitalWrite(LED2W, HIGH);
  digitalWrite(LED3G, HIGH);  
  delay(50);
  digitalWrite(LED1B, LOW);
  digitalWrite(LED2W, LOW);  
  digitalWrite(LED3G, LOW);  
  delay(50);

  delay(150);  
  digitalWrite(LED1B, HIGH);
  digitalWrite(LED2W, HIGH);
  digitalWrite(LED3G, HIGH);  
  delay(150);
  digitalWrite(LED1B, LOW);
  digitalWrite(LED2W, LOW);  
  digitalWrite(LED3G, LOW);  
  delay(150);
  
}
void SuhuBalance27(){
  float tempcek = dht.readTemperature();
  
  Serial.print("Temperature = ");
  Serial.print(tempcek); 
  Serial.println(" C");
  
  // Cek Suhu ke 27C 5x cek
  for(int i = 1; i < 6; i++){
    Serial.print("\n cek ke : ");
    Serial.print(i);
    delay (100);
    
  if (tempcek > 28.00) {
    irsend.sendRaw(rawData26c, 137,38); // if above 28C set to 26C @38khz
    Serial.println(F("Set to 26C"));     
    myBot.sendMessage(858195372, "Notif : Set to 26C");    // my id 858195372
    delay(500);
  }
  else if (tempcek < 26.00) {
    irsend.sendRaw(rawData28c, 137,38); // if below 26C set to 28C @38khz
    Serial.println(F("Set to 28C"));
    myBot.sendMessage(858195372, "Notif: Set to 28C"); // my id 858195372
    delay(500);
  }
 // delay(2*900000); // 30 mins
    delay(1*10000); // 10 sec
}

//---
}
