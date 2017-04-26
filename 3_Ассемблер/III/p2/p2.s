	.text /*сегмент кода*/
.globl main            /*описание символа как глобального*/
.type main, @function  /*указание типа символа*/
main:
	movq	$1, %rsi
	movq	$2, %rdx
	addq	%rdx, %rsi	
	
	movq 	$60, %rax  /*функция exit*/
	xorq	%rdi, %rdi /*её параметр*/
	syscall            /*системный вызов*/

	.size main, . - main /*размер символа main*/
