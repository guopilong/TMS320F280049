

// DisableDog - This function disables the watchdog timer.
// 关闭看门狗定时器

void DisableDog(void)
{
    volatile Uint16 temp;
    EALLOW;

    //
    // Grab the clock config so we don't clobber it
    //
    temp = WdRegs.WDCR.all & 0x0007;
    WdRegs.WDCR.all = 0x0068 | temp;
    EDIS;
}

//打开看门狗


// ServiceDog - This function resets the watchdog timer.
// Enable this function for using ServiceDog in the application
// 喂狗程序
void ServiceDog(void)
{
    EALLOW;
    WdRegs.WDKEY.bit.WDKEY = 0x0055;
    WdRegs.WDKEY.bit.WDKEY = 0x00AA;
    EDIS;
}
