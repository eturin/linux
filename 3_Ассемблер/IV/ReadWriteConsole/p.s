/*Чтоб узнать, как вызывать функции из библиотеки языка С, нужно написать на С и дизассемблировать так: objdump -d ./c*/
/*сегмент данных*/    
    .data 
buf:    .byte 0
        /*скобки*/
b:      .string "[({" 
e:      .string "])}"
        /*сообщения*/
err:    .string "%u\n"  
suc:    .string "Success\n"
res:    .string "                                 "

/*сегмент кода*/
    .text 
    .extern sprintf
    .globl main           /*объявляем символ глобальным*/
    .type main, @function /*и сразу задаем его тип*/
main:
    pushq   %rbp
    xorq    %rbx, %rbx  /*позиция символа в строке*/
    xorq    %rbp, %rbp  /*кол-во элементов в стеке*/
    
next:
    movq    $0,  %rax   /*берем очередной символ с клавиатуры (функция read)*/
    movq    $0,  %rdi   /*дескриптор stdin*/
    lea     buf, %rsi   /*буфер для приема символа*/
    movq    $1,  %rdx   /*один байт*/
    syscall            
    
    testq   %rax, %rax  /*проверяем прочитан символ или нет*/
    jz      chek_stack  
    incq    %rbx        /*увеличиваем индекс символа*/
    
    cld                 /*ищем символ среди открывающихся скобок*/
    movq    $3,  %rcx
    xorq    %rax,%rax
    movb    buf, %al
    lea     b,   %rdi
    repne   scasb    
    jne     eb          /*если это не открывающаяся скобка, то сравниваем с закрывающимися*/
    
    shlq    $56, %rax   /*помещаем открывающуюся скобку*/
    orq     %rbx, %rax  /*и её индекс в стек*/
    pushq   %rax
    incq    %rbp        /*кол-во элементов в стеке увеличивается*/ 
    jmp     next           
    
eb:
    cld                 /*ищем закрывающуюся скобку*/
    movq    $3,  %rcx
    xorq    %rax,%rax
    movb    buf, %al
    lea     e,   %rdi
    repne   scasb    
    jne     next        /*если это не закрывающаяся скобка, то игнорируем*/

    movq    %rbx, %rdx  /*сохраним позицию текущего символа*/
    shlq    $8,   %rdx
    
    testq   %rbp, %rbp  /*проверие есть ли элементы в стеке*/
    jz      error       
    
    popq    %rax        /*проверим скобку на вершине стека*/
    decq    %rbp        /*кол-во элементов в стеке уменьшается*/
    rolq    $8, %rax
    negq    %rcx
    addq    $2, %rcx
    cmpb    b(,%rcx,), %al
    je      next        /*скобка на вершине стека соответствует нашей*/

error:
    testq   %rbp, %rbp /*очищаем стек*/
    jz      prt
    movq    %rbp, %rcx 
lp1:    
    popq    %rax       
    loop    lp1
prt:
    shrq    $8,  %rdx   /*вызываем printf*/
    lea     res, %rdi
    lea     err, %rsi
    movq    %rsp,%rbp
    callq   sprintf
    lea     res, %rsi   /*буфер для приема символа*/
    movq    %rax,  %rdx /*кол-во байт*/
    jmp     wr

chek_stack:
    testq   %rbp, %rbp
    lea     suc, %rsi  /*буфер для приема символа*/
    movq    $8, %rdx   /*8 байт*/
    jz      wr
    movq    %rbp, %rcx /*вынимаем со дна стека скобку и её позицию*/
lp2:    
    popq    %rdx       
    loop    lp2
    xorq    %rbp, %rbp /*стек теперь пуст*/
    rolq    $8,   %rdx
    jmp     error
        
wr:
    movq    $1, %rax   /*функция write*/
    movq    $1, %rdi   /*дескриптор stdout*/
    syscall
    
exit:    
    popq    %rbp
    movq    $60, %rax  /*функция exit*/
    xorq    %rdi, %rdi /*её параметр*/
    syscall            /*вызываем*/
    
    .size main, . - main  /*указываем размер символа*/
    