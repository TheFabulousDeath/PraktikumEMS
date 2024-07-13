

bool t1(), t2(), t3(), t4(), t5(), t6(), t7(), t8();

enum STATES {calibration, start, outside, inside, stairs, elevator};
STATES state;

void setup() {
  state = calibration;
}

void loop() {





  switch (state) {
    case start:
      if(t1()) {state = outside;}
      if(t2()) {state = inside;}
      break;
    case outside:
      if(t3()) {state = inside;}
      break;
    case inside:
      if(t4()) {state = outside;}
      if(t5()) {state = stairs;}
      if(t6()) {state = elevator;}
      break;
    case stairs:
      if(t8()) {state = inside;}
      break;
    case elevator:
      if(t7()) {state = inside;}  
      break;
  }
}

bool t1(){
  return true;
}

bool t2(){
  return true;
}
bool t3(){
  return true;
}
bool t4(){
  return true;
}
bool t5(){
  return true;
}
bool t6(){
  return true;
}
bool t7(){
  return true;
}
bool t8(){
  return true;
}
