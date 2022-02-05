/* unlnk.c. The program shows, that it is possible to use a file after
 * its directory link was removed (and the contents of the file cannot
 * be accessed by other processes).
*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
void p(char *s){
	fprintf(stderr,"\n%s: press a key",s); getchar();
}
int main(int argc, char *argv[]){
char buf[20];
int fd, n;
struct stat sb;
	if((fd=open("tmp",O_RDWR|O_CREAT,0600))<0){
		perror("open:1"); return 1;
	}
	if(write(fd,"text\n",6)<6){
		perror("write 1"); return 2;
	}
	if(fstat(fd,&sb)<0){
		perror("fstat 1"); return 3;
	}
	fprintf(stderr,"Current size of tmp file=%lu\n",(unsigned long)sb.st_size);
	close(fd);
	p("ls -l tmp");
	system("ls -l tmp");
	p("cat tmp");
	system("cat tmp");
	if((fd=open("tmp",O_RDWR))<0){
		perror("open:2"); return 4;
	}
	p("unlink");
	if(unlink("tmp")<0){
		perror("unlink"); return 5;
	}
	p("ls -l");
	system("ls -l");
	p("reading contents of a file, which apparently was removed");
	while((n=read(fd,buf,sizeof(buf)))>0)
		if(write(1,buf,n)<n){
			perror("write 1");
			return 6;
		}
	if(n<0){
		perror("read 1"); return 7;
	}
/* modifying contents to the file */
	if(lseek(fd,(off_t)4,SEEK_SET)==(off_t)-1){ /* positioning the file */
		perror("lseek 1"); return 8;
	}
	if(write(fd," added\n",8)!=8){
		perror("write 2"); return 9;
	}
	if(fstat(fd,&sb)<0){
		perror("fstat");
		return 10;
	}
	p("reading contents of the file after modification");
	if(lseek(fd,(off_t)0,SEEK_SET)==(off_t)-1){ /* rewinding the file */
		perror("lseek 2"); return 11;
	}
	while((n=read(fd,buf,sizeof(buf)))>0)
		if(write(1,buf,n)<n){
			perror("write 3"); return 12;
		}
	if(n<0){
		perror("read 2"); return 13;
	}
	return 0;
}
