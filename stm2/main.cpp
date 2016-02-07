#include "mbed.h"
#include "QEI.h"
#include <cstdlib>
#include <iostream>
#include <map>

using namespace std;

Serial pc(USBTX, USBRX);
//Use X4 encoding.
//QEI wheel(p29, p30, NC, 624, QEI::X4_ENCODING);
//Use X2 encoding by default.
QEI wheel (PA_1, PA_4, NC, 500);

I2CSlave i2c( PB_3 , PB_10 );  //通信用のピン設定(SCA,SCL)

const int addr = 0x70;
char data[16] = "yyyyyyyyyyyyyyy";
int read_ack;
char buf = '0';
PwmOut servo1(PA_0);
int i = 0;

PwmOut motor1(PC_7);
PwmOut motor1_inv(PA_9);
PwmOut motor2();

//配列でパルス幅を管理

int servo_pulses[10] = {1500,1500,1500,1500,1500,1500,1500};
//servo_pulses[0] = 700;
int motor_pulses[10] = {0,0,0,0,0,0,0,0,0,0};


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
void check_i2c();
void press_button();
void servo_foward();
void servo_back();
void servo_foward_test();
void servo_back_test();
void init();





int main() {
    motor1.period_us(50);
    motor1_inv.period_us(50);
    i2c.address(addr);
    while(1){
      press_button();
      check_i2c();
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
  //ロボット前進
  pc.printf("十字キーの上ボタンが押されています．\r\n");
  pc.printf("ロボットを前方向に並進運動させます．\r\n");
}

void down(){
  //ロボット後退
  pc.printf("十字キーの下ボタンが押されています．\r\n");
  pc.printf("ロボットを後ろ方向に並進運動させます．\r\n");
}

void left(){
  //ロボット回転(左)
  pc.printf("十字キーの左ボタンが押されています．\r\n");
  pc.printf("ロボットを左方向に並進運動させます．\r\n");
}

void right(){
  //ロボット回転(右)
  pc.printf("十字キーの右ボタンが押されています．\r\n");
  pc.printf("ロボットを右方向に並進運動させます．\r\n");
}

void r_1(){
  pc.printf("R1ボタンが押されています．\r\n");
  pc.printf("モータを動かします\r\n");
  check_motor();
}

void l_1(){
  pc.printf("L1ボタンが押されています．\r\n");
}

////************確認用関数*******************************
void check_qei(){
  int pulse = wheel.getPulses();
  pc.printf("Pulses is: %d\r\n", pulse);
}

void check_i2c(){
  pc.printf("%s",data);
  printf("\r\n");
} 

void check_motor(){
  printf("モーターを動かします\r\n");

  //RS-540は10Vくらいかけないと動かない...
  motor1 = 0.9f;
  motor1_inv = 0.0f;
  wait(2);
  motor1_inv = 0.0f;
  motor1 = 0.0f;
  wait(1);
  motor1_inv = 0.9f;
  motor1 = 0.0f;
  wait(2);
  motor1_inv = 0.0f;
  motor1 = 0.0f;
  wait(1);
  printf("モータ動かし終わりました\r\n");
}

//************確認用関数終わり****************************

void servo_foward(){
  if (servo_pulses[0] <= 2250){
    pc.printf("サーボを正転させます\r\n");
    servo_pulses[0] += 50;
    servo1.pulsewidth_us(servo_pulses[0]);
  } else {
    pc.printf("最大角を超えています\r\n");
  }
}

void servo_back(){
  if (servo_pulses[0] >= 750){
    pc.printf("サーボを逆転させます\r\n");
    servo_pulses[0] -= 50;
    servo1.pulsewidth_us(servo_pulses[0]);
  } else {
    pc.printf("最大角を超えています\r\n");
  }
}



void servo_foward_test(){
  pc.printf("サーボを正転させます(テスト)\r\n");
  for(int p = 700; p <= 2300; p += 10) {
      servo1.pulsewidth_us(p);
      wait_ms(10);
  }
}

void servo_back_test(){
  pc.printf("サーボを逆転させます(テスト)\r\n");
  for(int p = 2300; p >= 700; p -= 10) {
    servo1.pulsewidth_us(p);
    wait_ms(10);
  }
}


void init(){
  pc.printf("初期位置に戻します\r\n");
  servo_pulses[0] = 1500;
  servo1.pulsewidth_us(servo_pulses[0]);
}


//*************実際の処理用関数***************************
void press_button(){
  int i = i2c.receive();
  read_ack = i2c.read(data, 16); //読み取り部
  printf("%d\r\n", read_ack);
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





