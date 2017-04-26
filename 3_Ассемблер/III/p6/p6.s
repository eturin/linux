    .text /*сегмент кода*/
    .globl main           /*объявляем символ глобальным*/
    .type main, @function /*задаем тип символа*/
main:
    movq    $11, %rsi
    movq    $12, %rdi
    
    call    min
    
    movq    $60, %rax  /*функция exit*/
    xorq    %rdi, %rdi /*её параметр*/
    syscall            /*вызываем*/
    .size main, . - main  /*указываем размер символа main*/
    
min:
    cmpq    %rsi, %rdi
    ja      _1
    movq    %rdi, %rax
    ret
_1:
    movq    %rsi, %rax
    ret

    .size min, . - min    /*указываем размер символа min*/
    