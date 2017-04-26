	.text /*сегмент кода*/
.globl main             /*описание символа как глобального*/
.type nmin, @function   /*указание типа символа*/
main:
	/*пересчет фарингейтов в цельси Tc=5*(Tf-32)/9*/
	movq	$40, %rsi  /*исходное значение*/
	
	subq	$32, %rsi  /*вычитаем 32*/
	movq	%rsi, %rax /*умножаем на 5*/
	movq	$5, %rsi
	imulq	%rsi       /*результат в rdx:rax*/
	movq	$9, %rsi   /*делим его на 9*/
	idivq   %rsi
	
	movq	%rax, %rsi /*результат укладываем обратно*/

	movq	$60, %rax  /*функция exit*/
	xorq	%rdi, %rdi /*её параметр*/
	syscall            /*системный вызов*/

	.size main, . - main /*размер символа*/

