#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

const int LEN=10;

int main(){
	const char * path="m";
	unlink(path);

	/*создаем жесткую ссылку*/
	int res=link("main.c",path);
	if(res){
		perror("Ошибка: ");
		return 1;
	}

	unlink(path);

	/*создаем мягкую ссылку*/
	res=symlink("main.c",path);
	if(res){
		perror("Ошибка: ");
		return 1;
	}

	

	/*ссылки открываются как файлы*/
	int fd=open(path,O_RDONLY/*|O_NOFOLLOW*/);//O_NOFOLLOW - не открывать файл по ссылке
	if(fd==-1){
		perror("Ошибка: ");
		return 1;
	}
	char buf[LEN];
	int len=0;
	while((len=read(fd,buf,LEN-1))>0){
		buf[len]='\0';
		printf(buf);
	}
	close(fd);

	/*можно работать непосредственно со ссылкой*/
	char buf2[512]; 
	len = readlink(path, buf2, sizeof(buf2)-1); //вернет -1, если указать не ссылку
	if(len==-1){ 
		perror("Ошибка: ");
		return 1;
       	}
	buf2[len]='\0';
       	printf("%s -> %s\n", path, buf2);

	/*fd=open(path,O_NOFOLLOW|O_PATH|O_RDWR);
	if(fd==-1){
		perror("Ошибка: ");
		return 1;
	}

	close(fd);*/

	return 0;
}
