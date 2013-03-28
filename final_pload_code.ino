#include <SoftwareSerial.h>

#define ENTERKEY 'A'
#define ESCAPEKEY 'D'

SoftwareSerial mySerial(10,11);

char Rx_data[20];
int i = 0;

char phone_number[12];
char amount[3];
char num;
char digit;
int  iamount;

int piso_pin = 4;
int sinko_pin = 5;

char choice=' ';

void setup() {
  Serial.begin(38400);
  initGSM();
  send_msg("09067356103","greetings!");
  mySerial.begin(9600);
  delay(100);
  pinMode(piso_pin, INPUT);  
  pinMode(sinko_pin, INPUT);


}

void loop() {
  clear_screen();
  welcome_prompt();

  while(mySerial.available()<=0){
  }

  choice = mySerial.read();
  mySerial.print(choice);
  delay(500);

  if(choice==ENTERKEY){
    clear_screen();

    //get phone number
    get_number();

    //get amount to load
    get_amount();

    //process_message();

    if(!verify_info()){

    }

    int final_amount = insert_coin();
    //int final_amount=5;
    if(final_amount>4){
      //building msg
      char msg_to_server[20] = {
        '\0'      };
      sprintf(msg_to_server, "LOAD:%s %s", phone_number, amount);


      //processing..
      waiting_time();

      //sending details to server
      send_msg("09067356103", msg_to_server);
      delay(5000);
    }
  }
}

void clear_buffer(){
  //clear keys
  while(mySerial.available() >0) {
    mySerial.read();
  }
}

void get_number() {
  clear_buffer();
  clear_screen();
  mySerial.println("Press 'A' when done");
  mySerial.println("-------------------");
  mySerial.print("Enter Phone number: ");


  for(int j=0; j<12;j++) {
    phone_number[j]='\0';
  }

  int i=0;
  while(i<12) {
    while(mySerial.available()<=0) {
    }
    num = mySerial.read();
    delay(200);

    if(num==ESCAPEKEY) {
      --i;
      if (i<0) {
        i=0;
      }
      else {
        mySerial.print("\b");
      }

      phone_number[i] = '\0';
      continue;
    }

    if(num==ENTERKEY) {
      break;
    }

    if(!isDigit(num)) {
      continue;
    }

    if(i<11){
      phone_number[i] = num;
      mySerial.print(num);
      i++;
    }
  }
  delay(1000);
}

void get_amount() {
  clear_buffer();
  clear_screen();
  mySerial.println("Press 'A' when done");
  mySerial.println("-------------------");
  mySerial.print("Enter amount: ");

  //init amount;
  for(int j=0; j<3;j++) {
    amount[j]='\0';
  }

  iamount=0;
  int i=0;

  while(i<3) {
    while(mySerial.available() <=0) {
    }

    digit = mySerial.read();
    delay(200);

    if(digit==ESCAPEKEY) {
      --i;
      if (i<0) {
        i=0;
      }
      else {
        mySerial.print("\b");
      }

      amount[i] = '\0';
      continue;
    }

    if(digit==ENTERKEY) {
      break;
    }

    if(!isDigit(digit)) {
      continue;
    }

    if(i<2) {
      amount[i] = digit;
      mySerial.print(digit);
      i++;
    }
  }  

  amount[i]='\0';
  iamount = atoi(amount);
  delay(1000);
}

void waiting_time(){
  clear_screen();
  mySerial.println("");
  mySerial.print("Processing..        ");
  mySerial.print("Please wait");

  for(int i=0;i<4;i++){
    mySerial.print(".");
    delay(500);
  }
  clear_buffer();
}

int insert_coin(){
  clear_buffer();
  clear_screen();
  delay(500);
  int coin_counter = 0;

  mySerial.print("INSERT ");
  mySerial.print(iamount);
  mySerial.print(" PESO COIN");
  mySerial.println("  TO CONTINUE..    ");

  while(1){
    int piso_pulse = digitalRead(piso_pin);
    int sinko_pulse = digitalRead(sinko_pin);

    if(piso_pulse==HIGH) {
      coin_counter += 1;
      mySerial.print("\b");
      mySerial.print("\b");
      mySerial.print(coin_counter);
      delay(100);
    }

    if(sinko_pulse==HIGH) {
      coin_counter += 5;
      mySerial.print("\b");
      mySerial.print("\b");
      mySerial.print(coin_counter);
      delay(100);
    }

    if(coin_counter>=iamount){
      break;
    }

    //while(mySerial.available()<=0) {
    //}
    //char quit_choice = mySerial.read();

    //if(quit_choice==ESCAPEKEY) {
    //  break;
    //}
  }

  clear_screen();
  waiting_time();
  mySerial.print("DONE..");
  delay(1000);
  return coin_counter;
}


void clear_screen() {
  mySerial.print("\033E");
  delay(1000);
}

void welcome_prompt(){
  clear_screen();
  mySerial.print(" AUTOMATED LOADING  ");
  mySerial.print("       SYSTEM       ");
  mySerial.print("--------------------");
  mySerial.print("   A - To start :"   );
}

boolean verify_info(){
  char choice;
  clear_screen();
  mySerial.print("Number: ");
  mySerial.println(phone_number);
  mySerial.print("Amount: Php ");
  mySerial.println(amount);
  mySerial.print("Press A to Continue ");
  mySerial.print("Press D to Cancel "); 

  clear_buffer() ;

  while(mySerial.available() <=0) { 
  }
  choice = mySerial.read();
  delay(500);
  mySerial.print(choice);

  if(choice==ENTERKEY){
    return true;
  }
  return false;
}


//------------------------
//GSM functions
//------------------------

void send_msg(char *number, char *msg) {
  char at_cmgs_cmd[30] = {
    '\0'          };
  char msg1[20] = {
    '\0'          };
  char ctl_z = 0x1A;

  sprintf(msg1, "%s%c", msg, ctl_z);
  sprintf(at_cmgs_cmd, "AT+CMGS=\"%s\"\r\n",number);

  sendGSM(at_cmgs_cmd);
  delay(1000);

  sendGSM(msg1);
  delay(2000);
}



void sendGSM(char *string){
  //Serial.write(string);
  Serial.print(string);
  delay(100);
}

void clearString(char *strArray) {
  int j;
  for (j = 100; j > 0; j--)
    strArray[j] = 0x00;
}

void send_cmd(char *at_cmd, char clr){
  char *stat = '\0';
  while(!stat){
    sendGSM(at_cmd);
    delay(100);
    readSerialString(Rx_data);

    stat = strstr(Rx_data, "OK");
  }
  if (clr){
    clearString(Rx_data);
    delay(200);
    stat = '\0';
  }
}

void initGSM(){

  send_cmd("AT\r\n",1);           
  send_cmd("ATE0\r\n",1);// Turn off automatic echo of the GSM Module 

  send_cmd("AT+CMGF=1\r\n",1);// Set message format to text mode
  //Sucess

  delay(1000);
  delay(1000);
  delay(1000);
}

void readSerialString (char *strArray) {

  if(!Serial.available()) {
    return;
  }

  while(Serial.available()) {
    strArray[i] = Serial.read();
    i++;
  }
}

