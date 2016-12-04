//////////////////////////////////////////////////
//                   Constants                  //
//////////////////////////////////////////////////
#define READ      0x00
#define RESET     0xFF
#define TRUE         1
#define FALSE        0

// Specific to the nRFL24 modules and driver
#define CE_PIN   9
#define CSN_PIN 10

// "Preset" names for the pedal
#define GROWL   0xA0
#define POINTY  0xA1

// Button permutations
#define STAMPY  0x01
//////////////////////////////////////////////////
//                Macro Machines                //
//////////////////////////////////////////////////
#define MILLISECONDS(value) value
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
//////////////////////////////////////////////////
//              Nano Board Pinouts              //
//////////////////////////////////////////////////
#define PIN_D2       2
#define PIN_D3       3
#define PIN_D4       4
#define PIN_D5       5
#define PIN_D6       6
#define PIN_D7       7
#define PIN_D8       8
#define PIN_D9       9
#define PIN_D10     10
#define PIN_D11     11
#define PIN_D12     12
#define PIN_D13     13
#define PIN_A0      14
#define PIN_A1      15
#define PIN_A2      16
#define PIN_A3      17
#define PIN_A4      18
#define PIN_A5      19
#define PIN_A6
#define PIN_A7

#define RLY_1_PIN  PIN_D4
#define RLY_2_PIN  PIN_D5
#define RLY_3_PIN  PIN_D6
#define RLY_4_PIN  PIN_D7
//////////////////////////////////////////////////
//                 Dependencies                 //
//////////////////////////////////////////////////
#include "readButtons.h"
#include "global.h"
#include "ws2812b.h"
#include "animate.h"
// #include "main.h"
#include "relays.h"