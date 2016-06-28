#include <stdio.h>
#include <errno.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

struct List {
	long long pid,ppid;
	struct List * next;
};

const int LEN=1024;

int search(long long pid,const struct List * root);

int main(int argc,char **argv){
	if(argc<2)
		return 1;

	/*открываем каталог*/
	DIR * dir=opendir("/proc");
	if(dir==NULL){
		perror("Ошибка");
		return 1;
	}

	/*список процессов*/
	struct List *root=NULL, *tail=root;

	/*читаем каталог*/
	struct dirent * ent=NULL;
	while((ent=readdir(dir))!=NULL)
		if(ent->d_type==DT_DIR){
			/*извлекаем pid и ppid*/
			char path[LEN];
			sprintf(path,"/proc/%s/stat",ent->d_name);
			FILE * file=fopen(path,"r");
			if(file!=NULL){
				long long pid, ppid; 
				char comm[LEN], state; 
				fscanf(file,"%lld %s %c %lld ",&pid,comm,&state,&ppid);
				fclose(file);
				if(tail==NULL)
					tail=root=(struct List*)malloc(sizeof(struct List));
				else
					tail=tail->next=(struct List*)malloc(sizeof(struct List));
				tail->pid=pid;
				tail->ppid=ppid;
				tail->next=NULL;
			}
		}

	/*получаем аргумент*/
	long long pid=atoll(argv[1]);
	int cnt=search(pid,root)+1;/*включая процесс с указанным pid*/
	printf("%d\n",cnt);	

	/*закрываем каталог*/
	closedir(dir);


	/*очищаем память*/
	while(root!=tail){
		struct List * tmp=root;
		root=root->next;
		free(tmp);
	}
	free(tail);

	return 0;
}

int search(long long pid,const struct List * root){
	int res=0;
	const struct List * node=root;
	while(node!=NULL){
		if(node->ppid==pid){
			res+=1+search(node->pid,root);
	//		printf("%d\n",node->pid);
		}
		node=node->next;
	}

	return res;
}
