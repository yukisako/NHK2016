#include <Wire.h>
#include <PS3USB.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

//paramators
#define LEN 16
#define DEVADD 8
#define DELAY 500  //interval of data connection.

//プロトタイプ宣言
void requestEvent();
char getLevel(uint8_t volume);
char getLevelButton(uint8_t volume);
void check();
void printButton();

int i; 
uint8_t TX_data[LEN]="000000000000000";



USB Usb;
PS3USB PS3(&Usb);

void setup() {
  Wire.begin(8);
  Wire.onRequest(requestEvent); 
  delay(DELAY);
  Serial.begin(9600);
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 USB Library Started"));
}


void loop() {

  Usb.Task();
 
  //オーバーフロー防止のためアナログ値は1/3に圧縮することとする
  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
    if (PS3.getAnalogHat(LeftHatX) > 137 || PS3.getAnalogHat(LeftHatX) < 117 ){
      TX_data[14] = (byte)getLevel(PS3.getAnalogHat(LeftHatX));
      //buttonDisc[lx] = PS3.getAnalogHat(LeftHatX);
    }

    if( PS3.getAnalogHat(LeftHatY) > 137 || PS3.getAnalogHat(LeftHatY) < 117 ){
      TX_data[15] = (byte)getLevel(PS3.getAnalogHat(LeftHatY));
    }

    if (PS3.getAnalogHat(RightHatX) > 137 || PS3.getAnalogHat(RightHatX) < 117 ){
      TX_data[12] = (byte)getLevel(PS3.getAnalogHat(RightHatX));
    }
    
    
      if( PS3.getAnalogHat(RightHatY) > 137 || PS3.getAnalogHat(RightHatY) < 117 ){
        TX_data[13] = (byte)getLevel(PS3.getAnalogHat(RightHatY));

      }
    }
    if (PS3.getAnalogButton(L2)){
      TX_data[11] = (byte)getLevelButton(PS3.getAnalogButton(L2));
    }      

    if ( PS3.getAnalogButton(R2)) {

      TX_data[10] = (byte)getLevelButton(PS3.getAnalogButton(R2));

    }

    /*f (PS3.getButtonClick(PS))*/


    if (PS3.getButtonPress(TRIANGLE)){
      TX_data[2] = '1';
    }else{
      TX_data[2] = '0';
    }

    if (PS3.getButtonPress(CIRCLE)){
      TX_data[0] = '1';
   }else{
      TX_data[0] = '0'; 
  }


    if (PS3.getButtonPress(CROSS)){
      TX_data[1] = '1';
      }else{
      TX_data[1] = '0';
    }
    
    if (PS3.getButtonPress(SQUARE)){
      TX_data[3] = '1';
      }else{
      TX_data[3] = '0';
    }


    if (PS3.getButtonPress(UP)) {
      TX_data[6] = '1';

      PS3.setLedOff();
      PS3.setLedOn(LED4);
      }else{
      TX_data[6] = '0';
    }
    if (PS3.getButtonPress(RIGHT)) {
      TX_data[4] = '1';


      PS3.setLedOff();
      PS3.setLedOn(LED1);
      }else{
      TX_data[4] = '0';
    }
    if (PS3.getButtonPress(DOWN)) {
      TX_data[5] = '1';


      PS3.setLedOff();
      PS3.setLedOn(LED2);
      }else{
      TX_data[5] = '0';
    }
    if (PS3.getButtonPress(LEFT)) {
      TX_data[7] = '1';

      PS3.setLedOff();
      PS3.setLedOn(LED3);
      }else{
      TX_data[7] = '0';
    }

    if (PS3.getButtonPress(L1)){
      TX_data[9] = '1';
      }else{
      TX_data[9] = '0';
    }

    /*if (PS3.getButtonClick(L3))*/

    if (PS3.getButtonPress(R1)){
      TX_data[8] = '1';
      }else{
      TX_data[8] = '0';
    }

    //Serial.print(TX_data[1]);
    /* if (PS3.getButtonClick(R3))
     
     
     
     if (PS3.getButtonClick(SELECT)) {
     
     
     }
     if (PS3.getButtonClick(START)) {
     
     
     
     }*/

  //AnalogControlについてはまた別に書く必要あり
  check();

}

void check(){
  int i = 0;

  printButton();
  
  while(i<LEN-1){
    //アスキーコードで'0'が48なので，48を引く
    Serial.print(TX_data[i]-48);
    i++;
  }
  delay(500);
}


void requestEvent()
{
  //Wire.beginTransmission(DEVADD);
  Wire.write(TX_data, 16);
  Serial.println("Call");
  //Wire.endTransmission();
}


//joystick
char getLevel(uint8_t volume){
  
  char output = 'z';
    

  if(volume >= 0 && volume < 49){
      output = '1';
      //Serial.println('%d', output);
  }else if(volume >= 49 && volume < 98){
      output = '2';
      //Serial.println('%d', output);
  }else if(volume >= 98 && volume < 154){
      output = '0';
      //Serial.println('%d', output);
  }else if(volume >= 154 && volume <205){
      output = '3';
      //Serial.println('%d', output);
  }else if(volume >= 205 && volume <= 255){
      output = '4';
      //Serial.println('%d', output);
  }
  
  return output;
}


//R2, L2
char getLevelButton(uint8_t volume){
  
  char output = 'z';
    

  if(volume >= 0 && volume < 49){
      output = '0';
      //Serial.println('%d', output);
  }else if(volume >= 49 && volume < 98){
      output = '1';
      //Serial.println('%d', output);
  }else if(volume >= 98 && volume < 154){
      output = '2';
      //Serial.println('%d', output);
  }else if(volume >= 154 && volume <205){
      output = '3';
      //Serial.println('%d', output);
  }else if(volume >= 205 && volume <= 255){
      output = '4';
      //Serial.println('%d', output);
  }
  
  return output;
}

void printButton(){
  Serial.println();
  Serial.println("ox34lbtrRL");
}



