    .data
A:  .byte 0x11   /*A db 17  */
B:  .byte 0x0f   /*B db 15  */
V:  .byte 0xff   /*V db 255 */
//VV: .byte 255
G:  .byte 0x96   /*G db 150 */
//GG: .byte 150
D:  .byte 0xef   /*D db -17 */
E:  .byte 0xff   /*E db -1  */ 
J:  .byte 0x96   /*J db -106*/
Z:  .byte 0x80   /*Z db -128*/

I:  .word 0x0011 /*I dw 17  */ 
K:  .word 0xffef /*K dw -17 */    
L:  .word 0xffff /*L dw -1  */
M:  .word 0x00ff /*M db 255 */
    .text
    .global main
main:
    movb B, %al
    
    movq $0,    %rax
    ret
    