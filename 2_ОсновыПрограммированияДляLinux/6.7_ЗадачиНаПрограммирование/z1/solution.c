#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <errno.h>

#define LEN 16

int main(int argc,char ** argv){
	//получаем парамет командной строки
	if(argc<2)
		printf("используйте утилиту так:\n\t%s www.ya.ru\n",*argv);
	else{
		struct hostent * inf=gethostbyname(argv[1]);
		if(inf==NULL)
			perror("Ошибка получения сведений");
		else{
			char ** paddr=inf->h_addr_list;
			while(*paddr!=NULL){
				char tmp[LEN];
				inet_ntop(inf->h_addrtype,*paddr++,tmp,LEN);
				printf("%s\n",tmp);
			}
		}
	}

	return 0;
}
