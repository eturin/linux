#include <stdio.h>
#include <unistd.h>
int main(){
	printf("1");
	fork();
	return 0;
}
