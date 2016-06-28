#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <assert.h>
#include <vector>


int main(int cnt, char ** arg){
	//читаем команду
	std::string str;
	std::getline(std::cin,str);

	//разбираем ее на команды, порождая процессы
	std::vector<std::string> v;
	v.push_back("");
	bool is_ok=str.length()>0;
	for(size_t i=0,len=str.length();is_ok && i<=len;++i)
		if(i==len || str[i]=='|'){
			//нужно заменить родителя на вызов и направить результат вызова потомку через канал
			int fd[2];          
                        assert(-1!=pipe(fd));
			int rez=fork();
			if(rez){
				//готовим массив аргументов
				int cnt=v.size();
				const char ** arg=new const char*[cnt+1];
				for(int j=0;j<cnt;++j)
					if(v[j].length())
						arg[j]=v[j].c_str();
					else 
						arg[j]=NULL;
				arg[cnt]=NULL;

				//родитель будет писать в канал
				close(STDOUT_FILENO);
				assert(dup2(fd[1],STDOUT_FILENO)==STDOUT_FILENO); //дублируем дескриптор с нужным номером
				close(fd[0]);close(fd[1]);
				execvp(arg[0],(char * const*)arg); //замена исполняемой программы
				is_ok=false;
			}else{
				v.clear();
				v.push_back("");
				//потомок будет читать из канала и дальше разбирать строку команды
				close(STDIN_FILENO);
				assert(STDIN_FILENO==dup2(fd[0],STDIN_FILENO));   //дублируем дескриптор с нужным номером
				close(fd[0]);close(fd[1]);
			}
		}else if(str[i]==' ' && v[v.size()-1].length()){
			v.push_back("");
		}else if(str[i]!=' ')
			v[v.size()-1]+=str[i];

	//читаем конечный вывод последнего родителя
	if(is_ok){
		std::ofstream fout("result.out");
		char c;
		while(std::cin.get(c)){
			fout <<c;
			std::cout <<c;
		}
		fout.close();
	}
	
	return 0;
}
