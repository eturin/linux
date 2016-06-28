#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

const int LEN=1024;

int main(){
	/*открываем каталог*/
	DIR * dir=opendir("/proc");
	if(dir==NULL){
		perror("Ошибка");
		return 1;
	}
	
	int cnt=0;

	/*читаем каталог*/
	struct dirent * ent;
	while((ent=readdir(dir))!=NULL)
		if(ent->d_type==DT_DIR){
			/*пробуем узнать имя процесса*/
			char path[LEN];
			sprintf(path,"/proc/%s/comm",ent->d_name);
			FILE * file=fopen(path,"r");
			if(file!=NULL){
				char name[LEN];
				fscanf(file,"%s",name);
				fclose(file);
				if(strcmp(name,"genenv")==0)
					++cnt;
			}
		}
	
	printf("%d\n",cnt);


	closedir(dir);

	return 0;
}
