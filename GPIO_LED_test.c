#include "F28x_Project.h"
#define CPU_RATE  10.00L
#define DELAY_US(A)  F28x_usDelay(((((long double) A * 1000.0L) / (long double)CPU_RATE) - 9.0L) / 5.0L)       //这两句定义的目的就是为了生成一个延时函数

void LED_Init(void);
void LED_Init(void)
{
    EALLOW;
    GpioCtrlRegs.GPAPUD.bit.GPIO23 = 0;   // 设置GPIO23上拉使能
    GpioDataRegs.GPASET.bit.GPIO23 = 1;   // 设置GPIO23为高电平
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 0;  // 0代表通用IO口，1代表其他外设接口
    GpioCtrlRegs.GPADIR.bit.GPIO23 = 1;   // 1代表输出，0代表输入
    EDIS;
}


void main(void)
{
    InitSysCtrl();
    InitGpio();        //这两句是初始化时钟和gpio
    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();
    EnableInterrupts();
    LED_Init();
    EINT;
    ERTM;                    //以上是配置中断，通常都这么配

    while(1)            //点亮和关闭GPIO23，流水的在while里循环
    {
        GpioDataRegs.GPASET.bit.GPIO23 = 1;       
        DELAY_US(1000000);
        GpioDataRegs.GPACLEAR.bit.GPIO23 = 1;
        DELAY_US(1000000);
    }

}


//下面是库函数中给出的读取和写入GPIO的方法
//Uint16 GPIO_ReadPin(Uint16 gpioNumber)

//GPIO_WritePin(Uint16 gpioNumber, Uint16 outVal)



