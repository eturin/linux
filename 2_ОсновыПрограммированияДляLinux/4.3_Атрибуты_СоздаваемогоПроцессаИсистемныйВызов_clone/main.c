#include <sched.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

const int LEN=256;
const int STACK=10000;


struct Param {
	int a;
	int b;
	char * msg;
};

void f1(){
	printf("(f1)==>потомок pid = %lld\n",getpid());
}

void f2(){
	printf("pid=%lld ppid=%lld\n",getpid(),getppid());
}

int f3(void * param){
	struct Param * prm=(struct Param*)param;
	/*с этой функции начнет выполнение порожденный потомок*/
	printf("(f2)==>потомок pid = %lld\n\ta = %d\n\tb = %d\n\tname %s\n",getpid(),prm->a,prm->b,prm->msg);
	strncpy(prm->msg,"Потомок (f2)",LEN);

	return (prm->a)+(prm->b);
}



int main(){
	printf("0 Родитель pid=%lld\n",getpid());

	/*порождается потомок, который начинает свою работу с выполнения конкретной функции*/
	char *stack=(char*)malloc(STACK+1);/*размер стека для потомка*/

	pid_t pid=clone(f1/*указатель на функцию*/,
	              stack+STACK/*стек потомка (стек заполняется от больших адресов к меньшим)*/,
	              0,NULL);

	printf("1 clone (f1) вернула pid=%lld\n",pid);
	free(stack);	
	int res=0;
	waitpid(pid,&res,0);
	printf("2 потомок (f1) pid=%lld вернул %d\n",pid,WEXITSTATUS(res));
	
	/*порождается потомок склонировав родителя*/
	//stack=(char*)malloc(STACK+1);/*размер стека для потомка*/
	//
	//pid=clone(f2/*указатель на функцию*/,
	//          stack+STACK,
	//          CLONE_PARENT/*флаг клонирования значения ppid*/,
	//	  NULL);
	//
	//printf("3 clone (f2) вернула pid=%lld\n",pid);
	//free(stack);	
	//waitpid(pid,&res,0);
	//printf("4 потомок (f2) pid=%lld вернул %d\n",pid,WEXITSTATUS(res));
	


	/*функция с параметрами*/
	struct {
		int a;
		int b;
		char * msg;
	} param;
	param.a=1;
	param.b=2;
	param.msg=(char*)malloc(LEN);
	strncpy(param.msg,"Привет!",LEN);

	/*стек у потомка свой*/
	stack=(char*)malloc(STACK+1);/*размер стека для потомка*/

	pid=clone(f3/*указатель на функцию*/,
	          stack+STACK/*стек потомка (стек заполняется от больших адресов к меньшим)*/,
	          0/*флаги определяющие, что копировать и что будет общим с родителем*/,
   	          &param/*параметры для функции*/);

	printf("5 clone (f3) вернула pid=%lld\n",pid);
	waitpid(pid,&res,0);
	printf("6 потомок (f3) pid=%lld вернул %d\n",pid,WEXITSTATUS(res));
	printf("7 вернулось msg=%s\n",param.msg);
	free(stack);
	free(param.msg);

	return 0;
}
