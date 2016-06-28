void foo(int x){

}



int main(){
	int  m[]={1,2,3,4,5,6,7,8,9,10};
	unsigned size=sizeof(m)/sizeof(int);

	/*последовательная обработка*/
	for(unsigned i=0;i<size;++i)
		foo(m[i]);


	/*параллельная обработка*/
	tbb::parallel_do(0,size,[=](int i){foo(m[i]);});

	return 0;
}
