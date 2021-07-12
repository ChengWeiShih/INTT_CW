const int PUL = 8; //define Pulse pin
const int DIR = 7; //define Pulse pin
const int ENA = 4; //define Enable pin
const int time_mea = 2000;
int steps_gap_A = 100; // (cycles)
int steps_gap_B = 8; // (cycles)
int AB_gap = 15;      // (cycles)


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  pinMode (PUL, OUTPUT);
  pinMode (DIR, OUTPUT);
  pinMode (ENA, OUTPUT);

}

int l = 0;  
char mode_message[255];
unsigned int mode_message_pos = 0;
bool newdata = true;
bool flushing = true;
bool checkpoint = false;
int ID = 0;
int ID_pre = 0;



void loop() {
  // put your main code here, to run repeatedly:

  while (true)
  {
      motor_steps(steps_gap_A);
  }

}


void motor_steps(int loops){  
  if(loops >= 0){
    for(int j=0; j<loops; j++){
      for (int i=0; i<6400; i++) {
        digitalWrite (DIR, LOW);
        digitalWrite (ENA, HIGH);
        digitalWrite (PUL, HIGH);
        delayMicroseconds (50);
        digitalWrite (PUL, LOW);
        delayMicroseconds (50);
      }
    }
  }else{
    loops = abs(loops);
      for(int j=0; j<loops; j++){
        for(int i=0; i<6400; i++){
          digitalWrite (DIR, HIGH);
          digitalWrite (ENA, HIGH);
          digitalWrite (PUL, HIGH);
          delayMicroseconds (50);
          digitalWrite (PUL, LOW);
          delayMicroseconds (50);
        }
     }
  }
}
