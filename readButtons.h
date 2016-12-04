#include <Arduino.h>
///////////////////////////////////////////////////////////////////////////////                              //
//                   Debounces switches, allows for several                  //
//                         interactions with switches.                       //
//                -Closures  -Releases  -Holds  -Double Clicks               //
//                      See Example at bottom for usage.                     //
//                             Author: Dan Hartman                           //
//                                dan@hrtmn.org                              //
//                               Date 12-31-2013                             //
//                                                                           //
//        Example:  Inside the if(allow()) braces, the button presses        //
//   will not evalutate true unless all buttons were released.  This gives   //
//     a single increment of variable y with each press.  Outside of the     //
//braces the button presses will continue to be evaluated.  In this instance //
//the value y is incrmemnted continuously IF the correct button is held, and //
//  the increments will occur at the interval set by constant definitions.   //
//                                                                           //
//                    #define A_BUTTON PIN_A0                                //
//                    #define B_BUTTON PIN_A1                                //
//                    int y = 0;                                             //
//                                                                           //
//                    void main(void) {                                      //
//                      if(allow()) {                                        //
//                        if(cls() == A_BUTTON) {                            //
//                          set_inh();                                       //
//                          y++;                                             //
//                        }                                                  //
//                      }                                                    //
//                      if(hld() == B_BUTTON)                                //
//                        y++;                                               //
//                                                                           //
//                      printf("Y = %u\n", y);                               //
//                    }                                                      //
///////////////////////////////////////////////////////////////////////////////
void button_init(void);
byte dry_read(void);
byte wet_read(byte current);
void button_counters(void);
byte debounce_counter(byte flag);
unsigned int longpress_counter(byte flag);
byte shortpress_counter(byte flag);
unsigned int doubleclick_counter(unsigned int flag);
byte debounced_state(byte flag);
byte longpress_state(byte flag);
byte shortpress_state(byte flag);
byte doubleclick_state(byte flag);
boolean inhibit(byte flag);
void verify_longpress(byte temp);
void read_buttons(void);
byte cls(void);
byte rel(void);
byte hld(void);
boolean dbl(byte temp);
void set_inh(void);
boolean allow(void);
//////////////////////////////////////////////////
//           Pin and Timing Constants           //
//////////////////////////////////////////////////
#ifdef REMOTE_STOMP
  #define PIN_0 PIN_D3
#else
  #define PIN_0 PIN_D8
#endif

#define PIN_1 
#define PIN_2 


#define DEBOUNCE_LENGTH          MILLISECONDS( 35)
#define LONGPRESS_LENGTH         MILLISECONDS(750)
#define DOUBLE_CLICK_LENGTH      MILLISECONDS(275)
//////////////////////////////////////////////////
// Low Level Function To Read The Physical Pins //
//////////////////////////////////////////////////
void button_init(void) {
  pinMode(PIN_0, INPUT_PULLUP);
}

byte dry_read(void) {
  byte temp = 0;
  if(!digitalRead(PIN_0))
    bitSet(temp, 0);
    // if(!input(PIN_1))
    //   bit_set(temp, 1);
    // if(!input(PIN_2))
    //   bit_set(temp, 2);
  return temp;
}

//////////////////////////////////////////////////
//             Debounce Pin Movement            //
//////////////////////////////////////////////////
byte wet_read(byte current) {
  static byte last = 0;
  static byte temp = 0;
  if(current != temp) {
    temp = current;
    debounce_counter(RESET);
  }
  else if(debounce_counter(READ) > DEBOUNCE_LENGTH) {
    debounce_counter(RESET);
    last = current;
  }
    
  return last;
}

//////////////////////////////////////////////////
//    Counters Advanced from ISR based Tick     //
//////////////////////////////////////////////////
void button_counters(void) {
  debounce_counter(TRUE);
  longpress_counter(TRUE);
  doubleclick_counter(TRUE);
}

byte debounce_counter(byte flag) {
  static byte count = 0;
  count += flag;
  if(flag == RESET)
    count = 0;
  return count;
}

unsigned int longpress_counter(byte flag) {
  static unsigned int count = 0;
  count += flag;
  if(flag == RESET)
    count = 0;
  return count;
}

unsigned int doubleclick_counter(unsigned int flag) {
  static unsigned int count = DOUBLE_CLICK_LENGTH + 10;
  count += flag;
  if(flag == RESET)
    count = 0;
  return count;
}

//////////////////////////////////////////////////
//States Polled For Logical Operations Elsewhere//
//////////////////////////////////////////////////
byte debounced_state(byte flag) {
  static byte temp = 0;
  if(flag)
    temp = flag;
  if(flag == RESET)
    temp = 0;
  return temp;
}

byte longpress_state(byte flag) {
  static byte temp = 0;
  if(flag)
    temp = flag;
  if(flag == RESET)
    temp = 0;
  return temp;
}

byte shortpress_state(byte flag) {
  static byte temp = 0;
  if(flag)
    temp = flag;
  if(flag == RESET)
    temp = 0;
  return temp;
}

byte doubleclick_state(byte flag) {
  static byte temp = 0;
  if(flag)
    temp = flag;
  if(flag == RESET)
    temp = 0;
  return temp;
}

//////////////////////////////////////////////////
//                 Nitty Gritty                 //
//////////////////////////////////////////////////
boolean inhibit(byte flag) {
  static boolean temp = 1;
  if(flag)
    temp = 0;
  if(flag == RESET)
    temp = 1;
  return temp;
}

void verify_longpress(byte temp) {
  static byte save = 0;
  if(hld())
    longpress_state(RESET);
  if(longpress_counter(READ) > LONGPRESS_LENGTH) {
    longpress_state(temp);
    longpress_counter(RESET);
  }
  if(temp != save) {
    save = temp;
    longpress_counter(RESET);
  }
}

void hold_dbl_count(void) {
  doubleclick_counter(RESET);
  doubleclick_counter(DOUBLE_CLICK_LENGTH + 10);
}

boolean dbl_timeout(void) {
  boolean flag = 0;
  if(doubleclick_counter(READ) > DOUBLE_CLICK_LENGTH)
    flag = 1;
  return flag;
}

void read_buttons(void) {
  byte dry = dry_read();
  byte wet = wet_read(dry);

  if(!wet) {
    if(!rel())
      shortpress_state(cls());
    else {
      shortpress_state(RESET);
      inhibit(RESET);
    }
    debounced_state(RESET);
    longpress_state(RESET);
    longpress_counter(RESET);
  }
  if(dry) {
    debounced_state(wet);
    verify_longpress(wet);
  }
}

//////////////////////////////////////////////////
//                  BUTTON API                  //
//  The function read_buttons() is to be called //
//        repeatedly from the main loop.        //
//                                              //
//  Care should be taken that pins are properly //
//  defined, timing calculations and defintions //
//  are complete.  And that a uniform time base //
//       is created via a hardware timer.       //
//                                              //
//     Use cls(), rel(), etc to compare the     //
//       processed states with the desired      //
//                button combo.                 //
//                                              //
// Manually set inhibit(TRUE) to avoid repeated //
//  true evaluations.  inhibit() is reset when  //
//          all switches are released.          //
//////////////////////////////////////////////////
byte cls(void) {
  return debounced_state(READ);
}

byte rel(void) {
  //Returns value on switch release
  return shortpress_state(READ);
}

byte hld(void) {
  //Returns value on switch closure for X time
  return longpress_state(READ);
}

boolean dbl(byte temp) {
  //Returns true for two, quick, duplicate switch closures.
  //doubleclick_counter is intialized to, and assigned the value
  //of DOUBLE_CLICK_LENGTH + 1.  This is true UNTIL the 
  //close == desired button.  At this condition the 
  //doubleclick_counter is reset to 0 and allowed to increment
  //normally.  User will need to press the same button again,
  //before the timer gets past its cutoff to set match_true.  
  static short int ok, release = 0;
  static short int prev = 0;
  short int match_true = 0;
  byte close = cls();

  if((close == temp) && !prev && !ok) {
    doubleclick_counter(RESET);
    prev = 1;
  }

  if(dbl_timeout()) {
    hold_dbl_count();
    if(!close) {
      prev = 0;
      ok = 0;
    }
  }
  else if(prev) {
    if(!close)
      ok = 1;
    if(ok && close) {
      if(close == temp) {
        hold_dbl_count();
        prev = 0;
        match_true = 1;
      }
      else
        hold_dbl_count();
    }
  }

  return match_true;
}

void set_inh(void) {
  //Call this whenever one of the above
  //functions evaluates true if it is
  //desired to not allow repeat button actions
  //to be evaluated UNTIL end user releases all
  //buttons.
  inhibit(TRUE);
}

boolean allow(void) {
  //Wrap this function in an if statment
  //to segregate desired repeated or non-repeatable
  //button actions.  
  return inhibit(READ);
}