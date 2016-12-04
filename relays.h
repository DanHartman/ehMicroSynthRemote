//////////////////////////////////////////////////
//           Control Mechanical Relays          //
//////////////////////////////////////////////////
void relay_init(void) {
	pinMode(RLY_1_PIN, OUTPUT);
  pinMode(RLY_2_PIN, OUTPUT);
  pinMode(RLY_3_PIN, OUTPUT);
  pinMode(RLY_4_PIN, OUTPUT);
  digitalWrite(RLY_1_PIN, LOW);
  digitalWrite(RLY_2_PIN, LOW);
  digitalWrite(RLY_3_PIN, LOW);
  digitalWrite(RLY_4_PIN, LOW);
}



boolean toggleEffect(boolean flag) {
  static boolean save = FALSE;
  if(flag) {
    save = !save;
    if(save) {
      digitalWrite(RLY_1_PIN, HIGH);
      digitalWrite(RLY_2_PIN, HIGH);
    }
    else {
      digitalWrite(RLY_1_PIN, LOW);
      digitalWrite(RLY_2_PIN, LOW);
    }
  }
  return save;
}

byte preset(byte temp) {
  static byte save = GROWL;
  if(temp) {
    save = temp;
    if(temp == GROWL) {
      digitalWrite(RLY_3_PIN, LOW);
      digitalWrite(RLY_4_PIN, LOW);
    }
    else if(temp == POINTY) {
      digitalWrite(RLY_3_PIN, HIGH);
      digitalWrite(RLY_4_PIN, HIGH);
    }
  }
	return save;
}

void checka(void) {
  if(wet_read(dry_read()))
    RGB_string(0, 25, 25, 0);
  else
    RGB_string(0, 0, 0, 0);
}

void stomp(void) {
  byte temp;
  
  if(allow()) {
    if(rel() == STAMPY) {
      set_inh();
      toggleEffect(TRUE);
    }
    if(hld() == STAMPY) {
      set_inh();
      temp = preset(READ);
      if(temp == GROWL)
        temp = POINTY;
      else if(temp == POINTY)
        temp = GROWL;
      preset(temp);
    }
  }
}

void main_xfer(void) {
  unsigned int temp;
  if(radio.available()) {
    bool done = false;
    while(!done) {
      done = radio.read(&temp, sizeof(unsigned int));
      Serial.write(temp);
    }
    radio.stopListening();
    radio.openWritingPipe(pipe);
    radio.write(&temp, sizeof(unsigned int));
    radio.openReadingPipe(1,pipe);
    radio.startListening();
    
    if(temp & 0x0001)
      while(toggleEffect(TRUE) != TRUE);
    else
      while(toggleEffect(TRUE) != FALSE);

    if((temp >> 8) == GROWL)
      preset(GROWL);
    else if((temp >> 8) == POINTY)
      preset(POINTY);
    
  }
}

void remote_xfer(void) {
  static boolean toggle = FALSE;
  static byte sound = GROWL;
  static unsigned int last = 0;
  unsigned int temp = 0;
  boolean flag = FALSE;

  //recover from a lost packet
  //the last information sent is saved until
  //the data returned matches.
  //indication will come from RED LEDs.
  //After a short period it will resend the 
  //packet.
  if(last) {
    RGB_string(0, 50, 0, 0);
    if(radio.available()) {
      bool done = false;
      while(!done) {
        done = radio.read(&temp, sizeof(unsigned int));
      }
      if(last == temp)
        last = 0;
      else
        flag = TRUE;
    }
    else if(timeout_counter(READ) > MILLISECONDS(50)) {
      timeout_counter(RESET);
      flag = TRUE;
    }
  }
  else
    timeout_counter(RESET);

  if((toggle != toggleEffect(READ)) || (sound != preset(READ))) {
    toggle = toggleEffect(READ);
    sound = preset(READ);
    last = (unsigned int)sound << 8;
    if(toggle)
      last = last | 0x0001;
    flag = TRUE;
  }

  if(flag) {
    radio.stopListening();
    radio.openWritingPipe(pipe);
    radio.write(&last, sizeof(unsigned int));
    radio.openReadingPipe(1,pipe);
    radio.startListening();
  }
}