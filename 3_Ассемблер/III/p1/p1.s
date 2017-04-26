	.text /*сегмент кода*/
.globl main           /*описание символа как глобального*/
.type main, @function /*описание типа символа (функция, а не данные)*/

main:
	movq	$1, %rsi  /*содержательные инструкции*/
	movq	$2, %rdx
	xchgq	%rsi, %rdx

	mov		$60, %rax   /*функция sys_exit для x64*/
	xor 	%rdi, %rdi  /*это вместо "return 0" в C*/
	syscall
	
	.size main, .-main
	