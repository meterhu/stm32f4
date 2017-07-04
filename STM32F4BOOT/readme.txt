
加上FPU和不加FPU的区别：

os_cpu_c.c中的OSTaskStkInit有所不同，
os_cpu_a.asm中的PendSV_Handler有所不同。

增加了对FPU寄存器的入栈和出栈的处理。



原文件:
STM32F4xx+ucos2.91+RVMDK.zip
STM32F4xx+FPU+ucos2.91+RVMDK.zip