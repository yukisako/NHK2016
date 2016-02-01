#include "mbed.h"
#include <stdlib.h>

I2C i2c( PB_3 , PB_10 );	//通信用のピン設定

const int addr = 0x08;
char data[17] = "zzzzzzzzzzzzzzzz";
int read_ack;
char buf = '0';
int count = 0;

int i = 0;


int main(){
	while(1){
		read_ack = i2c.read((addr<<1)+1, data, 16);	//読み取り部
		for(i=0;i<15;i++){
			printf("%c",data[i]);
		}
		printf("\n");
	}
} 