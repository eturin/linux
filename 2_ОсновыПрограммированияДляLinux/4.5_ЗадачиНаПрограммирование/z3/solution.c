#include <stdio.h>
#include <errno.h>
#include <string.h>

const size_t LEN=1024;

int main(int argc,char ** argv){
	for(int i=1;i<argc;++i){
		/*извлекаем очередной pid из параметра*/
		char pid[LEN];
		sprintf(pid,argv[i]);

		/*извлекаем всех родителей*/
		while(strcmp(pid,"0")!=0){
			char path[LEN];
			sprintf(path,"/proc/%s/stat",pid);
			FILE * file=fopen(path,"r");
			if(file==NULL){
				printf("Нет процесса с pid=%s\n",pid);
				sprintf(pid,"0");
			}else{
				long long ppid; 
				char comm[LEN], state; 
				fscanf(file,"%s %s %c %lld ",pid,comm,&state,&ppid);
				printf("%s\n",pid);
				sprintf(pid,"%lld",ppid);
				fclose(file);
			}
		}
	}
	return 0;
}
