#include <Arduino.h>
//////////////////////////////////////////////////
//                  Prototypes                  //
//////////////////////////////////////////////////
void (*pFunk)();
unsigned int counter(byte temp);
unsigned int timeout_counter(byte temp);
void time_base(void);
void indc_off(void);
void indc_on(void);

//////////////////////////////////////////////////
//               Global Functions               //
//////////////////////////////////////////////////
unsigned int counter(byte temp) {
	static unsigned int save = 0;

	if(temp)
	  save += 1;
	if(temp == RESET)
	  save = 0;
	return save;
}
unsigned int timeout_counter(byte temp) {
  static unsigned int save = 0;

  if(temp)
    save += 1;
  if(temp == RESET)
    save = 0;
  return save;
}

void time_base(void) {
	unsigned long tock = millis();
	static unsigned long last = 0;
  boolean increment = (tock - last);
  boolean roll_over = (tock == 0);

  if(roll_over)
    last = 0;
  if(increment) {
  	last = tock;
    counter(TRUE);
    timeout_counter(TRUE);
    button_counters();
  }
}

void indc_off(void) {
  digitalWrite(13, LOW);
  pinMode(13, OUTPUT);
}

void indc_on(void) {
	digitalWrite(13, HIGH);
  pinMode(13, OUTPUT);
}