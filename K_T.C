        rez=testS();
        if(rez==-1)
          {
           printf("Error S\n");
           AppLogT(LOGNAME,"Error S");
           return(-1);
          }
        rez=testR();
        if(rez==-1)
          {
           printf("Error R\n");
           AppLogT(LOGNAME,"Error R");
           return(-1);
          }
        rez=testL();
        if(rez==-1)
          {
           printf("Error L\n");
           AppLogT(LOGNAME,"Error L");
           return(-1);
          }
        rez=testExpandKey();
        if(rez==-1)
          {
           printf("Error Expand Key\n");
           AppLogT(LOGNAME,"Error Expand Key");
           return(-1);
          }
        rez=testEncrypt(1);
        if(rez==-1)
          {
           printf("Error Encrypt-1\n");
           AppLogT(LOGNAME,"Error Encrypt-1");
           return(-1);
          }
        rez=testEncrypt(2);
        if(rez==-1)
          {
           printf("Error Encrypt-2\n");
           AppLogT(LOGNAME,"Error Encrypt-2");
           return(-1);
          }
        rez=testEncrypt(3);
        if(rez==-1)
          {
           printf("Error Encrypt-3\n");
           AppLogT(LOGNAME,"Error Encrypt-3");
           return(-1);
          }
        rez=testEncrypt(4);
        if(rez==-1)
          {
           printf("Error Encrypt-4\n");
           AppLogT(LOGNAME,"Error Encrypt-4");
           return(-1);
          }
        rez=testDecrypt();
        if(rez==-1)
          {
           printf("Error Decrypt\n");
           AppLogT(LOGNAME,"Error Decrypt");
           return(-1);
          }
        rez=TestVector(rnd,1);
        if(rez==-1)
          {
           printf("Error TestVector-1\n");
           AppLogT(LOGNAME,"Error TestVector-1");
           return(-1);
          }
        rez=TestVector(rnd,2);
        if(rez==-1)
          {
           printf("Error TestVector-2\n");
           AppLogT(LOGNAME,"Error TestVector-2");
           return(-1);
          }
//    Проверка работы функции расширения ключа с маской (первоначальная установка нулевой маски)
       for(i=0;i<160;i++) mm[i]=0;
//    Расширение ключа без маски      
       ExpandKey(kMasterKeyData,kk);
//       for(i=0;i<16;i++) printf("%02x ", kk[i]);
//       printf("\n");
//    получить два вектора маски на главный ключ
       NextRandom16(rnd,rnd_1,rnd);
       for(i=0;i<16;i++) {mkm[i]=rnd[i];kMasterKeyData[i]=kMasterKeyData[i]^mkm[i];}
       NextRandom16(rnd,rnd_1,rnd);
       for(i=0;i<16;i++) {mkm[i+16]=rnd[i];kMasterKeyData[i+16]=kMasterKeyData[i+16]^mkm[i+16];}
//    получить десять случайных векторов на расширенную маску
       for(i=0;i<10;i++)      
          {
           NextRandom16(rnd,rnd_1,rnd);
           for(j=0;j<16;j++) mm[i*16+j]=mm[i*16+j]^rnd[j];
          }
//    расширить ключ с маской
       ExpandKeyM(kMasterKeyData,mkm,kk1,mm);
//       for(i=0;i<16;i++) printf("%02x ", kk1[i]^mm[i]);
//       printf("\n");
//    сверить с эталоном
        rez=0;
        for(i=0;i<160;i++) if(kk[i]!=(kk1[i]^mm[i])) rez++;
        if(rez!=0)
          {
           printf("Error Expand Key with mask\n");
           AppLogT(LOGNAME,"Error Expand Key with mask");
           return(-1);
          }
//       printf("Ok Expand Key with mask\n");

//    Тест защифрования с маской
       en_k_mask(ot_k,xx,kk1,mm);
       
//       for(i=0;i<16;i++) printf("%02x ",xx[i]);
//       printf("\n");
//    Сравнить с эталоном       
       if(memcmp(xx,et_k1,16)!=0)
         {
          printf("Error Encrypt with mask\n");
          AppLogT(LOGNAME,"Error Encrypt with mask");
          return(-1);
         } 
//       printf("Modify mask...\n");
       for(i=0;i<160;i++) mm[i]=0;
//    Inctemental modify mask       
       for(i=0;i<160;i++) {kk[i]=kk[i]^i;mm[i]=mm[i]^i;}
       en_k_mask(ot_k,xx,kk,mm);
       if(memcmp(xx,et_k1,16)!=0)
         {
          printf("Error Encrypt with mask\n");
          AppLogT(LOGNAME,"Error Encrypt with mask");
          return(-1);
         } 
       
//       for(i=0;i<16;i++) printf("%02x ",xx[i]);
//       printf("\n");
          
       dec_k_mask(xx,yy,kk,mm);
//       for(i=0;i<16;i++) printf("%02x ",yy[i]);
//       printf("\n");

       if(memcmp(yy,ot_k,16)!=0)
         {
          printf("Error Decrypt with mask\n");
          AppLogT(LOGNAME,"Error Decrypt with mask");
          return(-1);
         } 
    
//    отдельно модифицировать маску случайным вектором длиной 160 
       for(i=0;i<10;i++)      
          {
           NextRandom16(rnd,rnd_1,rnd);
           for(j=0;j<16;j++) {kk[i*16+j]=kk[i*16+j]^rnd[j];mm[i*16+j]=mm[i*16+j]^rnd[j];}
          }
//    проверить зашифрование на новой маске
       en_k_mask(ot_k,xx,kk,mm);
       if(memcmp(xx,et_k1,16)!=0)
         {
          printf("Error Encrypt with mask\n");
          AppLogT(LOGNAME,"Error Encrypt with mask");
          return(-1);
         } 
       
//    проверить гаммирование
       e_k_ctr(kIV,ot_k,yyy,kk,mm,64);            
       if(memcmp(yyy,et_kctr,64)!=0)
       if(memcmp(xx,et_k1,16)!=0)
         {
          printf("Error Encrypt CTR with mask\n");
          AppLogT(LOGNAME,"Error Encrypt CTR with mask");
          return(-1);
         } 
//    проверить имитовставку      
       i_k(ot_k,yyy,kk,mm,64);            
       if(memcmp(yyy,kGost15ImitS,8)!=0)
         {
          printf("Error Imit with mask\n");
          AppLogT(LOGNAME,"Error Imit with mask");
          return(-1);
         } 

        AppLogT(LOGNAME,"Success Protect Function");

        rnd[0]=td[0];
        rnd[4]=td[1];
        rnd[6]=td[2];
        rnd[1]=td[3];
        rnd[5]=td[4];
        rnd[11]=td[5]-2000;
        for(i=0;i<16;i++) rnd[i]=rnd[i]^k1[i];
//        if(argc==4) for(i=0;i<strlen(rs);i++) rnd[i]=rnd[i]^rs[i];
//     простой тест датчика на несовпадение последовательных итераций
        IRandom(rnd_1,td[0]*td[1]);
        IRandom(rnd,td[2]+td[3]);
        rez=0;
        for(i=0;i<16;i++) if(rnd[i]!=rnd_1[i]) rez++;
        if(rez==0)
          {
           printf("Error Random (0)");
           AppLogT(LOGNAME,"Error Random (0)");
           return(-4);
          }
        for(i=0;i<16;i++) rnd[i]=rnd[i]^rnd_1[i];

        rez=InitRandom(rnd);
        if(rez==-1)
         {
          Crandom(td[0]*td[1]+td[2]);
         }
        rez=InitRandom(rnd);
        if(rez!=0)
         {
          printf("Error Random (1)");
          AppLogT(LOGNAME,"Error Random (1)");
          return(-4);
         }
        rez=TestVector(rnd,0);
        if(rez!=0)
          {
           printf("Error Random (2)");
           AppLogT(LOGNAME,"Error Random (2)");
           return(-4);
          }
        NextRandom16(rnd,rnd_1,rnd);
        rez=TestVector(rnd,0);
        if(rez!=0)
          {
           printf("Error Random (3)");
           AppLogT(LOGNAME,"Error Random (3)");
           return(-4);
          }
        printf("Ok Test Random\n");
        AppLogT(LOGNAME,"Ok Test Random");
        rez=UpRandom(td[0]*td[1]+td[2]+td[3]);
        if(rez!=0)
         {
          printf("Error Update Random File\n");
          AppLogT(LOGNAME,"Error Update Random File");
          return(-5);
         } 
//        for(i=0;i<8;i++) printf("%02x",rnd[i]);printf("\n");   
