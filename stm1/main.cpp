/***************
一個目の基板
こいつがマスターの役をして，コントローラの情報とかを他のSTMに.
***************/

#include "mbed.h"
#include "QEI.h"
#include <cstdlib>
#include <iostream>
#include <map>

using namespace std;


//*****ピン設定******************

//PCとシリアル通信する為のピン
Serial pc(USBTX, USBRX);

//ArduinoとI2C通信用のピン設定(SCA,SCL)
I2C i2c( PB_3 , PB_10 );  

//子のSTMとシリアル通信する為のピン
Serial device(PA_9, PA_10);

//サーボ用のピン(5個)
PwmOut servo_right(PA_5);
PwmOut servo_left(PA_6);
PwmOut servo_back(PA_7);
PwmOut servo_prop(PB_6);
PwmOut servo_fan(PC_7);

//電磁弁用のピン設定(4個)
DigitalOut solenoid1(PC_2);
DigitalOut solenoid2(PC_3);
DigitalOut solenoid3(PC_0);
DigitalOut solenoid4(PC_1);

//*****ピン設定終わり*************

const int addr = 0x08;  //ArduinoとI2C通信するためのアドレス

char data[17] = "zzzzzzzzzzzzzzzz";
int read_ack;

int i = 0;

//配列でパルス幅を管理
//0番目から順番に，前の右，前の左，後ろ，プロペラ，ファン．
int servo_pulses[10] = {1500,1500,1500,1500,1500};




//*****プロトタイプ宣言*******

//ボタンを押された時の関数
void cycle();
void triangle();
void cross();
void square();
void up();
void down();
void left();
void right();
void r_1();
void l_1();

//確認用関数
void check_i2c();
void servo_test();
void send_data_test();

//実際に処理が書かれた関数
void init();
void press_button();
void move_servo(PwmOut servo, int *start, int end);
void rotate(int start, int end);

//*****プロトタイプ宣言終わり**



int main() {
    while(1){
      press_button();
      //init();
      check_i2c();
      //send_data_test();
      wait_ms(50);
    }
}





////************ボタンはそれぞれ関数化**********************


void cycle(){
  pc.printf("丸ボタンが押されています．\r\n");
  pc.printf("ファンを右に回転させます．\r\n");
  device.putc('A');
}

void triangle(){
  pc.printf("三角ボタンが押されています．\r\n");
  pc.printf("ファンを上に移動させます．\r\n");
  device.putc('B');
}

void cross(){
  pc.printf("バツボタンが押されています．\r\n");
  pc.printf("ファンを下に移動させます．\r\n");
  device.putc('C');
}

void square(){
  pc.printf("四角ボタンが押されています．\r\n");
  pc.printf("ファンを左に回転させます．\r\n");
  device.putc('D');
}

void up(){
  //ロボット前に移動
  pc.printf("十字キーの上ボタンが押されています．\r\n");
  pc.printf("ロボットを前方向に並進運動させます．\r\n");
  rotate(servo_pulses[0], 1500);
  device.putc('E');
  wait(1);
}

void down(){
  //ロボット後ろに移動
  pc.printf("十字キーの下ボタンが押されています．\r\n");
  pc.printf("ロボットを後ろ方向に並進運動させます．\r\n");
  device.putc('F');
  rotate(servo_pulses[0], 1500);
  wait(1);
}

void left(){
  //ロボット左に移動
  pc.printf("十字キーの左ボタンが押されています．\r\n");
  pc.printf("ロボットを左方向に並進運動させます．\r\n");
  device.putc('G');
  rotate(servo_pulses[0], 2033);
  wait(1);
}

void right(){
  //ロボット右に移動
  pc.printf("十字キーの右ボタンが押されています．\r\n");
  pc.printf("ロボットを右方向に並進運動させます．\r\n");
  rotate(servo_pulses[0], 967);
  device.putc('H');
  wait(1);
}

void r_1(){
  pc.printf("R1ボタンが押されています．\r\n");
  pc.printf("モータを動かします\r\n");
  device.putc('I');
}

void l_1(){
  pc.printf("L1ボタンが押されています．\r\n");
  device.putc('J');
}

////************確認用関数*******************************
void check_i2c(){
  pc.printf("%s",data);
  printf("\r\n");
} 

void servo_test(){
  up();
  wait(1);
  right();
  wait(1);
  left();
  wait(1);
  down();
}

void send_data_test(){
    printf("送信側\r\n");
    device.putc('A');
}
//************確認用関数終わり****************************




//*************実際の処理用関数***************************

void press_button(){
  read_ack = i2c.read((addr<<1)+1, data, 16);
  printf("Readは%d\r\n", read_ack);
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
}

//サーボの角度を初期位置に戻す関数
void init(){
  pc.printf("初期位置に戻します\r\n");
  for (int i=0; i<3; i++){
    servo_pulses[i] = 1500;
  }
  servo_right.pulsewidth_us(1500);
  servo_left.pulsewidth_us(1500);
  servo_back.pulsewidth_us(1500);
  /*
  //この二つ，モータ違う可能性あるからとりあえず後．
  servo_prop.pulsewidth_us(1500);
  servo_fan.pulsewidth_us(1500);
  */
}

//足回りの3つのサーボを動かす関数
void rotate(int start, int end){
    int p;
    if (start < end ){
        for(p = start; p < end; p += 10) {
            servo_right.pulsewidth_us(p);
            servo_left.pulsewidth_us(p);
            servo_back.pulsewidth_us(p);
            wait_ms(10);
        }
    } else {
        for(p = start; p > end; p -= 10) {
            servo_right.pulsewidth_us(p);
            servo_left.pulsewidth_us(p);
            servo_back.pulsewidth_us(p);
            wait_ms(10);
        }
    }
    for (int i=0; i<3; i++){
      servo_pulses[i] = p;
    }
}

//一個のサーボを動かす関数
void move_servo(PwmOut servo, int *start, int end){
    int p;
    if (*start < end ){
        for(p = *start; p < end; p += 10) {
            servo.pulsewidth_us(p);
            wait_ms(20);
        }
    } else {
        for(p = *start; p > end; p -= 10) {
            servo.pulsewidth_us(p);
            wait_ms(20);
        }
    }
    *start = p;
}





