#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

int matchhere(char *re, char *text)
{
  if(re[0] == '\0')
    return 1;
  if(re[0] == '$' && re[1] == '\0')
    return *text == '\0';
  if(*text!='\0' && (re[0]=='.' || re[0]==*text))
    return matchhere(re+1, text+1);
  return 0;
}

int match(char *re, char *path){
  char *p;
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
  ;
  p++;
  if (matchhere(re, p)){
    printf("%s\n", path);
    return 1;
  }
  return 0;
}

void
find(char *path, char *re)
{
  char buf[512], *p;
  int fd;
  struct stat st;
  struct dirent de;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    match(re, path);
    break;
  case T_DIR:
    if(strlen(path)+1+DIRSIZ+1 > sizeof buf){
      printf("find: path too long\n");
      break;
    }
    if(match(re, path)){
      break;
    }

    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0){
        continue;
      }
      if((strcmp(de.name, ".")==0) || (strcmp(de.name, "..")==0)){
        continue;
      }
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      find(buf, re);
    }
    break;
  }
  close(fd);
}


int
main(int argc, char *argv[])
{
  if(argc != 3){
    printf("find [path] [expression]\n");
    exit();
  }

  find(argv[1], argv[2]);
  exit();
}
