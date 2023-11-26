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
  bytes_written = sprintf(buffer, "Drepturi de acces altii: %s\n", others_permissions_str);
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


void countLines(int fisier, int linii_pipe_fd[]) {
    lseek(fisier, 0, SEEK_SET);

    char ch;
    while (read(fisier, &ch, 1) > 0) {
        if (ch == '\n') {
            numar_linii++;
        }
    }

    write(linii_pipe_fd[1], &numar_linii, sizeof(numar_linii));
}


void readBMP(char path_name[])
{
  int fisBMP=open(path_name, O_RDWR);
  if(fisBMP==-1)
    {
      perror("eroare la deschiderea fisierului BMP");
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
      pixel[0] = grayscale; // Blue
      pixel[1] = grayscale; // Green
      pixel[2] = grayscale; // Red

      // mut cursorul cu 3 pozitii inapoi pentru a putea salva modificarile
      lseek(fisBMP, -3, SEEK_CUR);
      write(fisBMP, pixel, sizeof(pixel));
    }
  }


  close(fisBMP);
    
}

int main(int argn,char *argv[])
{
  if(argn!=4)
    {
      perror("eroare nr de argumente\n");
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
  char caracter = argv[3][0];
  if(!isalpha(caracter) || islower(caracter))
    {
      perror("argumentul 3 nu este caracter");
      exit(-1);
    }
  printf("%c\n\n\n",caracter);
  
  //deschidere director iesire

   if(!(dir2=opendir(argv[2])))
    {
      perror("eroare la deschiderea directorului iesire\n");
      exit(-1);
      
    }
  
  //creare statistics.txt


   int fisier=open("statistics.txt",O_RDWR|O_CREAT, S_IRUSR|S_IWUSR|S_IXUSR|S_IROTH|S_IWOTH|S_IROTH );
  if(fisier==-1)
    {
      perror("Nu s-a putut crea fisierul");
      exit(-1);
    }
  else
    printf("fisierul este deschis");

   char path_dir[1026];
   char new_file_name[1024];
 



   //creare comunicare intre parinte si copii prin pipes
  int linii_pipe_fd[2];
  int pid_pipe_fd[2];
  int exit_code_pipe_fd[2];

  int fisiere_pipe[2], fisiere_pipe2[2]; // pipe fiu1-fiu2  pipe fiu2-parinte
  pid_t pid_fisier_o=-1;//pid fiu 1
  pid_t pid_fisier_o2=-1;//pid fiu 2

  if (pipe(linii_pipe_fd) == -1 || pipe(pid_pipe_fd) == -1 || pipe(exit_code_pipe_fd) == -1 ) {
    perror("eroare creare pipe");
    exit(-1);
  }

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
	  pid_t pid=fork();
	  if(pid==0)
	    {
	      close(linii_pipe_fd[0]); 
	      close(pid_pipe_fd[0]);
	      close(exit_code_pipe_fd[0]);  // inchidere capete citire parinte
	      numar_linii = 0;
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
		      printf("%d %d\n",width,height);
		      pid_t pid2=fork();
		      if(pid2==0)
			{
			  readBMP(path_name);
			  exit(0);
			}
		    }
		  else
		    {
		      if(S_ISREG(intrare.st_mode))
			{
			  Fisier(dirr->d_name);
			  Permisiuni(fis);
			 
			  
			  if(pipe(fisiere_pipe)==-1)
			    {
			      printf("eroare la crearea comunicarii intre cele 2 procese fiu");
			      exit(-1);
			    }
			   if( pipe(fisiere_pipe2)==-1)
			    {
			      printf("eroare la crearea comunicarii intre procesul fiu 2 si parinte");
			      exit(-1);
			      }
			   pid_fisier_o=fork();
			  
			  if(pid_fisier_o==0)
			    {
			      close(fisiere_pipe[0]);//inchidere citire (fiu 2)
			      //close(fisiere_pipe2[0]);//inchidere citire (parinte)
			      dup2(fisiere_pipe[1],1); //redirectioneaza iesirea standard catre pipe
			      execlp("cat","cat",path_name,NULL);
			      perror("Eroare la exec pentru fiul 1\n");
			      exit(-1);
			      
			    }
			  
			  
			  pid_fisier_o2=fork();
			  if(pid_fisier_o2==0)
			    {
			      close(fisiere_pipe[1]);//inchide capatul de scriere (fiu 1)
			      //close(fisiere_pipe2[0]);//inchide capatul de citire (parintele)

			      dup2(fisiere_pipe[0],0); //redirectionare la stdin a capatului de citire (fiu2)
			      close(fisiere_pipe2[0]); 
			      dup2(fisiere_pipe2[1],1);//redirectionare la stdout a capatului de scriere (fiu2)
			      execlp("bash","bash","script.sh",argv[3],NULL);

			      perror("Eroare la exec pentru fiul 2\n");
			      exit(-1);
			      
			      
			    }
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
	      printf("aici este codul copilului\n");
	      countLines(fis, linii_pipe_fd);
	      pid_t copil_pid = getpid();
	      int exit_code = 0;    

	      write(pid_pipe_fd[1], &copil_pid, sizeof(copil_pid));
	      write(exit_code_pipe_fd[1], &exit_code, sizeof(exit_code));

	  
	      
	      
	      
	      close(fis);
	      close(linii_pipe_fd[1]); 
	      close(pid_pipe_fd[1]);   
	
	      
	      
	      exit(0);
	    }
	  
        } 
      
    }
  close(fisiere_pipe[0]);//inchidere fisier citire fiu1
  close(fisiere_pipe[1]); // inchidere fisier scriere fiu1
  close(fisiere_pipe2[1]);//inchidere scriere fiu2
  
  close(linii_pipe_fd[1]); 
  close(pid_pipe_fd[1]);  
  close(exit_code_pipe_fd[1]); //inchidere capat scriere copil
  waitpid(pid_fisier_o, NULL, 0);
    waitpid(pid_fisier_o2, NULL, 0);
  int numar_prop_corecte=-1;
  read(fisiere_pipe2[0],&numar_prop_corecte,sizeof(numar_prop_corecte));
    
  bytes_written= sprintf(buffer,"Numarul de propozitii corecte este care contin caracterul %c este: %d\n", caracter,numar_prop_corecte);
  write(fisier, buffer, bytes_written);
  close(fisiere_pipe2[0]);


  
  while ((wait(NULL)) > 0) {
    read(linii_pipe_fd[0], &numar_linii, sizeof(numar_linii));
    pid_t copil_pid;
    int exit_code;
    read(pid_pipe_fd[0], &copil_pid, sizeof(copil_pid));
    read(exit_code_pipe_fd[0], &exit_code, sizeof(exit_code));
    
   
    bytes_written = sprintf(buffer, "Numarul de linii: %d\n", numar_linii);
    write(fisier, buffer, bytes_written);
    bytes_written = sprintf(buffer, "S-a încheiat procesul cu pid-ul %d și %d\n\n", copil_pid, exit_code);
    write(fisier, buffer, bytes_written);
  }
  
  close(fisier);
  closedir(dir);
  closedir(dir2); 
  return 0;
}
