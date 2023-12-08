#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<pwd.h>
#include<string.h>
#include<time.h>
#include<dirent.h>
#include<unistd.h>
#include<ctype.h>

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
int numar_linii;
int width, height;
int count_procese=0;
int prop_corecte=0;



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

void Permisiuni(int fis) //RWX
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
  bytes_written = sprintf(buffer, "Drepturi de acces altii: %s\n", others_permissions_str);
  write(fis, buffer, bytes_written);
}


void Fisier(char name[])  //fisier obisnuit
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

void Fisier_BMP(char name[], char path_name[])  //fisier obisnuit
{
  int fisBMP=open(path_name, O_RDONLY);
  if(fisBMP==-1)
    {
      perror("eroare la deschiderea fisierului BMP");
      exit(-1);
    }
  
 

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

void Director(char name[])  //director
{
  bytes_written = sprintf(buffer, "Nume fisier: %s\n", name);
  write(fis, buffer, bytes_written);

  bytes_written = sprintf(buffer, "Identificatorul utilizatorului: %d\n", intrare.st_uid);
  write(fis, buffer, bytes_written);
  
}

void Legatura_simbolica(char name[]) //legatura simbolica
{
  
  bytes_written = sprintf(buffer, "Nume legatura: %s\n", name);
  write(fis, buffer, bytes_written);
  
  bytes_written = sprintf(buffer, "Dimensiune legatura: %ld bytes\n", (long)intrare.st_size);
  write(fis, buffer, bytes_written);

  bytes_written = sprintf(buffer, "Dimensiune fisier: %ld bytes\n", (long)fisier_leg_simbolica.st_size);
  write(fis, buffer, bytes_written);
    
} 


int countLines(int fisier) // numara liniile dintr-un fisier
{  
   numar_linii=0;
    lseek(fisier, 0, SEEK_SET);

    char ch;
    while (read(fisier, &ch, 1) > 0) {
        if (ch == '\n') {
            numar_linii++;
        }
    }
    return numar_linii;
}


void readBMP(char path_name[])  //citeste fisierul .bmp si transforma poza in alb-negru
{
  int fisBMP=open(path_name, O_RDWR);
  if(fisBMP==-1)
    {
      perror("eroare la deschiderea fisierului BMP\n");
      exit(-1);
    }
  lseek(fisBMP,54,SEEK_SET);

   for (int row = 0; row < height; ++row) {
    for (int col = 0; col < width; ++col) {
      char pixel[3];
      read(fisBMP, pixel, sizeof(pixel));

      // formula pentru obtinerea culorii gri
      char grayscale = (char)(
          0.299 * pixel[2] + 0.587 * pixel[1] + 0.114 * pixel[0]
      );

      // inlocuim in locul pixelilor rosu albastru vedre culoarea grii
      pixel[0] = grayscale; // Albastru
      pixel[1] = grayscale; // Verde
      pixel[2] = grayscale; // Rosu

      // mutam cursorul cu 3 pozitii inapoi pentru a putea salva modificarile
      lseek(fisBMP, -3, SEEK_CUR);
      write(fisBMP, pixel, sizeof(pixel));
    }
  }


  close(fisBMP);
    
}

int main(int argn,char *argv[])
{
  int pipefd_result[2];  //pipe-ul fiu2 - parinte
  int pipefd[2]; // pipe-ul fiu1 - fiu2
  
  if(argn!=4)
    {
      perror("eroare nr de argumente\n");
      exit(-1);
    }
  

  if(lstat(argv[1],&var)!=-1)  //directorul intrare
    {
      if(!S_ISDIR(var.st_mode))
	{
	  perror("nu e director de intrare\n");
	  exit(-1);
	}
      else
	{
	  printf("exista director de intrare\n");
	}
    }
  else
    {
      perror("nicio informatie despre director intrare\n");
      exit(-1);
    }

 

  if(lstat(argv[2],&var2)!=-1)  //director iesire
    {
      if(!S_ISDIR(var2.st_mode))
	{
	  perror("nu e director de iesire\n");
	  exit(-1);
	}
      else
	{
	  printf("exista director de iesire\n");
	}
    }
  else
    {
      perror("nicio informatie despre director iesire\n");
      exit(-1);
    }
  
  char caracter = argv[3][0];;
  if(!isalpha(caracter) || islower(caracter)) //verificare daca al patrulea argument este caracter
    {
      perror("argumentul 3 nu este caracter");
      exit(-1);
    }
 
  if(!(dir2=opendir(argv[2])))  //deschidere director iesire
    {
      perror("eroare la deschiderea directorului iesire\n");
      exit(-1);
      
    }
  
  int fisier=open("statistics.txt",O_RDWR|O_CREAT, S_IRUSR|S_IWUSR|S_IXUSR|S_IROTH|S_IWOTH|S_IROTH ); //creare si deschidere  statistics.txt
  if(fisier==-1)
    {
      perror("Nu s-a putut crea fisierul statistics.txt\n");
      exit(-1);
    }
  else
    printf("fisierul statistics.txt este deschis\n");
  
  char path_dir[1026];  //calea catre director
  char new_file_name[1024];
 
  if(!(dir=opendir(argv[1]))) //parcurgere director
    {
      perror("eroare la deschiderea directorului\n");
      exit(-1);
      
    }
  
  char path_name[258];// calea fiecarei intrari din director
  
  while((dirr=readdir(dir)))
    {
      if(strcmp(dirr->d_name,".")!=0 && strcmp(dirr->d_name,"..")!=0)
	{
	 
	      numar_linii = 0;
	      sprintf(new_file_name,"%s%s",dirr->d_name,"_statistics.txt");
	      sprintf(path_dir,"%s/%s",argv[2],new_file_name);
	      
	      fis=open(path_dir,O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IXUSR|S_IROTH|S_IWOTH|S_IXOTH); //deschidere si creare fisier _statistics.txt pt fiecare intrare din director
	      if(fis==-1)
		{
		  perror("nu s-a putut crea fisierul _statistics.tx\n");
		  exit(-1);
		}
	      else
		{
		  printf("fisierul _statistics.txt este deschis\n");
		}
	 
	      sprintf(path_name,"%s/%s",argv[1],dirr->d_name);
	      if(lstat(path_name,&intrare)!=-1)
		{
	      
		  if(S_ISDIR(intrare.st_mode))
		    {
		      int pid_director=fork();  //proces pentru a scrie in fisierul _statistics.txt
		      if(pid_director==0)
			{
			  Director(dirr->d_name);
			  Permisiuni(fis);
			  int nr=countLines(fis);
			  exit(nr);
			}
		       count_procese++;
		    }
	      
		  if(S_ISREG(intrare.st_mode) && strstr(path_name,".bmp"))
		    {
		      int pid_bmp=fork();  //proces pentru a scrie in fisierul _statistics.txt
		      if(pid_bmp==0)
			{
			  Fisier_BMP(dirr->d_name, path_name);
			  Permisiuni(fis);
			  int nr=countLines(fis);
			  exit(nr);
			}
		       count_procese++;
		      
		      int pid2=fork(); //proces care face poza gri
		      if(pid2==0)
			{
			  readBMP(path_name);
			  exit(0);
			}
		       count_procese++;
		    }
		  else
		    {
		      if(S_ISREG(intrare.st_mode))
			{
			  
			  
			  if(pipe(pipefd)==-1)
			    {
			      perror("eroare la crearea pipe-ului fiu1-fiu2");
			      exit(-1);
			    }
			  
			  int pid_fisier_obisnuit=fork(); //proces pentru a scrie in fisierul _statistics.txt  + proces fiu1-fiu2
			  if(pid_fisier_obisnuit==0)
			    {
			      close(pipefd[0]); //inchidem capatul de citire 
			      Fisier(dirr->d_name);
			      Permisiuni(fis);
		              int nr=countLines(fis);
			      
			      
			      dup2(pipefd[1], 1);
			      execlp("cat", "cat", path_name, NULL);
			      perror("eroare la cat\n");
			      exit(nr);
			    }
			  count_procese++;
			  close(pipefd[1]);//nu scriem in procesul parinte
			  
			  
			  if (pipe(pipefd_result) == -1)  // al doilea proces fiu2-parinte
			    {
			      perror("Eroare la crearea pipe-ului fiu2-parinte");
			      exit(EXIT_FAILURE);
			    }
			  
			  int pid_fiu_script = fork();  //al doilea proces
			   if(pid_fiu_script == 0)
			     {
			      
			       close(pipefd_result[0]);//incidem capatul de citire
			       dup2(pipefd[0], STDIN_FILENO);  //redirectam intrarea 
			
			       dup2(pipefd_result[1],STDOUT_FILENO); 

			       execlp("./script.sh", "./script.sh",&caracter, NULL);
			       perror("eroare la script\n");
			       exit(0);
			     }
			   count_procese++;
			   close(pipefd_result[1]);
			   char result_buffer[1024];
			   read(pipefd_result[0], result_buffer, sizeof(result_buffer));
			   close(pipefd_result[0]);
			   prop_corecte =prop_corecte+atoi(result_buffer);
			}
		    }
		}
	      
	      if(S_ISLNK(intrare.st_mode))
		{
		  if(stat(path_name, &fisier_leg_simbolica)!=-1)
		    {
		      if(S_ISREG(fisier_leg_simbolica.st_mode))
			{
			  int pid_link=fork();//proces pentru a scrie in fisierul _statistics.txt
			  if(pid_link==0)
			    {
			      Legatura_simbolica(dirr->d_name);
			      Permisiuni(fis);
			      int nr=countLines(fis);
			      exit(nr);
			    }
			  count_procese++;
			}
		    }
		}
	      close(fis);
	}
    } 
  int i, wstatus;
  for(i=0;i<count_procese;i++)
    {
      
      int w = wait(&wstatus);
      if (w == -1)
	{
	  perror("waitpid");
	  exit(EXIT_FAILURE);
	}
      else
	{ 
	  bytes_written = sprintf(buffer, "Numarul de linii: %d\n", WEXITSTATUS(wstatus));
	  write(fisier, buffer, bytes_written);
	  bytes_written = sprintf(buffer, "S-a încheiat procesul cu pid-ul %d și codul %d\n\n", w,  WEXITSTATUS(wstatus));
	  write(fisier, buffer, bytes_written);
	}
      
    }
  bytes_written = sprintf(buffer, "Au fost identificate in total %d propozitii corecte care contin caracterul %c\n", prop_corecte, caracter);
  write(fisier, buffer, bytes_written);
  
  close(fisier);  // inchidere statistics.txt
  close(pipefd[1]); 
  closedir(dir); //inchidere director de intrare
  closedir(dir2); //inchidere director iesire
  return 0;
}
