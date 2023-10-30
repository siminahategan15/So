#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>

struct stat var;

int main(int argn,char *argv[])
{
  if(argn!=2)
    {
       perror("eroare");
       exit(-1);
    }
  if(lstat(argv[1],&var)!=-1)
    {
      if(!S_ISREG(var.st_mode))
	{
	  perror("not a file");
	  exit(-1);
	}
      else
	{
	  printf("succes");
	}
    }
  else
    perror("not a file2");

  int fis=open("statistics.txt",O_RDWR|O_CREAT, S_IRUSR|S_IWUSR|S_IXUSR|S_IROTH|S_IWOTH|S_IROTH );
  if(fis==-1)
    {
      perror("Nu s-a putut crea fisierul");
      exit(-1);
    }
  else
    printf("fisierul este deschis");
  return 0;
}
