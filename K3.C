void step_inc_key(unsigned char *tmp_k,int len)
     {
      int p,i,per;
      per=1;
      for(i=0;i<len;i++)
         {
          p=tmp_k[len-(i+1)];
          if( (p< 255)&&(per==1) ) {tmp_k[len-(i+1)]++;per=0;}
          if( (p==255)&&(per==1) ) {tmp_k[len-(i+1)]=0;per=1;}
         } 
   
     }

unsigned char o2[16]={
0,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xee,0xff,0x0a};
unsigned char z2[16]={ 
0xb4,0x29,0x91,0x2c,0x6e,0,0x32,0xf9,0x28,0x54,0x52,0xd7,0x67,0x18,0xd0,0x8b};

unsigned char ot_k[64] = 
{
 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x00, 0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88,
 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xee, 0xff, 0x0a,
 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xee, 0xff, 0x0a, 0x00,
 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xee, 0xff, 0x0a, 0x00, 0x11
};

unsigned char et_k1[64] = 
{
 0x7f, 0x67, 0x9d, 0x90, 0xbe, 0xbc, 0x24, 0x30, 0x5a, 0x46, 0x8d, 0x42, 0xb9, 0xd4, 0xed, 0xcd,
 0xb4, 0x29, 0x91, 0x2c, 0x6e, 0x00, 0x32, 0xf9, 0x28, 0x54, 0x52, 0xd7, 0x67, 0x18, 0xd0, 0x8b,
 0xf0, 0xca, 0x33, 0x54, 0x9d, 0x24, 0x7c, 0xee, 0xf3, 0xf5, 0xa5, 0x31, 0x3b, 0xd4, 0xb1, 0x57,
 0xd0, 0xb0, 0x9c, 0xcd, 0xe8, 0x30, 0xb9, 0xeb, 0x3a, 0x02, 0xc4, 0xc5, 0xaa, 0x8a, 0xda, 0x98
};

unsigned char kIV[8] =
{
     0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xce, 0xf0
};
unsigned char et_kctr[64] = 
{
     0xf1, 0x95, 0xd8, 0xbe, 0xc1, 0x0e, 0xd1, 0xdb, 0xd5, 0x7b, 0x5f, 0xa2, 0x40, 0xbd, 0xa1, 0xb8,
     0x85, 0xee, 0xe7, 0x33, 0xf6, 0xa1, 0x3e, 0x5d, 0xf3, 0x3c, 0xe4, 0xb3, 0x3c, 0x45, 0xde, 0xe4,
     0xa5, 0xea, 0xe8, 0x8b, 0xe6, 0x35, 0x6e, 0xd3, 0xd5, 0xe8, 0x77, 0xf1, 0x35, 0x64, 0xa3, 0xa5,
     0xcb, 0x91, 0xfa, 0xb1, 0xf2, 0x0c, 0xba, 0xb6, 0xd1, 0xc6, 0xd1, 0x58, 0x20, 0xbd, 0xba, 0x73
};


int funcLSX4m(unsigned char* a, unsigned char* b, unsigned char *m, unsigned char* outdata)
{
     unsigned char temp1[16];
     unsigned char temp2[16];

     funcX(a, b, temp1);
     funcX(temp1, m, temp2);
     funcSL(temp2, outdata);

     return 0;
}


int  en_k_mask(unsigned char* plainText, unsigned char* chipherText, unsigned char* keys_m, unsigned char *m)
{
     unsigned char xTemp[16];
     unsigned char yTemp[16];
     int i;

     memcpy(xTemp, plainText, 16);
     
     for(i = 0; i < 9; ++i)
        {
         funcLSX4m(xTemp, keys_m + 16*i, m + 16*i, yTemp);
         memcpy(xTemp, yTemp, 16);
        }
     funcX(yTemp, keys_m+9*16, xTemp);
     funcX(xTemp, m     +9*16, chipherText);

     return 0;
}

int funcReverseLSXm(unsigned char* a, unsigned char* b, unsigned char *m, unsigned char* outdata)
{
     unsigned char temp1[16];
     unsigned char temp2[16];
     unsigned char temp3[16];


     funcX(a, b, temp1);
     funcX(temp1, m, temp2);
     funcReverseL(temp2, temp3);
     funcReverseS(temp3, outdata);

     return 0;
}


int dec_k_mask(unsigned char* chipherText, unsigned char* plainText, unsigned char* keys_m, unsigned char *m)
{
     unsigned char xTemp[16];
     unsigned char yTemp[16];
     int i;

     memcpy(xTemp, chipherText, 16);
     for(i = 0; i < 9; ++i)
        {
         funcReverseLSXm(xTemp, keys_m+(9-i)*16, m+(9-i)*16, yTemp);
         memcpy(xTemp, yTemp, 16);
        }
     funcX(yTemp, keys_m, xTemp);
     funcX(xTemp, m     , plainText);

     return 0;
}

int e_k_ctr(unsigned char *iv, unsigned char *plainText, unsigned char *chipherText, unsigned char* keys_m, unsigned char *m,int len)
    {
     int i,j,step;
     unsigned char ot[16],xx[16];

     if(len%16!=0) return(-1);
     for(j=0;j<16;j++) ot[j]=0;
     for(j=0;j<8 ;j++) ot[j]=iv[j];

     step=len/16;
     en_k_mask(ot,xx,keys_m,m);
     for(j=0;j<16;j++) chipherText[j]=plainText[j]^xx[j];
     if(step>1)
       {
        for(i=0;i<step-1;i++)
           {
            step_inc_key(ot,16);
//            for(j=0;j<16;j++) printf("%02x ",ot[j]);
//            printf("\n");              
            en_k_mask(ot,xx,keys_m,m);
//            for(j=0;j<16;j++) printf("%02x ",xx[j]);
//            printf("\n");              
            for(j=0;j<16;j++) chipherText[(i+1)*16+j]=plainText[(i+1)*16+j]^xx[j];
           }
       }
     return(0);  
    }
static void Shift1(unsigned char *x, unsigned char *y, int len)
{
  int i;
  unsigned char c = x[0], carry = c >> 7, cnext;
  /* Shift block to left, including carry */
  for (i = 0; i < len - 1; i++, c = cnext)
    y[i] = (c << 1) | ((cnext = x[i + 1]) >> 7);
    y[len-1] = (c << 1) | ((cnext = x[i + 1]) >> 7);
  /* If MSB set fixup with R */
//  y[i] = (c << 1) ^ ((0 - carry) & (16 == len ? 0x87 : 0x1b));
//  y[i] = (c << 1) ^ ((0 - carry));
}    

void Shift(unsigned char *x,unsigned char *y,int len)
     {
      int i,j,pp[128],pp1[128],cnt;
      unsigned char xx[16];
      
      for(i=0;i<16;i++) xx[i]=0;
      
      for(i=0;i<len;i++)
         {
          for(j=0;j<4;j++) pp[i*8+j  ]=bintab[ (x[i]&0xF0)>>4][j];
          for(j=0;j<4;j++) pp[i*8+j+4]=bintab[ (x[i]& 0xF)   ][j];
         }
//      for(i=0;i<16;i++) printf("%01d ",pp[i]);
//      printf("\n");   
      for(i=0;i<len*8  ;i++) pp1[i]=0;
      for(i=0;i<len*8-1;i++) pp1[i]=pp[i+1];
      cnt=0;
      for(i=0;i<len*8;i=i+8)
         {
          for(j=0;j<16;j++)
             if( (pp1[i]==bintab[j][0])&&(pp1[i+1]==bintab[j][1])&&(pp1[i+2]==bintab[j][2])&&(pp1[i+3]==bintab[j][3]) )
               xx[cnt]=j<<4;
          for(j=0;j<16;j++)
             if( (pp1[i+4]==bintab[j][0])&&(pp1[i+5]==bintab[j][1])&&(pp1[i+6]==bintab[j][2])&&(pp1[i+7]==bintab[j][3]) )
               xx[cnt]=xx[cnt]^j;
          cnt++;
         } 
      for(i=0;i<len;i++) y[i]=xx[i];              
     }

int i_k(unsigned char *plainText, unsigned char *imi, unsigned char* keys_m, unsigned char* m,int len)
    {
     int i,j,step;
     unsigned char ot[16],xx[16],yy[16],yy1[16],zz[16],im[16];

     if(len%16!=0) return(-1);
     
     for(j=0;j<16;j++) ot[j]=0;
     en_k_mask(ot,xx,keys_m,m);

//     for(j=0;j<16;j++) printf("%02x ",xx[j]);
//     printf("\n");              

     Shift(xx,yy,16);
//     for(j=0;j<16;j++) printf("%02x ",yy[j]);
//     printf("\n");      
     
     yy[15]=yy[15]^0x87;

     step=len/16;
     for(j=0;j<16;j++) zz[j]=0;
     for(i=0;i<step-1;i++)
        {
         for(j=0;j<16;j++) ot[j]=plainText[i*16+j]^zz[j]; 
//         for(j=0;j<16;j++) printf("%02x ",ot[j]);
//         printf("\n");      
         
         en_k_mask(ot,zz,keys_m,m);
//         for(j=0;j<16;j++) printf("%02x ",zz[j]);
//         printf("\n");      
        }
     for(j=0;j<16;j++) ot[j]=plainText[(step-1)*16+j]^zz[j]^yy[j]; 
//     for(j=0;j<16;j++) printf("%02x ",ot[j]);
//     printf("\n");      
     en_k_mask(ot,zz,keys_m,m);
//     for(j=0;j<16;j++) printf("%02x ",zz[j]);
//     printf("\n");      
     
     for(j=0;j<8;j++) imi[j]=zz[j];            
     return(0);
    }

int i_k2(unsigned char *plainText, unsigned char *imi, unsigned char* keys_m, unsigned char* m,int len)
    {
     int i,j,step,ost;
     unsigned char ot[16],xx[16],yy[16],yy1[16],zz[16],im[16],last[16];

     if(len%16==0) return(-1);
     
     
     for(j=0;j<16;j++) ot[j]=0;
     en_k_mask(ot,xx,keys_m,m);

//     for(j=0;j<16;j++) printf("%02x ",xx[j]);
//     printf("\n");              

     Shift(xx,yy,16);
     yy[15]=yy[15]^0x87;
     Shift(yy,yy1,16);
     

//     for(j=0;j<16;j++) printf("%02x ",yy1[j]);
//     printf("\n");      
     
     step=len/16;
     ost =len%16;
     for(j=0;j<16;j++) zz[j]=0;
     for(i=0;i<step-1;i++)
        {
         for(j=0;j<16;j++) ot[j]=plainText[i*16+j]^zz[j]; 
//         for(j=0;j<16;j++) printf("%02x ",ot[j]);
//         printf("\n");      
         
         en_k_mask(ot,zz,keys_m,m);
//         for(j=0;j<16;j++) printf("%02x ",zz[j]);
//         printf("\n");      
        }
//   Last block
     if(ost==15) last[15]=0x80;
     else
      {
       last[ost]=0x80;
       for(j=ost+1;j<16;j++) last[j]=0;
      }  
     for(j=0;j<ost;j++) last[j]=plainText[step*16+j];
     for(j=0;j<16 ;j++) ot[j]=last[j]^zz[j]^yy1[j]; 
//     for(j=0;j<16;j++) printf("%02x ",ot[j]);
//     printf("\n");      
     en_k_mask(ot,zz,keys_m,m);
//     for(j=0;j<16;j++) printf("%02x ",zz[j]);
//     printf("\n");      
     
     for(j=0;j<8;j++) imi[j]=zz[j];            
     return(0);
    }
