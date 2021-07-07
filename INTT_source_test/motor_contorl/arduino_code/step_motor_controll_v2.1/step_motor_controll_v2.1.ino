const int PUL = 8; //define Pulse pin
const int DIR = 7; //define Pulse pin
const int ENA = 4; //define Enable pin
const int time_mea = 3000;
int steps_gap_A = 10;
int steps_gap_B = 10;
int AB_gap = 20;


void setup() {
  Serial.begin(9600); 
  pinMode (PUL, OUTPUT);
  pinMode (DIR, OUTPUT);
  pinMode (ENA, OUTPUT);

  //calibration();

  Serial.println(" ");
  Serial.println("=================");
  Serial.println("Features: ");
  Serial.println("- standard");
  Serial.println("- single");
  Serial.println("- free");
  //Serial.println("- calibration");
  Serial.println("- help");
  Serial.println("=================");
  
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

  while(Serial.available() > 0){
    char command = Serial.read();
    if (command != '\n'){
      mode_message[mode_message_pos] = command;
      mode_message_pos++;
      if(mode_message_pos >= 255){
        mode_message_pos = 254;
      }
    }
    else{
      Serial.println("Recieved.");
      Serial.read();
      mode_message[mode_message_pos] = '\0';
      mode_message_pos = 0;
      checkpoint = true;
    }
  }

  delay(100);

  if(checkpoint = true){

    //************* standard mode ************
    if(strcmp(mode_message, "standard") == 0){
      bool done = false;
      bool done_changedir = false;
      
      Serial.println("Running standard mode...");
      
      //delay(time_mea);
      for(int s=1; s<5; s++){
        Serial.print("Now running ID: ");
        Serial.println(s);
        delay(time_mea);
        motor_steps(steps_gap_A);
      }
      Serial.println("Now running ID: 5");
      delay(time_mea);            // measure for id = 5
      motor_steps(AB_gap);            // A, B gap
      for(int s=6; s<13; s++){
        Serial.print("Now running ID: ");
        Serial.println(s);
        delay(time_mea);
        motor_steps(steps_gap_B);
      }
      Serial.println("Now running ID: 13");
      delay(time_mea);            // measure for id = 13

      Serial.println("Now changing the direction");
      delay(10000);               // time for change the direction
      Serial.println("Done?");

      if(done == false){           //check whether changing the direction are done
        int i = 0;
        Serial.read();
        while(true){
          if (Serial.available() > 0){
            done_changedir = true;
            break;
          }
          delay(1000);
          i++;
          if(i%10 == 0){Serial.println("Done?");}
        }
      }

      if(done_changedir == true){
        for(int s=14; s<21; s++){
          Serial.print("Now ring ID: ");
          Serial.println(s);
          delay(time_mea);
          motor_steps(-steps_gap_B);
        }
        Serial.println("Now running ID: 21");
        delay(time_mea);            // measure for id = 21
        motor_steps(AB_gap);         // B, A gap
        for(int s=22; s<26; s++){
          Serial.print("Now running ID: ");
          Serial.println(s);
          delay(time_mea);
          motor_steps(-steps_gap_A);
        }
        Serial.println("Now running ID: 26");
        delay(time_mea);            // measure for id = 26
        done = true;
      }

      if(done == true){
        Serial.println("-----------------------");
        Serial.println("Standard Test Finished");
        Serial.println("-----------------------");
        checkpoint = false;
      }
    }

    //************* single mode **************
    if (strcmp(mode_message, "single") == 0){
      bool done = false;
      bool ID_read = false;
      bool ID_trans = false;
      bool done_move = false;

      char ID_message[16];
      int ID_message_pos = 0;
      
      Serial.println("Running single mode...");
      Serial.println("Entering the ID: ");
      Serial.read();
      while(done == false){
        
        while(Serial.available() > 0 && ID_read == false){
          char command = Serial.read();
          if (command != '\n'){     
            ID_message[ID_message_pos] = command;
            ID_message_pos++;
          }
          else{
            Serial.read();
            ID_message[ID_message_pos] = '\0';
            ID_message_pos = 0;
            ID_read = true;
          }
        }
        delay(100);

        if(ID_read == true){
          if(isAlpha(ID_message[0]) || isAlpha(ID_message[1])){
            done = true;
            break;
          }
          else{
            ID = atoi(ID_message);
            Serial.print("ID:");
            Serial.println(ID);
            ID_trans = true;
          }
          Serial.read();
          ID_read = false;
        }
        delay(100);
        
        if(ID_trans == true){
          if(ID <= 26 && ID >= 1){
            if(ID >= 14 && ID <= 26){ID = ID - 13;}
            if((ID <= 5 && ID_pre > 5) || (ID_pre <= 5 && ID > 5)){
              Serial.print("running steps(with gap): ");
              Serial.println(ID - ID_pre);
              if(ID <= 5){
                motor_steps(steps_gap_B*(ID - 5)+ AB_gap + steps_gap_A*(ID_pre - 6));
                delay(100);
              }
              else{
                motor_steps(steps_gap_A*(ID - 6)+ AB_gap + steps_gap_B*(5 - ID_pre));
                delay(100);
              }
            }
            else{
              Serial.print("running steps: ");
              Serial.println(ID - ID_pre);
              if(ID <= 5){
                motor_steps(steps_gap_B*(ID - ID_pre));
                delay(100);
              }
              else{
                motor_steps(steps_gap_A*(ID - ID_pre));
                delay(100);
              }
            }
          }
          else{
            Serial.println("No such ID!");
            break;
          }
          done_move = true;
          Serial.println("done");
        }
        delay(100);

        if(done_move == true){
          ID_pre = ID;
          memset(ID_message, 0, 255);
          ID_trans = false;
          ID_read = false;
          done_move = false;
          Serial.print("id_pre:");
          Serial.println(ID_pre);
          Serial.println("Entering ID:");
        } 
      }
      delay(100);

      if(done == true){
        Serial.println("-----------------------");
        Serial.println("Single Test Finished");
        Serial.println("-----------------------");
        //memset(ID_message, 0, 255);
        checkpoint = false;
      }
    }
    
    //************* free mode ****************
    if (strcmp(mode_message,"free") == 0){
      bool done = false;
      bool done_step = false;
      bool done_time = false;
      bool step_check = false;
      bool time_check = false;
      bool timeout = false;
      int steps = 0;
      int times = 0;   
      
      Serial.println("Running free mode...");

      if(done == false){
        Serial.println("Entering the steps:");
        int i = 0;
        Serial.read();
        while(true){
          if (Serial.available() > 0){
            steps = Serial.parseInt();
            Serial.print("Runs for ");
            Serial.print(steps);
            Serial.println(" steps");
            step_check = true;
            break;
          }
          delay(1000);
          if(i%5 == 0){Serial.print("...");}
          i++;      
          if(i>=15){
            timeout = true;
            break;
          }
        }
      }

      if(step_check == true){
        motor_steps(steps);
        done_step = true;        
      }

      if(done_step == true){
        Serial.println("Entering the measuring times: (sec)");

        int i = 0;
        Serial.read();
        while(true){
          if (Serial.available() > 0){
            times = Serial.parseInt();
            Serial.print("Measuring for ");
            Serial.print(times);
            Serial.println("s");
            time_check = true;
            break;
          }
          delay(1000);
          if(i%5 == 0){Serial.print("...");}
          i++;      
          if(i>=15){
            timeout = true;
            break;
          }
        }
      }

      if(time_check == true){
        times = abs(times*1000);
        if(times < 300000){
          delay(times);
          done_time = true;
        }
        else{
          Serial.println("It's too long, measure manually!");
        }
      }

      if(done_time == true){
        int i = 0;
        Serial.read();
        while(true){
          if(Serial.available() > 0){
            done = true;
            break;
          }
          i++;
          if(i%10 == 0){Serial.println("ok?");}
          delay(1000);
        }
      }

      if(timeout == true){
        int i = 0;
        while(true){
          if(Serial.available() > 0){
            done = true;
            break;
          }
          delay(1000);
          i++;
          if(i%10 == 0){Serial.println("Timeout now, enter ok to continue.");}
        }
      }

      if(done == true){
        Serial.println("-----------------------");
        Serial.println("Free mode Test Finished");
        Serial.println("-----------------------");
        checkpoint = false;
      }
    }

    //************* calibration mode *************
    if (strcmp(mode_message,"calibration") == 0){
      bool done = false;

      if(done == false){
        Serial.read();
        calibration();
        done = true;
      }

      if(done = true){
        checkpoint = false;
      }
    }


    //**************** help *******************
    if (strcmp(mode_message,"help") == 0){
  
      Serial.println("=================");
      Serial.println("Features: ");
      Serial.println("- standard");
      Serial.println("- single");
      Serial.println("- free");
      //Serial.println("- calibration");
      Serial.println("- help");
      Serial.println("=================");
      checkpoint = false;  
    }
  }

  if(checkpoint == false){
    Serial.read();
    memset(mode_message, 0, 255);
    Serial.println("flush");
  }
}

void calibration(){       // not finish yet
  bool done = false;
  bool start_check = false;
  bool step_check = false;
  bool done_start = false;
  bool timeout = false;
  int steps = 0;
  
  Serial.println("Now doing calibration: ");
  Serial.println("===========================================");
  Serial.println("Find the start position: ");
  //motor_steps_for(10000);
  Serial.println("Entering the steps: ");

  if(done == false){
    int i = 0;
    while(true){
      if(Serial.available() > 0){
        steps = Serial.parseFloat();
        Serial.print("Running ");
        Serial.print(steps);
        Serial.println(" steps");
        start_check = true;
      }
      delay(1000);
      i++;
      if(i%10 == 0){Serial.print("...");}
    }

    if(step_check == true){
      motor_steps(steps);
      Serial.println("Start position -> Done");
      Serial.println("*******************************************");
      steps = 0;
      done_start = true;
    }

    if(done_start == true){
      Serial.println("Calibrate the steps gap: ");
      motor_steps(steps_gap_A);
      Serial.println("Entering the steps: ");

      int i = 0;
      Serial.read();
      while(true){
        if (Serial.available() > 0){
          steps = Serial.parseFloat();
          Serial.println("steps");
          step_check = true;
        }

        if(step_check == true){
          if(steps != 0){
            motor_steps(steps);
          }
          if(steps == 0){
            Serial.println("Step gap -> Done");
            Serial.println("*******************************************");
            done = true;
          }
        }
        delay(1000);
        if(i%5 == 0){Serial.print("...");}
        i++;      
        if(i>=16){
          timeout = true;
          break;
        }
      }
    }
  }
}

void motor_steps(int loops){  
  if(loops >= 0){
    for(int j=0; j<loops; j++){
      for (int i=0; i<6400; i++) {
        digitalWrite (DIR, LOW);
        digitalWrite (ENA, HIGH);
        digitalWrite (PUL, HIGH);
        delayMicroseconds (10);
        digitalWrite (PUL, LOW);
        delayMicroseconds (10);
      }
    }
  }else{
    loops = abs(loops);
      for(int j=0; j<loops; j++){
        for(int i=0; i<6400; i++){
          digitalWrite (DIR, HIGH);
          digitalWrite (ENA, HIGH);
          digitalWrite (PUL, HIGH);
          delayMicroseconds (10);
          digitalWrite (PUL, LOW);
          delayMicroseconds (10);
        }
     }
  }
}
