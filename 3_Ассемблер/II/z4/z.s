    .data
D:  .long 0x00010000 /*это 2^16*/ 
    .text
    .global main
main:
    xor %rax,   %rax
    ret
    
