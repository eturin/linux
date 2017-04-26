    .text /*сегмент кода*/
    .globl main           /*объявляем символ глобальным*/
    .type main, @function /*и задаем его тип сразу*/
main:
    movq    $2, %rdi   /*число*/
    movq    $16, %rsi  /*степень*/

    call    pow

    movq    $60, %rax  /*функция exit*/
    xorq    %rdi, %rdi /*её параметр*/
    syscall            /*вызываем*/
    .size main, . - main  /*указываем размер символа*/
    
pow:
    xorq    %rax, %rax /*результат*/
    test    %rdi, %rdi
    jz      ok         /*ноль в любой натуральной степени*/


    addq    $1, %rax   /*инициализация перед циклом*/
    movq    %rdi, %rbx /*множитель*/
repeat:                /*цикл*/
    shrq    %rsi       /*степень анализируется в двоичном виде*/
    jnc     next
    mulq    %rbx       /*если бит установлен, то уиножаем на множитель*/
next:
    xchg    %rax, %rbx /*множитель в любом случае возводим в квадрат*/
    mulq    %rax
    xchg    %rax, %rbx
    
    test    %rsi, %rsi /*проверяем наличие установленных битов в оставшемся фрагменте степени*/
    jnz     repeat
    
ok:    
    retq
    .size pow, . - pow    /*указываем размер символа*/
    