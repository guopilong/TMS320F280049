

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
