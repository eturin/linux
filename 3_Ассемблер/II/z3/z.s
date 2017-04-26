    .data
A:  .word   0x1020
/*первый байт(младший)
    x/1hb (void*)&A    =0x20
  второй байт
    x/1hb (void*)&A+1  =0x10
*/
B:  .long   0x10203040
/*первый
    x/1hb (void*)&B   
  второй
    x/1hb (void*)&B+1
  третий
    x/1hb (void*)&B+2
  последний (самый старший)
    x/1hb (void*)&B+3
*/
    .text
    .global main
main:
    xor %rax,   %rax
    ret
    
