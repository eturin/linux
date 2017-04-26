    .data
a:  .byte 0,1,2
b:  .byte 3,4,5,6
/*проверка значения по адресу b+2:
    x/6db (void*)&b+2
*/
    
    .text
    .global main
main:
    xor %rax,   %rax
    ret