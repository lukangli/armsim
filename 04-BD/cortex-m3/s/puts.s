.text
.global _puts
.global puts
.code 16
.syntax unified
.equ    UATRT_DATA,  0x4000C000
_puts:
    .type puts, function
puts:
    /*该子程序向UATRT发送字符串*/
    /*入参：r0 = 字符串的起始地址*/
    /*字符串要以零结尾*/
    push {r0,r1,lr}         /*保存寄存器*/
    mov r1, r0              /*把地址拷贝到r1*/
putsloop:
    ldrb.w r0, [r1], #1     /*读取一个字符并且自增地址*/
    cbz    r0, putsloopexit /*如果字符为NULL，则跳转到结束*/
    bl putc
    b putsloop
putsloopexit:
    pop {r0,r1,pc}          /*返回*/
putc:
    /*该子程序通过UATR发送一个字符*/
    /*入参: r0 = 要发送的字符*/
    push  {r1,r2,r3,lr}      /*保存寄存器*/
    ldr r1, = UATRT_DATA     /*向缓冲区发送数据*/
    str r0, [r1]
    pop {r1,r2,r3,pc}        /*返回*/
.data
    int:
        .word 0xfff
    .end

