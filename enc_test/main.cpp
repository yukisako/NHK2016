#include "mbed.h"
#include "QEI.h"

#define MOTOR_VAL 0.7f

void motor_foward();
void motor_backward();
void motor_stop();

PwmOut motor_right(PC_7);
PwmOut motor_right_inv(PA_9);
Serial pc(USBTX, USBRX);

QEI enc_side_right(PA_0, PA_1, NC, 500);

void check_qei();

int main() {
  motor_right.period_us(50);
  motor_right_inv.period_us(50);
  while(1) {
        check_qei();

        /*
        motor_foward();
        wait(2);
        motor_stop();
        wait(2);
        motor_backward();
        wait(2);
        motor_stop();
        */
  }
}

void motor_foward(){
  pc.printf("モータを回転させます．\r\n");
  motor_right = MOTOR_VAL;
  motor_right_inv = 0.0f;
}

void motor_backward(){
  pc.printf("モータを逆回転させます．\r\n");
  motor_right = 0.0f;
  motor_right_inv = MOTOR_VAL;
}


void motor_stop(){
  pc.printf("モータを止まらせます．\r\n");
  motor_right = 0.0f;
  motor_right_inv = 0.0f;
}

void check_qei(){
  int pulse = enc_side_right.getPulses();
  pc.printf("Pulses is: %d\r\n", pulse);
}

