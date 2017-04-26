    .data /*сегмент данных*/
a:  .quad 1
b:  .quad 2

    .text /*сегмент кода*/
.globl main           /*объявляем символ глобальным*/
.type main, @function /*задаем тип символа*/
main: 
    leaq    a, %rdi
    leaq    b, %rsi
    call    swap

    movq    $60, %rax   /*функция exit*/
    xorq    %rdi, %rdi  /*её параметр*/
    syscall             /*вызов*/
.size main, . - main  /*указываем размер main*/

swap:
    movq    (%rsi), %rax /*загружаем из мапяти*/
    xchgq   (%rdi), %rax /*обмен память-регистр*/
    movq    %rax, (%rsi) /*записываем в память*/
    
    retq
.size swap, . - swap  /*указываем размер swap*/
