    .text /*сегмент кода*/
.globl  main          /*объявляем символ как глобальный*/
.type main, @function /*задаем тип символа*/
main:
    movq    $1, %rsi    /*инициализация*/
    movq    $2, %rdx
    
    pushq   %rsi  /*обмене через стек*/    
    pushq   %rdx
    popq    %rsi
    popq    %rdx

    movq    $60, %rax   /*функция exit*/
    xorq    %rdi, %rdi  /*её параметр*/
    syscall             /*вызов*/
    
    .size main, . - main /*размер символа*/
    