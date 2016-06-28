#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
const int LEN=256;


int main(){
	/*определяем pid текущего процесса (можно сразу ppid)*/
	long long pid=getpid();
	printf("%lld",pid);

	/*извлекаем всех родителей*/
	while(pid){
		char path[LEN];	
		memset(path,'\0',LEN);
		sprintf(path,"/proc/%lld/stat",pid);
	
		FILE * f=fopen(path,"r");
		if(f==NULL){
			printf("\n%s\n",path);
			perror("Ошибка");
			return 1;
		}
		long long ppid;
		char comm[LEN], state;
		fscanf(f,"%lld %s %c %lld ",&pid,comm,&state,&ppid);
		pid=ppid;
		printf("-->%lld",ppid);
		fclose(f);
	}
	printf("\n");

	return 0;
}
