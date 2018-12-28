/* 定义常数 */
    .equ STACK_TOP, 0x20000800
.text
.global _start
.code 16
.syntax unified
;.global buf
/* .thumbfunc */
/* .thumbfunc仅仅在2006Q3-26之前的CodeSourcery工具中需要*/
_start:
    .word STACK_TOP, start
    .type start, function
/* 主程序入口点 */
start:
    movs r0, #0
    movs r1, #0
    movs r2, #0
    movs r3, #0
    movs r4, #0
    movs r5, #0
    ldr r0, =buf
    bl puts
    ;movs r0, #0x4
    ;bl puts
deadloop:
    b deadloop
buf:
    .ascii "Hello\n"
    .byte 0
    .align
.end
