#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>

#define LEN 500

int main(){
	//спрашиваем адрес 
	printf("Укажите адрес ipv4: ");
	int a,b,c,d;
	scanf("%d.%d.%d.%d",&a,&b,&c,&d);

	//получаем сведении по адресу
	char name[LEN];
	sprintf(name,"%d.%d.%d.%d",a,b,c,d);
	struct in_addr addr;
	inet_aton(name,&addr);
	struct hostent * inf=gethostbyaddr(&addr,sizeof(addr),AF_INET); 
	if(inf==NULL) 
		herror("Ошибка"); 
	else{ //отображаем сведения
	       printf("Официальное имя:\n\t%s\nВозможные имена:\n",inf->h_name); 
	       int i=0;
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
		       printf("\t<нет>\n"); }	

	return 0;
}
