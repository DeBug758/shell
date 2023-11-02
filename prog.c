#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

void analysis(char* buff){
	int i=0, inFlag=0, amp = 0,line = 0, rStr=0, cnt=0, space=0;
	char ch;
	while((ch = buff[i]) != '\n'){
		if(ch == '&' && !inFlag){
			if(amp < 1){
				amp++;
			}
			else{
				printf("Write ONE ampersand on the END!");
				return;
			}
		}
		else if(ch == '|' && !inFlag){
			if(line < 1){
				line++;
			}
			else{
				printf("Write ONE | !");
				return;
			}
		}
		else if(ch == '>' && !inFlag){
			rStr++;
		}
		else if(ch == '\"'){
			if(inFlag){
				inFlag=0;
			}
			else{
				inFlag=1;
			}
		}
		i++;
	}
	pid_t pid;
	char word[100], arg[100];
	if(amp){
		pid = fork();
		if(pid == -1){
			perror("Child creation Error");
			exit(1);
		}
		if(pid == 0){ // Son
			i=0, inFlag=0;
			cnt = 0, space = 0;

			while((ch = buff[i]) != '&'){
				if(ch == ' ' && !inFlag){
					word[cnt]='\0';
					cnt=0;
					space = 1;
				}
				else if(!space){
					word[cnt++]=buff[i];
				}
				else if(space){
					arg[cnt++]=buff[i];
				}
				i++;
			}
			if(cnt!=0){
				word[cnt]='\0';
				cnt=0;
			}
			arg[cnt]='\0';
			if(cnt != 0){
				execlp(word, word, NULL);
			}
			else{
				execlp(word, word, arg, NULL);
			}
			exit(1);
		}

		else{ // Father
			int status;
			wait(&status);
			printf("Status: ", WEXITSTATUS(status));
			return;
		}
	}
	else if(line){
		i=0, inFlag=0, cnt=0, space=0;
		while((ch = buff[i]) != '|'){
			if(ch == ' ' && !inFlag){
				word[cnt]='\0';
				cnt=0;
				space=1;
			}
			else if(!space){
				word[cnt++]=buff[i];
			}
			else if(space){
				arg[cnt++]=buff[i];
			}
			i++;
		}
		if(cnt!=0){
			word[cnt]='\0';
			cnt=0;
		}
		arg[cnt]='\0';
		pid = fork();
		if(pid==0){
			if(cnt!=0){
				execlp(word, word, arg, NULL);
			}
			else{
				puts("First NO");
				execlp(word, word, NULL);
			}
			exit(1);
		}
		else{
			i++, inFlag=0, cnt=0, space=0;
			while((ch = buff[i]) != '\n'){
				if(ch == ' ' && !inFlag){
					word[cnt]='\0';
					cnt=0;
					space=1;
				}
				else if(!space){
					word[cnt++]=buff[i];
				}
				else if(space){
					arg[cnt++]=buff[i];
				}
				i++;
			}
		}
		if(cnt!=0){
			word[cnt]='\0';
			cnt=0;
		}
		arg[cnt]='\0';
		wait();
		pid=fork();
		if(pid==0){
			if(cnt!=0){
				execlp(word, word, arg, NULL);
			}
			else{
				puts("Second NO");
				execlp(word, word, NULL);
			}
			exit(1);
		}
		else{
			
			return;
		}
	}
	else{
		i=0, inFlag=0, cnt=0, space=0;
		while((ch=buff[i]) != '\n'){
			if(ch == ' ' && !inFlag){
				word[cnt]='\0';
				cnt=0;
				space=1;
			}
			else if(!space){
				word[cnt++]=buff[i];
			}
			else if(space){
				arg[cnt++]=buff[i];
			}
			i++;
		}
		if(cnt != 0){
			word[cnt]='\0';
			cnt = 0;
		}
		if(cnt!=0){
			word[cnt]='\0';
			cnt=0;
		}
		arg[cnt]='\0';
		pid=fork();
		if(pid==0){
			if(cnt!=0){
				puts("YES ARG");
				execlp(word, word, arg, NULL);
			}
			else{
				puts("NO ARG");
				execlp(word, word, NULL);
			}
			exit(1);
		}
		else{
			return;
		}
	}
}

void cons_read(char* history){
	int fd;
	if ((fd = open(history, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1){
		printf("ERROR FILE");
		exit(1);
	}
	int flag = 1, start = 0;
	char buffer[100+1], ch;
	int cnt = 0;
	while((ch = getchar()) != EOF){
		if(ch == ' '){
			if(start && flag){
				flag = 0;
				buffer[cnt++]=ch;
			}
		}
		else if(ch == '\n'){
			if(buffer[cnt-1] == ' '){
				buffer[cnt-1]='\n';
			}
			else{
				buffer[cnt++]='\n';
			}
			analysis(buffer);
			write(fd, buffer, cnt);
			cnt=0;
			flag=1;
			start=0;;
		}
		else{
			start=1;
			flag=1;
			buffer[cnt++]=ch;
		}
	}
	if(cnt > 0){
		buffer[cnt++]='\n';
		analysis(buffer);
		write(fd, buffer, cnt);
	}
	close(fd);
}

int main(int argc, char* argv[]){
	//system("ls -la");
	cons_read("result.txt");
	return 0;
}
