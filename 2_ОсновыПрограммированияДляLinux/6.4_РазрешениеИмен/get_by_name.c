#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>

#define LEN 256

int main(){
	//запрашиваем имя хоста у пользователя
	char name[LEN],c;
	int i=0;
	printf("Укажите имя хоста: ");
	while(i<LEN-1 && (c=getchar())!='\n' && c!=EOF)
		name[i++]=c;
	name[i]='\0';
	
	//извлекаем сведения об имени 
	struct hostent * inf=gethostbyname(name);
	if(inf==NULL)
		herror("Ошибка");
	else{	
		//отображаем сведения
		printf("Официальное имя:\n\t%s\nВозможные имена:\n",inf->h_name);
		i=0;
		while(inf->h_aliases[i]!=NULL)
			printf("\t%s\n",inf->h_aliases[i++]);
		if(i==0)
			printf("\t<нет>\n");
		printf("Тип адреса:\n\t%s\n", inf->h_addrtype == AF_INET ? "AF_INET" : "AF_INET6");
		printf("Длина адресов в байтах\n\t%d\nAдреса:\n", inf->h_length);
		i=0;
		while(inf->h_addr_list[i]!=NULL){
			inet_ntop(inf->h_addrtype, inf->h_addr_list[i++], name, LEN);
			printf("\t%s\n",name);
		}
		if(i==0)
			printf("\t<нет>\n");
		

	}
	




	return 0;
}
