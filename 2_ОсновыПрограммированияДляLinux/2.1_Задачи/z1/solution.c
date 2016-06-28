/*Функция возвращает значение длины строки string, умноженное на multiplier, и увеличивает на 1 значение, на которое указывает count.*/
int stringStat(const char * str, int mult, int * cnt){
	int len=0;
	/*двигаем указатель на позицию за '\0' и подсчитываем длинну строки*/
	while(*str++!='\0') ++len;
	*cnt+=1;
	
	return len*mult;
}
