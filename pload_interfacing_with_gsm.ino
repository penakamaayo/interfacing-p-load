#include <SoftwareSerial.h>

#define ENTERKEY 'A'
#define NULLCHAR '\0'


SoftwareSerial mySerial(10,11);

char Rx_data[50];
int i = 0;

char phone_number[12];

char amount[3];
char num;
char digit;
int  iamount;

int piso_pin = 8;
int sinko_pin = 9;

char choice=' ';

void setup() {
  Serial.begin(38400);
  
  initGSM();
  send_msg("09067356103","welcome!");
  mySerial.begin(9600);
 
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

    //building msg
    char msg_to_server[20] = {'\0'};
    sprintf(msg_to_server, "LOAD:%s %s", phone_number, amount);
    
   
    //processing..
    waiting_time();
    
    //sending details to server
    //send_msg(String(phone_number),msg_to_server);
    send_msg("09067356103", msg_to_server);
    delay(5000);

    //mySerial.print("Done!");
    //delay(2000);
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


  for(int j=0; j<3;j++) {
    amount[j]='\0';
  }

  int i=0;
  while(i<12) {
    while(mySerial.available()<=0) {
    }
    num = mySerial.read();
    delay(200);

    if(num=='D') {
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
    while(mySerial.available() <=0) {}

    digit = mySerial.read();
    delay(200);

    if(digit=='D') {
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


void send_msg(char *number, char *msg) {
  char at_cmgs_cmd[30] = {'\0'};
  char msg1[160] = {'\0'};
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


