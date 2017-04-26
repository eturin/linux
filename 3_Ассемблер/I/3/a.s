.text
.global main /*символ, видимый снаружи*/

main:
    push $15 
    push $14
    pop %rax /*вынимаем из стека и пишем его в регистр rax*/
    
    push $13
    pop %rax 
    
    push $12
    pop %rax 
    
    push $11
    pop %rax 
    
    push $10
    pop %rax 
    
    movl $1, %eax /*записываем номер системного выхова exit*/
    movl $0, %ebx /*единственный параметр - код возврата*/
    
    int $0x80  /*вызывае прерывание*/
    
    .size main, . - main /*переменная будет содержать размер функции main*/
    