#include "mbed.h"
#include "QEI.h"
#include <cstdlib>
#include <iostream>

Serial pc(USBTX, USBRX);
//Use X4 encoding.
//QEI wheel(p29, p30, NC, 624, QEI::X4_ENCODING);
//Use X2 encoding by default.
QEI wheel (PA_1, PA_4, NC, 500);

I2C i2c( PB_3 , PB_10 );  //通信用のピン設定(SCA,SCL)

const int addr = 0x08;
char data[17] = "zzzzzzzzzzzzzzzz";
int read_ack;
char buf = '0';
int count = 0;

int i = 0;




void cycle(){
  pc.printf("丸ボタンが押されています．\r\n");
}

void triangle(){
  pc.printf("三角ボタンが押されています．\r\n");
}

void cross(){
  pc.printf("バツボタンが押されています．\r\n");
}

void square(){
  pc.printf("四角ボタンが押されています．\r\n");
}

void up(){
  pc.printf("十字キーの上ボタンが押されています．\r\n");
}

void down(){
  pc.printf("十字キーの下ボタンが押されています．\r\n");
}

void left(){
  pc.printf("十字キーの左ボタンが押されています．\r\n");
}

void right(){
  pc.printf("十字キーの右ボタンが押されています．\r\n");
}

void r_1(){
  pc.printf("R1ボタンが押されています．\r\n");
}

void l_1(){
  pc.printf("L1ボタンが押されています．\r\n");
}

void press_button(){
  if (data[0]=='1') cycle();
  else if (data[1] == '1')  cross();
  else if (data[2] == '1')  triangle();
  else if (data[3] == '1')  square();
  else if (data[4] == '1')  right();
  else if (data[5] == '1')  down();
  else if (data[6] == '1')  up();
  else if (data[7] == '1')  left();
  else if (data[8] == '1')  r_1();
  else if (data[9] == '1')  l_1();
  else printf("なにも押されていません．\r\n");

}



int main() {

    while(1){
      pc.printf("Pulses is: %i\r\n", wheel.getPulses());
      read_ack = i2c.read((addr<<1)+1, data, 16); //読み取り部
      pc.printf("%s",data);
      printf("\r\n");
      press_button();
      wait_ms(500);
    }

}








