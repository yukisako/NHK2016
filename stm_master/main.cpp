#include "mbed.h"
/*
Serial pc(USBTX, USBRX);

I2C i2c( PB_3 , PB_10 );  //通信用のピン設定(SCA,SCL)

const int addr = 0x70;
char data[16] = "zzzzzzzzzzzzzzz";

int write_ack;

void check();

int main() {
    while(1){
      check();
      wait_ms(50);
    }
}

void check(){
  write_ack = i2c.write((addr<<1), data, 16); 
  printf("Ackは%d\r\n", write_ack);
  pc.printf("%s",data);
  printf("\r\n");
}

*/


 
Serial device(PA_9, PA_10);


 
int main() {
  while(1){
    printf("送信側\r\n");
    device.putc('A');
    }
}