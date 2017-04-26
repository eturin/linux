    .data
C:  .byte 54 /*аналог '5'+1 = '6' */
    .text
    .global main
main:
    xor %rax, %rax
    ret
    