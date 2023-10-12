//Code to run oil cooler fans on Porsche 944
//Written by: John H.

//inputs
int temp_1 = 4;
int temp_2 = 7;
int temp_3 = 8;

//outputs
int fan_1 = 9;
int fan_2 = 10;
int fan_3 = 3;

//other globals
#define OFF   0    // defining our states
#define LOW   1     
#define MED   2
#define HIGH  3
int STATE = OFF;    // start in the OFF state

void setup() {

  Serial.begin(9600);

  pinMode(temp_1, INPUT);
  pinMode(temp_2, INPUT);
  pinMode(temp_3, INPUT);

  //enable outputs for Timer 1
  pinMode(fan_1,OUTPUT); //1A
  pinMode(fan_2,OUTPUT); //1B
  setupTimer1();
  //enable outputs for Timer 2
  pinMode(fan_3,OUTPUT); //2
  setupTimer2();
  //note that pin 11 will be unavailable for output in this mode!
  setPWM1A(0.0f); //set duty to 50% on pin 9
  setPWM1B(0.0f); //set duty to 20% on pin 10
  setPWM2(0.0f); //set duty to 80% on pin 3
  
}

void loop() {
  int temp = checkTemp();
  setSpeed(temp);

  Serial.println(STATE);

  switch (STATE) {
    case OFF: 
    setFans(0.0);
    break;

    case LOW: 
    setFans(0.2);
    break;

    case MED: 
    setFans(0.5);
    break;

    case HIGH:
    setFans(0.9);
    break;

    default:          // default case, do nothing and go back
      break;
  }

}

void setFans(float f){
  setPWM1A(f); 
  setPWM1B(f);
  setPWM2(f); 
}

void setSpeed(int temp){
  if(temp < 200){
    STATE = OFF;
  }
  else if(temp >= 200 && temp < 210){
    STATE = LOW;
  }
  else if(temp >= 210 && temp < 220){
    STATE = MED;
  }
  else{
    STATE = HIGH;
  }
}

int checkTemp(){
  int temp1 = analogRead(temp_1);    //Read the analog pin
  temp1 = temp1 * 0.48828125;   // convert output (mv) to readable celcius
  temp1 = (1.8 * temp1) + 32; //convert temp to F

  int temp2 = analogRead(temp_2);    //Read the analog pin
  temp2 = temp2 * 0.48828125;   // convert output (mv) to readable celcius
  temp2 = (1.8 * temp2) + 32; //convert temp to F

  int temp3 = analogRead(temp_3);    //Read the analog pin
  temp3 = temp3 * 0.48828125;   // convert output (mv) to readable celcius
  temp3 = (1.8 * temp3) + 32; //convert temp to F

  int avg = floor((temp1 + temp2 + temp3) / 3);

  return avg;
}

//configure Timer 1 (pins 9,10) to output 25kHz PWM
void setupTimer1(){
    //Set PWM frequency to about 25khz on pins 9,10 (timer 1 mode 10, no prescale, count to 320)
    TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
    TCCR1B = (1 << CS10) | (1 << WGM13);
    ICR1 = 320;
    OCR1A = 0;
    OCR1B = 0;
}
//configure Timer 2 (pin 3) to output 25kHz PWM. Pin 11 will be unavailable for output in this mode
void setupTimer2(){
    //Set PWM frequency to about 25khz on pin 3 (timer 2 mode 7, prescale 8, count to 79)
    TIMSK2 = 0;
    TIFR2 = 0;
    TCCR2A = (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
    TCCR2B = (1 << WGM22) | (1 << CS21);
    OCR2A = 79;
    OCR2B = 0;
}
//equivalent of analogWrite on pin 9
void setPWM1A(float f){
    f=f<0?0:f>1?1:f;
    OCR1A = (uint16_t)(320*f);
}
//equivalent of analogWrite on pin 10
void setPWM1B(float f){
    f=f<0?0:f>1?1:f;
    OCR1B = (uint16_t)(320*f);
}
//equivalent of analogWrite on pin 3
void setPWM2(float f){
    f=f<0?0:f>1?1:f;
    OCR2B = (uint8_t)(79*f);
}
