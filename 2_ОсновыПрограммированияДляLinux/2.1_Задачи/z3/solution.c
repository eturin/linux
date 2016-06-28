#include <stdio.h>
#include <getopt.h>

int main(int argc, char ** argv){
	/*короткие параметры*/
	const char * short_opt="q:iv";
	/*длинные параметры*/
	struct option long_opt[]={
		{"query",required_argument,NULL,'q'},
		{"longinformationrequest",no_argument,NULL,'i'},
		{"version",no_argument,NULL,'v'}
	};

	/*проверка параметров*/
	int c,ind,rez=0; 
	while((c=getopt_long(argc,argv,short_opt,long_opt,&ind))!=-1) 
		if(c=='q') 
			rez=1;
	printf(rez? "+":"-");

	return 0;
}
