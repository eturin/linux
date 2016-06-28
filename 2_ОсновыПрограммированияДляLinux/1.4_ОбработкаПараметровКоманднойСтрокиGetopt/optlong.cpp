#include <iostream>
#include <getopt.h>

int main(int argvc, char ** argv){
	/*для обработки коротких параметров*/
	const char * line="l:h:v?";
	/*для длинных параметров (возвращается последний символьный параметр структуры в вызове getopt_long)*/
	option long_line[]={
		{"line",required_argument,nullptr,'l'}, /*обязательный аргумент*/
		{"help2",no_argument,nullptr,'?'},      /*нет аргумента*/
		{"help",optional_argument,nullptr,'h'}, /*может быть аргумент*/
		{"ver",no_argument,nullptr,'v'}         /*нет аргумента*/
	};
	int c,ind;
	while((c=getopt_long(argvc,argv,line,long_line,&ind))!=-1)
		if(c=='l')
			std::cout<<"-l "<<optarg<<std::endl;
		else if(c=='h')
			std::cout<<"-h "<<optarg<<std::endl;
		else if(c=='v')
			std::cout<<"-v\n";
	

	return 0;
}
