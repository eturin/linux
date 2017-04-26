    .data
A:  .byte 0x0a      /*A db 0ah */
A1: .byte 10
A2: .byte 10
B:  .byte 0xa5      /*B db a5h */
B1: .byte 165
B2: .byte -91
V:  .byte 0x7f      /*V db 7fh */
V1: .byte 127
V2: .byte 127
G:  .byte 0x80      /*G db 80h */
G1: .byte 128
G2: .byte -128
D:  .byte 0b101     /*D db 101b*/
D1: .byte 5
D2: .byte 5

E:  .word 0x00fffe  /*E dw 0fffeh (заполнение начнется с младших битов, остальное обрежется)*/
E1: .word 65534
E2: .word -2
J:  .word 0x7f      /*J dw 7fh    (старшие биты заполнятся нулями)*/
J1: .word 127
J2: .word 127
Z:  .word 0x80      /*Z dw 80h    (старшие биты заполнятся нулями)*/
Z1: .word 128
Z2: .word 128

    .text
    .global main
main:
    movq $0, %rax
    ret
    