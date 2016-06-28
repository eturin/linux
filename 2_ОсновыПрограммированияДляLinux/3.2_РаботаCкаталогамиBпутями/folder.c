#include <sys/types.h>
#include <dirent.h>

#include <errno.h>
#include <stdio.h>

int main(){
	/*открываем каталог по пути*/
	const char * path="./";
	DIR * dir=opendir(path);
	//DIR * dir=fdopendir(fd); //открываем каталог по дискриптору
	if(dir==NULL){
		perror("Ошибка:");
		return 1;
	}

	/*читаем из каталога список файлов*/
	struct dirent * d_ent=NULL;
	while((d_ent=readdir(dir))!=NULL){
		printf("Файл: %s\nтип=%d\nдлинна записи в байтах=%d\nномер записи в каталоге=%d\n",d_ent->d_name,d_ent->d_type,d_ent->d_reclen,d_ent->d_off);
		switch(d_ent->d_type){
			case (DT_BLK): printf("Это бинарное устройство\n");break;
			case (DT_CHR): printf("Это символьное устройство\n");break;
			case (DT_DIR): printf("Это каталог\n");break;
			case (DT_FIFO): printf("Это именованный канал (FIFO)\n");break;
		        case (DT_LNK): printf("Это мягкая ссылка\n");break;
			case (DT_REG): printf("Это обычный файл\n");break;
			case (DT_SOCK): printf("Это сокет UNIX\n");break;
			case (DT_UNKNOWN): printf("Неизвестный тип файла\n");break; //не многие системы целиком поддерживают d_type (часто выдается на них неизвестный тип файла)
		}
		printf("inode=%i\n\n",d_ent->d_ino);
	}
	/*проверяем наличие ошибок при последнем просмотре каталога*/
	if(errno){
		perror("Ошибка:");
	}

	/*закрываем каталог*/
	closedir(dir);

	
	return 0;
}
