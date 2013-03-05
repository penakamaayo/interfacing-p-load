#include <SoftwareSerial.h>
#define ENTERKEY 'A'

SoftwareSerial mySerial(10,11);


String phone_number="";
char amount[3];
char num;
char digit;
int  iamount;

void setup() {
  mySerial.begin(9600);
  delay(500);
}

void loop() {
    char choice=' ';
    
    clear_buffer();
    welcome_msg();
    while(mySerial.available()<=0){}
    
    choice = mySerial.read();
    if(choice==ENTERKEY){
      clear_screen();
            
      //-----------------------------------
      phone_number = get_number();
      delay(1000);
      //-----------------------------------
      
      //-----------------------------------
      get_amount();
      delay(1000);
      //-----------------------------------      
      
      //----------------------------------
       waiting_time();
      //---------------------------------

      if(!verify_info()){
     
      }
      
      clear_screen();
      mySerial.print(iamount);
      delay(5000);
    }
}



//----------------------------------------
//         USER-DEFINED FUNCTIONS
//----------------------------------------

void clear_screen(){
  mySerial.print("\033E");
  delay(1000);
}

void welcome_msg(){
  clear_screen();
  mySerial.print(" AUTOMATED LOADING  ");
  mySerial.print("       SYSTEM       ");
  mySerial.print("--------------------");
  mySerial.print("   A - To start :"   );
}


String get_number(){
  phone_number="";
  clear_buffer();
  clear_screen();   
  mySerial.print("Enter Phone number: ");
  int i=0;
   
  while(i<11){
    while(mySerial.available()<=0){}
    num = mySerial.read();
    delay(200);
     
    if(num=='D'){
      --i;
      if (i<0) {
        i=0;
      }
      else{
        mySerial.print("\b");
      }
      
      phone_number[i] = '\0';
      continue;
    }
    
    if(!isDigit(num)){
      continue;
    }
    
    phone_number += num;
    mySerial.print(num);
    i++;
  }
  
  return phone_number;
}

void get_amount(){
  clear_buffer();
  clear_screen();
  mySerial.println("Press 'A' when done");
  mySerial.println("-------------------");
  mySerial.print("Enter amount: ");
 
  //init amount;
  for(int j=0; j<3;j++){
    amount[j]='\0';
  }

  iamount=0;
  int i=0;
  
  while(i<3){
    
    while(mySerial.available() <=0) { }
    digit = mySerial.read();
    delay(200);

    if(digit=='D'){
      --i;
      if (i<0) {
        i=0;
      }
      else{
        mySerial.print("\b");
      }
      
      amount[i] = '\0';
      continue;
    }
    
    if(digit==ENTERKEY){
      // return amount;
      break;
    }
    
    if(!isDigit(digit)){
      continue;
    }
  
    if(i<2){
      amount[i] = digit;
      mySerial.print(digit);
      i++;
    }
  }  
    
  amount[i]='\0';
  iamount = atoi(amount);
}

void waiting_time(){
  clear_screen();
  mySerial.println("");
  mySerial.print("Processing..        ");
  mySerial.print("Please wait...");
  delay(1000);
  
  clear_screen();
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

   // // clear keys
   // while(mySerial.available() >0) {
   //   mySerial.read();
   // }

  clear_buffer() ;

   while(mySerial.available() <=0) { }
   choice = mySerial.read();
   
   mySerial.print(digit);
   
   if(choice==ENTERKEY){
      return true;
   }
   return false;
}

void clear_buffer(){
   //clear keys
  while(mySerial.available() >0) {
    mySerial.read();
  }
}
