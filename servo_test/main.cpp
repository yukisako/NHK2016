#include "mbed.h"

PwmOut servo1(PA_0);
//PwmOut servo2(PA_1);

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


int main() {
  servo1.period_ms(3);
  while(1) {
        /*
        for(int p = 700; p <= 1500; p += 10) {
            servo1.pulsewidth_us(p);
            wait_ms(20);
        }
        */
        int pos  = 700;
        move_servo(servo1, &pos, 1500);
        printf("%d\r\n",pos);
        wait(1);
        move_servo(servo1, &pos, 2300);
        printf("%d\r\n",pos);
        wait(1);
        move_servo(servo1, &pos, 1500);
        printf("%d\r\n",pos);
        wait(30);
        move_servo(servo1, &pos, 700);
        printf("%d\r\n",pos);
        wait(1);
  }
}


