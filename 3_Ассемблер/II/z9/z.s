    .data
/*три байта со значениями кодов символов*/
m1: .byte 'A', 'B', 'C'
/*пять байт со значениями 0*/
m2: .zero 5
/*три байта со значениями кодов символов*/
m3: .string "АБВ"
m4: .ascii "АБВ"


    
    .text
    .global main
main:
    xor %rax, %rax
    ret
    