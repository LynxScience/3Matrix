int if_exist(char *filename)
    {
     FILE *in;
     in=fopen(filename,"rb");
     if(in==NULL) return(-1);
     fclose(in);
     return(0);
    } 
void GetTimeDate(int *td)
     {
      time_t timer;
      struct tm *tb;
      timer=time(NULL);
      tb=localtime(&timer);
      td[0]=tb->tm_hour;
      td[1]=tb->tm_min;
      td[2]=tb->tm_sec;
      td[3]=tb->tm_mday;
      td[4]=tb->tm_mon+1;
      td[5]=tb->tm_year+1900;
     }
void AppLogT(char *LogName,char *Text)
     {
      FILE *out;
      int td[6];
      char ss[3]={0xD,0xA,0};
      out=fopen(LogName,"r+b");
      if(out==NULL) out=fopen(LogName,"wb");
      if(out==NULL) return;
      fseek(out,0,SEEK_END);
      GetTimeDate(td);
      fprintf(out,"%02d:%02d:%02d %02d.%02d.%04d:",td[0],td[1],td[2],td[3],td[4],td[5]);
//      fprintf(out,"%02d:%02d:%02d %02d.%02d:",td[0],td[1],td[2],td[3],td[4]);
      fwrite(Text,sizeof(char),strlen(Text),out);
      fwrite(ss  ,sizeof(char),2           ,out);
      fclose(out);
      return;
     } 
void AppS(char *LogName,char *Text,int len)
     {
      FILE *out;
      out=fopen(LogName,"wb");
      if(out==NULL) return;
      fwrite(Text,sizeof(char),len,out);
      fclose(out);
      return;
     } 
void ReadS(char *LogName,char *Text)
     {
      FILE *out;
      int len;
      out=fopen(LogName,"rb");
      if(out==NULL) return;
      len=fread(Text,sizeof(char),128,out);
      fclose(out);
      Text[len]=0;
      return;
     } 
// Сохранение в журнал элементов матрицы
void AppLogM(char *LogName,double** mas, int n)
     {
      FILE *out;
      int td[6];
      char ss[3]={0xD,0xA,0};
      out=fopen(LogName,"r+b");
      if(out==NULL) out=fopen(LogName,"wb");
      if(out==NULL) return;
      fseek(out,0,SEEK_END);
      for (int i = 0; i < n; i++)
          {
           for (int j = 0; j < n; j++){

           fprintf(out,"%8.4lf ", mas[i][j]);}
           fwrite(ss  ,sizeof(char),2           ,out);
          }
      fclose(out);
     }
