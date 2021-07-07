const int PUL = 8; //define Pulse pin
const int DIR = 7; //define Pulse pin
const int ENA = 4; //define Enable pin
const int time_mea = 1000;
int steps_gap_A = 10; // (cycles)
int steps_gap_B = 10; // (cycles)
int AB_gap = 10;      // (cycles)


void setup() {
  Serial.begin(9600); 
  pinMode (PUL, OUTPUT);
  pinMode (DIR, OUTPUT);
  pinMode (ENA, OUTPUT);

  Serial.println(" ");
  Serial.println("=================");
  Serial.println("Features: ");
  Serial.println("- standard");
  Serial.println("- single");
  Serial.println("- free");
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
      Serial.read();
      mode_message[mode_message_pos] = '\0';
      mode_message_pos = 0;
      checkpoint = true;
    }
  }

  delay(100);

  if(checkpoint == true){

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
      delay(time_mea);                                // measure for id = 5
      motor_steps(AB_gap);                            // A, B gap
      for(int s=6; s<13; s++){
        Serial.print("Now running ID: ");
        Serial.println(s);
        delay(time_mea);
        motor_steps(steps_gap_B);
      }
      Serial.println("Now running ID: 13");
      delay(time_mea);                                // measure for id = 13

      Serial.println("Now changing the direction");
      delay(5000);                                   // time for change the direction
      Serial.println("Done?");

      if(done == false){                              //check whether changing the direction are done
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
          Serial.print("Now runing ID: ");
          Serial.println(s);
          delay(time_mea);
          motor_steps(-steps_gap_B);
        }
        Serial.println("Now running ID: 21");
        delay(time_mea);                        // measure for id = 21
        motor_steps(-AB_gap);                   // B, A gap
        for(int s=22; s<26; s++){
          Serial.print("Now running ID: ");
          Serial.println(s);
          delay(time_mea);
          motor_steps(-steps_gap_A);
        }
        Serial.println("Now running ID: 26");
        delay(time_mea);                        // measure for id = 26
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
      bool ID_next = false;
      bool ID_read = false;
      bool ID_trans = false;
      bool done_move = false;

      char ID_message[16];
      int ID_message_pos = 0;
      
      Serial.println("Running single mode...");
      Serial.println("Entering the ID: ");
      Serial.read();
      while(true){
        
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
                motor_steps(-(steps_gap_B*(ID - 5)+ AB_gap + steps_gap_A*(ID_pre - 6)));
                delay(100);
              }
              else{
                motor_steps((steps_gap_A*(ID - 6)+ AB_gap + steps_gap_B*(5 - ID_pre)));
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
        }
        delay(100);

        if(done_move == true){       
          ID_pre = ID;
          delay(100);

          Serial.print("id_pre:");
          Serial.println(ID_pre);
          delay(100);

          ID_next = true;
        }
        delay(100);

        if(ID_next == true){
          Serial.println("Entering next ID:");
  
          int i = 0;
          Serial.read();
          while(true){
            if (Serial.available() > 0){
              ID_read = false;
              break;
            }
            delay(1000);
            if(i%5 == 0){Serial.print("...");}
            i++;
          }
        }
        
      }
      
      delay(100);

      if(done == true){
        Serial.println("-----------------------");
        Serial.println("Single Test Finished");
        Serial.println("-----------------------");
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
      float times = 0;   
      
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
          if(i>=25){
            timeout = true;
            break;
          }
        }
      }
      delay(100);

      if(step_check == true){
        motor_steps(steps);
        done_step = true;  
      }
      delay(100);

      if(done_step == true){
        Serial.println("Entering the measuring times: (min)");

        int i = 0;
        Serial.read();
        while(true){
          if (Serial.available() > 0){
            times = Serial.parseFloat();
            Serial.print("Measuring for ");
            Serial.print(times);
            Serial.println("mins");
            time_check = true;
            break;
          }
          delay(1000);
          if(i%5 == 0){Serial.print("...");}
          i++;      
          if(i>=25){
            timeout = true;
            break;
          }
        }
      }
      delay(1);

      if(time_check == true){
        if(times <= 5 && times >= 0){
          delay((times*60*1000));
          done_time = true;
        }
        else{
          Serial.println("Measures manually!");
        }
      }
      delay(10);

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
        Serial.println("Timeout now, enter ok to continue.");
        int i = 0;
        while(true){
          if(Serial.available() > 0){
            done = true;
            break;
          }
          delay(1000);
          i++;
          if(i%10 == 0){done = true;}
        }
      }

      if(done == true){
        Serial.println("-----------------------");
        Serial.println("Free mode Test Finished");
        Serial.println("-----------------------");
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
      Serial.println("- help");
      Serial.println("=================");
      checkpoint = false;  
    }
  }

  /*
  if(checkpoint == false){
    Serial.read();
    memset(mode_message, 0, 255);
    Serial.println("flush");
  }
  */
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
