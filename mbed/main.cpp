#include "mbed.h"
#include <stdlib.h>
#include "QEI.h"


Serial pc(USBTX, USBRX);
I2C i2c( PB_3 , PB_10 );	//’ÊM—p‚Ìƒsƒ“İ’è(SCA,SCL)

const int addr = 0x08;
char data[17] = "zzzzzzzzzzzzzzzz";
int read_ack;
char buf = '0';
int count = 0;


int i = 0;


int main(){
	while(1){
		read_ack = i2c.read((addr<<1)+1, data, 16);	//“Ç‚İæ‚è•”
		for(i=0;i<15;i++){
			pc.printf("%c",data[i]);
		}
    printf("\r\n");
    wait(0.1);
  }
} 