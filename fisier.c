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
#pragma pack(1)
typedef struct {
    uint16_t signature;  // BMP Signature (should be 'BM' for BMP files)
    uint32_t fileSize;   // Size of the BMP file
    uint32_t reserved;   // Reserved, should be 0
    uint32_t dataOffset; // Offset to the start of the pixel data

    // DIB Header
    uint32_t headerSize; // Size of the DIB header
    int32_t width;       // Width of the image
    int32_t height;      // Height of the image
    uint16_t planes;     // Number of color planes (should be 1)
    uint16_t bitsPerPixel; // Number of bits per pixel
} BMPHeader;
#pragma pack(1)

struct stat var;


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

int main(int argn,char *argv[])
{
  if(argn!=2)
    {
       perror("eroare\n");
       exit(-1);
    }
  if(lstat(argv[1],&var)!=-1)
    {
      if(!S_ISREG(var.st_mode))
	{
	  perror("nu e fisier\n");
	  exit(-1);
	}
      else
	{
	  printf("succes\n");
	}
    }
  else
    {
    perror("nicio informatie despre fisier");
    exit(-1);
    }
  //creare statistics.txt
  int fis=open("statistics.txt",O_RDWR|O_CREAT, S_IRUSR|S_IWUSR|S_IXUSR|S_IROTH|S_IWOTH|S_IROTH );
  if(fis==-1)
    {
      perror("nu s-a putut crea fisierul");
      exit(-1);
    }
  else
    printf("fisierul este deschis");



  //citire header
  BMPHeader bmpHeader;
    int bmpFile = open(argv[1], O_RDONLY);
    if (bmpFile == -1) {
        perror("error BMP file");
        close(fis);
        exit(-1);
    }

    int bytesRead = read(bmpFile, &bmpHeader, sizeof(BMPHeader));
    if (bytesRead != sizeof(BMPHeader) || bmpHeader.signature != 0x4D42) {
        perror("invalid BMP file");
        close(fis);
        close(bmpFile);
        exit(-1);
    }

    int imageWidth = bmpHeader.width;
    int imageHeight = bmpHeader.height;

    char buffer[1024];
    int bytes_written;

    bytes_written = sprintf(buffer, "Nume fisier: %s\n", argv[1]);
    write(fis, buffer, bytes_written);

    bytes_written = sprintf(buffer, "Inaltime: %d\n",imageHeight);
    write(fis, buffer, bytes_written);

    bytes_written = sprintf(buffer, "Lungime: %d\n",imageWidth);
    write(fis, buffer, bytes_written);

    bytes_written = sprintf(buffer, "Dimensiune: %ld bytes\n", (long)var.st_size);
    write(fis, buffer, bytes_written);

    bytes_written = sprintf(buffer, "Identificatorul utilizatorului: %d\n", var.st_uid);
    write(fis, buffer, bytes_written);

    time_t modificationTime = var.st_mtime;
    struct tm *timeInfo = localtime(&modificationTime);
    bytes_written = strftime(buffer, sizeof(buffer), "Timpul ultimei modificari: %d.%m.%Y\n", timeInfo);
    write(fis, buffer, bytes_written);

    bytes_written = sprintf(buffer, "Contorul de legaturi: %lu\n", (unsigned long)var.st_nlink);
    write(fis, buffer, bytes_written);

     char user_permissions_str[4];
     strcpy(user_permissions_str, mode_to_str(var.st_mode & S_IRWXU));
   
    bytes_written = sprintf(buffer, "Drepturi de acces user: %s\n", user_permissions_str);
    write(fis, buffer, bytes_written);

    
    char group_permissions_str[4];
    strcpy(group_permissions_str, mode_to_str(var.st_mode & S_IRWXG));
    bytes_written = sprintf(buffer, "Drepturi de acces grup: %s\n", group_permissions_str);
    write(fis, buffer, bytes_written);

    char others_permissions_str[4];
    strcpy(others_permissions_str, mode_to_str(var.st_mode & S_IRWXO));
    bytes_written = sprintf(buffer, "Drepturi de acces altii: %s\n", others_permissions_str);
    write(fis, buffer, bytes_written);

    
    close(fis);
    close(bmpFile);
  
 
  return 0;
}
