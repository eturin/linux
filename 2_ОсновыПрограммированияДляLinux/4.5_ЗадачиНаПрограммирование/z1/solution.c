#include <stdio.h>
#include <errno.h>
#include <unistd.h>

const int LEN=1024;

int main(){
	/*узнаем pid процесса*/
	long long pid=getpid();

	char path[LEN];
	sprintf(path,"/proc/%lld/stat",pid);
	
	FILE * file=fopen(path,"r");
	if(file==NULL){
		perror("Ошибка");
		return 1;
	}
	long long ppid; 
	char comm[LEN], state; 
	fscanf(file,"%lld %s %c %lld ",&pid,comm,&state,&ppid);
	fclose(file);

	printf("%lld\n",ppid);

	return 0;
}
