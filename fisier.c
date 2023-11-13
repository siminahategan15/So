#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<pwd.h>
#include<string.h>
#include<time.h>
#include<dirent.h>


struct stat var;
struct stat var2;
struct stat intrare;
struct stat fisier_leg_simbolica;
struct dirent *dirr=NULL;
DIR *dir=NULL;
DIR *dir2=NULL;
int fis;
char buffer[1024];
int bytes_written;



char* mode_to_str(mode_t mode) {
  switch (mode) {
  case S_IRUSR | S_IWUSR | S_IXUSR:
  case S_IRGRP | S_IWGRP | S_IXGRP:
  case S_IROTH | S_IWOTH | S_IXOTH:
    return "RWX";
  case S_IRUSR | S_IWUSR:
  case S_IRGRP | S_IWGRP:
  case S_IROTH | S_IWOTH:
    return "RW-";
  case S_IRUSR | S_IXUSR:
  case S_IRGRP | S_IXGRP:
  case S_IROTH | S_IXOTH:
    return "R-X";
  case S_IRUSR:
  case S_IRGRP:
  case S_IROTH:
    return "R--";
  case S_IWUSR | S_IXUSR:
  case S_IWGRP | S_IXGRP:
  case S_IWOTH | S_IXOTH:
    return "-WX";
  case S_IWUSR:
  case S_IWGRP:
  case S_IWOTH:
    return "-W-";
  case S_IXUSR:
  case S_IXGRP:
  case S_IXOTH:
    return "--X";
  default:
    return "---";
  }
}

void Permisiuni(int fis)
{
  char user_permissions_str[4];
  strcpy(user_permissions_str, mode_to_str(intrare.st_mode & S_IRWXU));
  bytes_written = sprintf(buffer, "Drepturi de acces user: %s\n", user_permissions_str);
  write(fis, buffer, bytes_written);

  char group_permissions_str[4];
  strcpy(group_permissions_str, mode_to_str(intrare.st_mode & S_IRWXG));
  bytes_written = sprintf(buffer, "Drepturi de acces grup: %s\n", group_permissions_str);
  write(fis, buffer, bytes_written);

  char others_permissions_str[4];
  strcpy(others_permissions_str, mode_to_str(intrare.st_mode & S_IRWXO));
  bytes_written = sprintf(buffer, "Drepturi de acces altii: %s\n\n", others_permissions_str);
  write(fis, buffer, bytes_written);
}


void Fisier(char name[])
{
  
  bytes_written = sprintf(buffer, "Nume fisier: %s\n", name);
  write(fis, buffer, bytes_written);

  bytes_written = sprintf(buffer, "Dimensiune: %ld bytes\n", (long)intrare.st_size);
  write(fis, buffer, bytes_written);

  bytes_written = sprintf(buffer, "Identificatorul utilizatorului: %d\n", intrare.st_uid);
  write(fis, buffer, bytes_written);

  struct tm *timeInfo = localtime(&(var.st_mtime));
  bytes_written = strftime(buffer,sizeof(buffer), "Timpul ultimei modificari: %d.%m.%Y\n",timeInfo);
  write(fis, buffer, bytes_written);

  bytes_written = sprintf(buffer, "Contorul de legaturi: %lu\n", (unsigned long)intrare.st_nlink);
  write(fis, buffer, bytes_written);

    

}

void Fisier_BMP(char name[], char path_name[])
{
  int fisBMP=open(path_name, O_RDONLY);
  if(fisBMP==-1)
    {
      perror("eroare la deschiderea fisierului BMP");
      exit(-1);
    }
  
  int width, height;

  lseek(fisBMP, 18, SEEK_SET); 
  read(fisBMP, &width, sizeof(int)); 

  lseek(fisBMP, 22, SEEK_SET); 
  read(fisBMP, &height, sizeof(int));

  
  bytes_written = sprintf(buffer, "Nume fisier: %s\n", name);
  write(fis, buffer, bytes_written);

  bytes_written = sprintf(buffer, "Inaltime: %d\n",height);
  write(fis, buffer, bytes_written);

  bytes_written = sprintf(buffer, "Lungime: %d\n",width);
  write(fis, buffer, bytes_written);

  bytes_written = sprintf(buffer, "Dimensiune: %ld bytes\n", (long)intrare.st_size);
  write(fis, buffer, bytes_written);

  bytes_written = sprintf(buffer, "Identificatorul utilizatorului: %d\n", intrare.st_uid);
  write(fis, buffer, bytes_written);

  struct tm *timeInfo = localtime(&(var.st_mtime));
  bytes_written = strftime(buffer,sizeof(buffer),"Timpul ultimei modificari: %d.%m.%Y\n",timeInfo);
  write(fis, buffer, bytes_written);

  bytes_written = sprintf(buffer, "Contorul de legaturi: %lu\n", (unsigned long)intrare.st_nlink);
  write(fis, buffer, bytes_written);

  close(fisBMP);

}

void Director(char name[])
{
  bytes_written = sprintf(buffer, "Nume fisier: %s\n", name);
  write(fis, buffer, bytes_written);

  bytes_written = sprintf(buffer, "Identificatorul utilizatorului: %d\n", intrare.st_uid);
  write(fis, buffer, bytes_written);
  
}

void Legatura_simbolica(char name[])
{
  
  bytes_written = sprintf(buffer, "Nume legatura: %s\n", name);
  write(fis, buffer, bytes_written);
  
  bytes_written = sprintf(buffer, "Dimensiune legatura: %ld bytes\n", (long)intrare.st_size);
  write(fis, buffer, bytes_written);

  bytes_written = sprintf(buffer, "Dimensiune fisier: %ld bytes\n", (long)fisier_leg_simbolica.st_size);
  write(fis, buffer, bytes_written);
    
} 


int main(int argn,char *argv[])
{
  if(argn!=3)
    {
      perror("eroare\n");
      exit(-1);
    }
  //directorul intrare
  
  if(lstat(argv[1],&var)!=-1)
    {
      if(!S_ISDIR(var.st_mode))
	{
	  perror("nu e director de intrare\n");
	  exit(-1);
	}
      else
	{
	  printf("succes\n");
	}
    }
  else
    {
      perror("nicio informatie despre director intrare\n");
      exit(-1);
    }

  //director iesire

  if(lstat(argv[2],&var2)!=-1)
    {
      if(!S_ISDIR(var2.st_mode))
	{
	  perror("nu e director de iesire\n");
	  exit(-1);
	}
      else
	{
	  printf("succes\n");
	}
    }
  else
    {
      perror("nicio informatie despre director iesire\n");
      exit(-1);
    }

  //deschidere director iesire

   if(!(dir2=opendir(argv[2])))
    {
      perror("eroare la deschiderea directorului iesire\n");
      exit(-1);
      
    }
  
  //creare statistics.txt 

   char path_dir[1026];
   char new_file_name[1024];
 
 

  //parcurgere director
  
  
  if(!(dir=opendir(argv[1])))
    {
      perror("eroare la deschiderea directorului\n");
      exit(-1);
      
    }
  
  
  char path_name[258];
  while((dirr=readdir(dir)))
    {
      if(strcmp(dirr->d_name,".")!=0 && strcmp(dirr->d_name,"..")!=0)
	{

	  sprintf(new_file_name,"%s%s",dirr->d_name,"_statistics.txt");
	  sprintf(path_dir,"%s/%s",argv[2],new_file_name);
	  fis=open(path_dir,O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IXUSR|S_IROTH|S_IWOTH|S_IXOTH);
	  if(fis==-1)
	    {
	      perror("nu s-a putut crea fisierul");
	      exit(-1);
	    }
	  else
	    printf("fisierul este deschis\n");
	  
	 
	  
	  
	  sprintf(path_name,"%s/%s",argv[1],dirr->d_name);
	  if(lstat(path_name,&intrare)!=-1)
	    {
	      
	      if(S_ISDIR(intrare.st_mode))
		{
		  
		  Director(dirr->d_name);
		  Permisiuni(fis);
		}
	      
	      if(S_ISREG(intrare.st_mode) && strstr(path_name,".bmp"))
		{
		  Fisier_BMP(dirr->d_name, path_name);
		  Permisiuni(fis);
		}
	      else
		{
		  if(S_ISREG(intrare.st_mode))
		    {
		      Fisier(dirr->d_name);
		      Permisiuni(fis);
		    }
		}
	      
	      if(S_ISLNK(intrare.st_mode))
		{
		  if(stat(path_name, &fisier_leg_simbolica)!=-1)
		    {
		      if(S_ISREG(fisier_leg_simbolica.st_mode))
			{
			  Legatura_simbolica(dirr->d_name);
			  Permisiuni(fis);
			}
		    }
		}
	    }
	  
	}
      
    }
  close(fis);
  closedir(dir);
  return 0;
}
