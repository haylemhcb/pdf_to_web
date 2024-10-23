
/*

  Este programa es cread por Haylem Candelario Bauza.
  Software libre GPLv2
  Cuba 2024.

  Convierte un pdf a una carpeta web con index y estilo
  fiel de paginas de libro.

*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <dirent.h>
#include <time.h>

#define MV_TO_PERSONAL "mv /tmp/out ~/out-html-pdf"


void pdf_photos(const char *pathfile);
void convert_pages_html();
void count_pages(void);


unsigned long int maxpages = 0;

int main(int argc, char *argv[])
{

  char filename[500] = {'\0'};
  char var_env[500] = {'\0'};
  char fmtren[500] = {'\0'};
  char ruta_binarios[PATH_MAX];
  unsigned long int aleat = 0;

  srand(time(NULL));

  aleat = (rand() % 1000) + 1;
  sprintf(fmtren, "/tmp/out-%ld", aleat);

  rename("/tmp/out", fmtren);
  mkdir("/tmp/out", 0777);

  strcpy(var_env, getenv("PATH"));
  strcat(var_env, ":./bin");

  getcwd(ruta_binarios, sizeof(ruta_binarios));
  strcat(ruta_binarios, "/bin");

  setenv("PATH", var_env, 1);

  if(argv[1] != NULL)
  {
    strcpy(filename, argv[1]);
    pdf_photos(filename);
    convert_pages_html();
    system(MV_TO_PERSONAL);
  }
  else
  {
    printf("Uso: %s <pdfPath>\n", argv[0]);
  }

  printf("\n\nNumero de paginas: %ld\n\n", maxpages - 2);


  return 0;
}


void count_pages(void)
{
  struct dirent *entr = NULL;
  DIR *directorio = NULL;

  directorio = opendir("/tmp/out");

  if(directorio != NULL)
  {
    maxpages = 0;
    while((entr = readdir(directorio)) != NULL)
    {
      ++maxpages;
    }
    closedir(directorio);
  }

}

void convert_pages_html()
{
  const char *path = "/tmp/out";
  const char *prefix = "page-";
  unsigned long int npage = 1;
  FILE *fout = NULL;
  char str_num[500] = {'\0'};

  struct dirent *entr = NULL;
  DIR *directorio = NULL;

  count_pages();

  directorio = opendir("/tmp/out");
  if(directorio != NULL)
  {
    fout = fopen("/tmp/out/index.html", "w");

    if(fout == NULL) return;


    fprintf(fout, "<body style=\"background-color: lightgray;\">\n");


    while((entr = readdir(directorio)) != NULL)
    {
      if (entr->d_name[0] != '.') {
        printf("%s\n", entr->d_name);
        /* Agregar entrada html */

        if((strstr(strdup(entr->d_name), "index.html") == NULL))
        {

          if(maxpages > 10)
          {
            if(npage > 0)
             sprintf(str_num, "page-0%ld.png", npage);

            if(npage > 9)
             sprintf(str_num, "page-%ld.png", npage);
          }


          if(maxpages > 100)
          {
            if(npage > 0)
             sprintf(str_num, "page-00%ld.png", npage);

            if(npage > 9)
             sprintf(str_num, "page-0%ld.png", npage);

            if(npage > 99)
             sprintf(str_num, "page-%ld.png", npage);

          }


          if(maxpages > 1000)
          {
            if(npage > 0)
             sprintf(str_num, "page-000%ld.png", npage);

            if(npage > 9)
             sprintf(str_num, "page-00%ld.png", npage);

            if(npage > 99)
             sprintf(str_num, "page-0%ld.png", npage);

            if(npage > 999)
             sprintf(str_num, "page-%ld.png", npage);

          }


          if(((maxpages - 2) == 9) || ((maxpages - 2) == 8) || \
          ((maxpages - 2) == 7) || ((maxpages - 2) == 6) || \
          ((maxpages - 2) == 5) || ((maxpages - 2) == 4) || \
          ((maxpages - 2) == 3) || ((maxpages - 2) == 2) || \
          ((maxpages - 2) == 1))
          {
            if(npage > 0)
             sprintf(str_num, "page-%ld.png", npage);
          }


          fprintf(fout, "<center><div style=\"margin-top: 20px;\">");
            fprintf(fout, "<img style=\"box-shadow: 0 10px 30px rgba(0, 0, 0, 0.3), 0 20px 40px rgba(0, 0, 0, 0.2);\" width=\"90%\" src=\"%s\"></img>\n", str_num);
          fprintf(fout, "</div></center>");

          ++npage;

        }

      }
      fprintf(fout, "</body>\n");
    }
    closedir(directorio);
    fclose(fout);
  }
}

void pdf_photos(const char *pathfile)
{

  char cmd[500] = {'\0'};

  if(pathfile[0] == '\0') return;

  sprintf(cmd, "pdftoppm -aa yes -progress -png '%s' '/tmp/out/page'", pathfile);
  system(cmd);

}

