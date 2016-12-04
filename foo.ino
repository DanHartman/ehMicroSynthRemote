//////////////////////////////////////////////////
//               Dan BAMF Hartman               //
//            EH Micro Synth Remote             //
//                                              //
//             DEVICE: Arduino Nano             //
//              AUTHOR: Dan Hartman             //
//           CONTACT: dan.h@rtman.org           //
//           PRODUCTION DATE: 11/10/14          //
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//                Compile Options               //
//////////////////////////////////////////////////
// #define REMOTE_STOMP TRUE
//////////////////////////////////////////////////
//                 Dependencies                 //
//////////////////////////////////////////////////
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "header.h"

void display(byte temp, boolean up) {
  static boolean flag = FALSE;
  static boolean presetColor = FALSE;
  byte r = temp; 
  byte g = temp; 
  byte b = temp;
  temp = preset(READ);
  //sees the falling half
  //of the LED pulse.  setting
  //trigger to switch to second color
  if(!up && !flag)
    flag = TRUE;
  
  //trigger set, use second
  //color and reset snare
  if(flag && up) {
    flag = FALSE;
    presetColor = !presetColor;
  }

  //rgb values default to a white color
  //unless we are on the even numbered
  //LED pulse as indicated by presetColor
  //or we've got the damnded effect turned
  //on.  Then we see the color baby!
  if(presetColor || toggleEffect(READ)) {
    if(temp == GROWL) {
      b = 0;
    }  
    else {
      r = 0;
    }
  }

  RGB_string(0, r, g, b);
}



void bar(void) {
  boolean flag = toggleEffect(READ);
  int temp = MILLISECONDS(10);
  
  static int save = 0;
  static boolean up = TRUE;
  static boolean color = TRUE;


  if(flag)
    temp = MILLISECONDS(4);
  if(counter(READ) > temp) {
    counter(RESET);
    if(up) {
      if(save < 100)
        save += 1;
      else
        up = FALSE;
    }
    else {
      if(save > 20)
        save -= 1;
      else {
        up = TRUE;
      }
    }
    display(save, up);
  }
  
  stomp();
  #if REMOTE_STOMP
    remote_xfer();
  #else
    main_xfer();
  #endif
}

void setup(void) {
  // UART
  Serial.begin(9600);

  // nRF24L01
  pinMode(10, OUTPUT);
  radio.begin();
  radio.openReadingPipe(1, pipe);
  radio.startListening();

  relay_init();

  button_init();

  ws_init();

  // main "switch"
  pFunk = bar;
}


void loop(void) {
  time_base();
  read_buttons();
  (*pFunk)();
}