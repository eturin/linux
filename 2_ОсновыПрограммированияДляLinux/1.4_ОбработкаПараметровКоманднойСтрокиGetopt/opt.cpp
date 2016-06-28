#include <iostream>
#include <unistd.h>

int main(int argvc, char ** argv){
	const char * line="l:h:v?";
	int c;
	while((c=getopt(argvc,argv,line))!=-1)
		if(c=='l')
			std::cout<<"-l "<<optarg<<std::endl;
		else if(c=='h')
			std::cout<<"-h "<<optarg<<std::endl;
		else if(c=='v')
			std::cout<<"-v\n";
		else if(c=='?')
			std::cout<<"-?\n";
	

	return 0;
}
