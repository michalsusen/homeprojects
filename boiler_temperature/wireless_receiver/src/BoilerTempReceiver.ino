#include <VirtualWire.h>
#include <MsTimer2.h>

const int receive_pin = 12;

//electrodes a,b,c...DP
int segments[] = {2,3,4,5,6,7,8,9};

int characters[] = {
//B0xxxxxxx <- MSB not used, segments "gfedcba"
B01000000,//0
B01111001,//1
B00100100,//2
B00110000,//3
B00011001,//4
B00010010,//5
B00000010,//6
B01111000,//7
B00000000,//8
B00010000,//9
};


//segments pin
int digits[] = {10,11};
int currentDigit = 0;
int maxDigits = 2;
int temperature = 0;

uint8_t buf[VW_MAX_MESSAGE_LEN];
uint8_t buflen = VW_MAX_MESSAGE_LEN;

void setup()
{
    for(int i=0;i<8;i++) 
      pinMode(segments[i], OUTPUT); 
    for(int i=0;i<maxDigits;i++) 
      pinMode(digits[i], OUTPUT); 
    
    vw_set_rx_pin(receive_pin); 
    vw_setup(2000);	 
    vw_rx_start();       
   
    MsTimer2::set(1, changeSegment); 
    MsTimer2::start();  
}

void loop()
{
    if (vw_get_message(buf, &buflen)) 
    {
      temperature = buf[0];
    }
}

void changeSegment()
{
  int i,number = 0;
  //turn off all digits
  for(i=0;i<maxDigits;i++)
    digitalWrite(digits[i],LOW);
  
  if(currentDigit==0) 
    number = temperature%10;
  else if(currentDigit==1)
  {
    number = temperature/10;
    if(number>9) number=9;
  }
  else number=0;
  
  //set up characters
  for(i=0;i<7;i++)
    digitalWrite(segments[i],characters[number] & (1 << i));
    
  //turn on one of digit
  digitalWrite(digits[currentDigit],HIGH);
  currentDigit++;
  
  //check overflow digit
  if(currentDigit>=maxDigits)
    currentDigit=0;
}

