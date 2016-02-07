#include <mbed.h>
 /*
I2CSlave slave( PB_3 , PB_10 );
 
int main() {
    char buf[10];
    char msg[] = "Slave!";
 
    slave.address(0x70<<1);
    while (1) {
        int i = slave.receive();
        switch (i) {
            case I2CSlave::ReadAddressed:
                slave.write(msg, strlen(msg) + 1); // Includes null char
                break;
            case I2CSlave::WriteGeneral:
                slave.read(buf, 10);
                printf("Read G: %s\r\n", buf);
                break;
            case I2CSlave::WriteAddressed:
                slave.read(buf, 10);
                printf("Read A: %s\r\n", buf);
                break;
            default:
                printf("error\r\n");
        }
        for(int i = 0; i < 10; i++) buf[i] = 0;    // Clear buffer
    }
}

*/
Serial device(PA_9, PA_10);  // tx, rx
/*
void pc_rx () {
    device.putc(pc.getc());
}
 
void dev_rx () {
    pc.putc(device.getc());
}


 

void pc_tx () {
    pc.putc(buf[i]);
    i ++:
    if (i >= strlen(buf)) {
       pc.attach(NULL, Serial::TxIrq);
    }
}
 
int main() {
    pc.attach(pc_rx, Serial::RxIrq);
    device.attach(dev_rx, Serial::RxIrq);
    pc.attach(pc_tx, Serial::TxIrq);
 
    while(1);
}
*/
Serial pc(USBTX, USBRX);

int main() {
    while(1) {
        printf("受信側\r\n");
        pc.putc(device.getc());
        wait(1);
    }
}

