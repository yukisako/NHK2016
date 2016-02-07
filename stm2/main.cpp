/***************
二個目の基板
スレーブ役

この基板は，足回りのモータ3つとそれに付随しているエンコーダを制御している．
***************/



#include "mbed.h"
#include "QEI.h"
#include <cstdlib>
#include <iostream>
#include <map>

#define MOTOR_VAL 0.7f

using namespace std;


//*****ピン設定******************
//PCとシリアル通信する為のピン
Serial pc(USBTX, USBRX);

//親のSTMとシリアル通信する為のピン
Serial device(PA_9, PA_10);

//ロータリーエンコーダ(今回は2層のエンコーダなので第三引数は考えなくて良い)
QEI enc_right(PA_0, PA_1, NC, 500);
QEI enc_left(PA_2, PA_3, NC, 500);
QEI enc_back(PB_14, PB_13, NC, 500);

//モータ
PwmOut motor_right(PC_9);
PwmOut motor_right_inv(PC_8);
PwmOut motor_left(PB_8);
PwmOut motor_left_inv(PB_9);
PwmOut motor_back(PA_6);
PwmOut motor_back_inv(PC_7);



//*****ピン設定終わり*************

/*
//I2CSlaveできなかった...悲しい
I2CSlave i2c( PB_3 , PB_10 );  //通信用のピン設定(SCA,SCL)
const int addr = 0x70;
int read_ack;

void check_i2c(){
  pc.printf("%s",data);
  printf("\r\n");
} 

*/
char data = 'z';

char buf = '0';
int i = 0;

//配列でパルス幅を管理
int motor_pulses[10] = {0,0,0,0,0,0};

/*
//プロトタイプ宣言
void check_motor();
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
void check_qei();

void press_button();
void servo_foward();
void servo_back();
void servo_foward_test();
void servo_back_test();
void init();
void get_data();
void motor_setup();
*/


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
void check_motor(PwmOut motor, PwmOut motor_inv);
void send_data_test();

//実際に処理が書かれた関数
void init();
void press_button();
void motor_setup();
void get_data();
void motor_foward();
void motor_backward();
void motor_stop();
//*****プロトタイプ宣言終わり**


int main() {
  motor_setup();
    while(1){
      get_data();
      press_button();
      wait_ms(50);
    }
}





////************ボタンはそれぞれ関数化**********************


void cycle(){
  pc.printf("丸ボタンが押されています．\r\n");
  pc.printf("ファンを右に回転させます．\r\n");
}

void triangle(){
  pc.printf("三角ボタンが押されています．\r\n");
  pc.printf("ファンを上に移動させます．\r\n");
}

void cross(){
  pc.printf("バツボタンが押されています．\r\n");
  pc.printf("ファンを下に移動させます．\r\n");
}

void square(){
  pc.printf("四角ボタンが押されています．\r\n");
  pc.printf("ファンを左に回転させます．\r\n");
}

void up(){
  //ロボット前に移動
  pc.printf("十字キーの上ボタンが押されています．\r\n");
  pc.printf("ロボットを前方向に並進運動させます．\r\n");
  motor_foward();
  wait(1);
  motor_stop();
}

void down(){
  //ロボット後ろに移動
  pc.printf("十字キーの下ボタンが押されています．\r\n");
  pc.printf("ロボットを後ろ方向に並進運動させます．\r\n");
  motor_backward();
  wait(1);
  motor_stop();
}

void left(){
  //ロボット左に移動
  pc.printf("十字キーの左ボタンが押されています．\r\n");
  pc.printf("ロボットを左方向に並進運動させます．\r\n");
  motor_foward();
  wait(1);
  motor_stop();
}

void right(){
  //ロボット右に移動
  pc.printf("十字キーの右ボタンが押されています．\r\n");
  pc.printf("ロボットを右方向に並進運動させます．\r\n");
  motor_foward();
  wait(1);
  motor_stop();
}

void r_1(){
  pc.printf("R1ボタンが押されています．\r\n");
  pc.printf("モータを動かします\r\n");
  check_motor(motor_right, motor_right_inv);
}

void l_1(){
  pc.printf("L1ボタンが押されています．\r\n");
}

////************確認用関数*******************************
void check_qei(){
  int pulse = enc_right.getPulses();
  pc.printf("Pulses is: %d\r\n", pulse);
}




void check_motor(PwmOut motor, PwmOut motor_inv){
  printf("モーターを動かします\r\n");

  //RS-540は10Vくらいかけないと動かない...
  motor = 0.9f;
  motor_inv = 0.0f;
  wait(2);
  motor_inv = 0.0f;
  motor = 0.0f;
  wait(1);
  motor_inv = 0.9f;
  motor = 0.0f;
  wait(2);
  motor_inv = 0.0f;
  motor = 0.0f;
  wait(1);
  printf("モータ動かし終わりました\r\n");
}



//************確認用関数終わり****************************


//*************実際の処理用関数***************************

void get_data(){
  data = 'z';
  printf("受信側\r\n");
  data = device.getc();
  pc.putc(data);
}

void press_button(){
  if (data=='A') cycle();
  else if (data == 'B')  cross();
  else if (data == 'C')  triangle();
  else if (data == 'D')  square();
  else if (data == 'E')  right();
  else if (data == 'F')  down();
  else if (data == 'G')  up();
  else if (data == 'H')  left();
  else if (data == 'I')  r_1();
  else if (data == 'J')  l_1();
}

//PWM周波数を設定
void motor_setup(){
  motor_right.period_us(50);
  motor_right_inv.period_us(50);
  motor_left.period_us(50);
  motor_left_inv.period_us(50);
  motor_back.period_us(50);
  motor_back_inv.period_us(50);
}


void motor_foward(){
  pc.printf("モータを回転させます．\r\n");
  motor_right = MOTOR_VAL;
  motor_right_inv = 0.0f;
  motor_left = MOTOR_VAL;
  motor_left_inv = 0.0f;
  motor_back = MOTOR_VAL;
  motor_back_inv = 0;
}

void motor_backward(){
  pc.printf("モータを逆回転させます．\r\n");
  motor_right = 0.0f;
  motor_right_inv = MOTOR_VAL;
  motor_left = 0.0f;
  motor_left_inv = MOTOR_VAL;
  motor_back = 0.0f;
  motor_back_inv = MOTOR_VAL;
}


void motor_stop(){
  pc.printf("モータを止まらせます．\r\n");
  motor_right = 0.0f;
  motor_right_inv = 0.0f;
  motor_left = 0.0f;
  motor_left_inv = 0.0f;
  motor_back = 0.0f;
  motor_back_inv = 0.0f;

}
