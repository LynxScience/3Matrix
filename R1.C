unsigned char rnd[32],rnd_1[32];
unsigned long k[64] = {
	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2};
unsigned char k1[32]={
        0x42,0x8a,0x2f,0x98,0x71,0x37,0x44,0x91,0xb5,0xc0,0xfb,0xcf,0xe9,0x5d,0xba,0x39,
        0x56,0xc2,0x5b,0x59,0xf1,0x11,0xf1,0x92,0x3f,0x82,0xa4,0xab,0x1c,0x5e,0xd5,0xd8};
unsigned char k2[32]={
     	0x19,0xa4,0xc1,0x16,0x1e,0x37,0x6c,0x08,0x27,0x48,0x77,0x4c,0x34,0xb0,0xb,0xcb,
        0x39,0x1c,0x0c,0xb3,0x4e,0xd8,0xaa,0x4a,0x5b,0x9c,0xca,0x4f,0x68,0x2e,0x6f,0xf3};

unsigned char bintab[16][4]=
{
 0,0,0,0,
 0,0,0,1,
 0,0,1,0,
 0,0,1,1,
 0,1,0,0,
 0,1,0,1,
 0,1,1,0,
 0,1,1,1,
 1,0,0,0,
 1,0,0,1,
 1,0,1,0,
 1,0,1,1,
 1,1,0,0,
 1,1,0,1,
 1,1,1,0,
 1,1,1,1
};
int TestVector(unsigned char *rnd,int mode)
    {
     int num_3[8],i,j;
     int num_0,num_1,num_01,num_3all;
     unsigned char rnd2b[256];
     int len01,len_m;
     if(mode==1)
       {
        for(i=6;i<32;i++) rnd[i]=0x55^i;
        rnd[0]=0x4;rnd[1]=0;rnd[2]=0;rnd[3]=0;rnd[4]=0xFF;rnd[5]=0xFF;
       } 
     if(mode==2)
       {
        for(i=0;i<16;i++) rnd[2*i  ]=0x98;
        for(i=0;i<16;i++) rnd[2*i+1]=0x37;
       } 
     for(i=0;i<32;i++)
        {
         for(j=0;j<4;j++) rnd2b[i*8+j  ]=bintab[ (rnd[i]&0xF0)>>4][j];
         for(j=0;j<4;j++) rnd2b[i*8+j+4]=bintab[ (rnd[i]& 0xF)   ][j];
        }
     for(i=0;i<8;i++) num_3[i]=0;
     num_0=0;num_1=0;
     for(i=0;i<256;i++)
        {
         if(rnd2b[i]==0) num_0++;
         if(rnd2b[i]==1) num_1++;
        }
//     if(num_1+num_0==256) printf(" Ok sum 0 and 1\n");
     num_01=0;len01=0;len_m=0;
     for(i=0;i<255;i++) 
        if(rnd2b[i]^rnd2b[i+1]==1) 
          {
           num_01++;
           if(i-len01>len_m){len_m=i-len01;len01=i;}
           else len01=i;
          }
     for(i=0;i<254;i++) 
        num_3[rnd2b[i]*4+rnd2b[i+1]*2+rnd2b[i+2]]++;
//-- PRINT ---
//     printf(" Num 0           = %02d\n",num_0);   
//     printf(" Num 1           = %02d"  ,num_1);   
//     if((num_1>156)||(num_1<100)) printf(" - Bad...\n");
//     else printf(" - Goog!!!\n");
//     printf(" Num 1/0 and 0/1 = %02d",num_01);   
//     if((num_01>155)||(num_01<100)) printf(" - Bad...\n");
//     else printf(" - Goog!!!\n");
//     printf(" Len 0..0 or 1..1 = %02d",len_m);   
//     if(len_m>24) printf(" - Bad...\n");
//     else printf(" - Goog!!!\n");
//     printf(" Group(s) 3-gramms:\n");
//     for(i=0;i<8;i++) printf(" %02d ",num_3[i]);
//     printf("\n");
     num_3all=0;
     for(i=0;i<8;i++) num_3all=num_3all+num_3[i];
//     if(num_3all==254) printf(" Ok sum 3bit word\n");
     for(i=0;i<8;i++) if(num_3[i]==0) return(-4);
     if((num_1>156) ||(num_1<100 )) return(-1);
     if((num_01>155)||(num_01<100)) return(-2);
     if(len_m>24) return(-3);
     
     return(0);
    
    }
void NextRandom16(unsigned char *prnd,unsigned char *rnd,unsigned char *krnd)
     {
      int i;
      unsigned char keys[160];

      ExpandKey(krnd, keys);

      Encrypt_15_1(prnd, rnd, keys);
      for(i=0;i<16;i++) prnd[i]=prnd[i]^rnd[i];
      return;
     }
int InitRandom(unsigned char *rand)
    {
     FILE *rnd;
     int l,i;
     unsigned char p_rand[32];
     rnd=fopen("random.bin","rb");
     if(rnd!=NULL)
       {
        l=fread(p_rand,1,32,rnd);
        fclose(rnd);
        if(l!=32) return(-1);
        for(i=0;i<32;i++) rand[i]=rand[i]^p_rand[i];
        return(0);
       } 
     return(-1);
    }
int IRandom(unsigned char *rnd,unsigned long r)
    {
     int i;
     unsigned char *kr;
     unsigned char r1[16],r2[16];
     kr=(unsigned char *)k;
     for(i=0;i<16;i++) r1[i]=r1[i]^kr[i];
     for(i=0;i<16;i++) r2[i]=r2[i]^kr[i+16];
     r1[5]=r1[6]^(unsigned char)(r&0xFF); r2[5]=r2[7]^(unsigned char)((r&0xFF00)>>8); r2[10]=r2[11]^(unsigned char)((r&0xFF0000)>>16);
     NextRandom16(rnd,r2,r1);
    }
int Crandom(unsigned long r)
    {
     FILE *rnd;
     unsigned char r3[16],r2[16];
     int i,l1,l2,cc;
     IRandom(r3,r);     
     IRandom(r2,r);
     rnd=fopen("random.bin","wb");
     if(rnd!=NULL)
       {
        l1=fwrite(r3,1,16,rnd);
        l2=fwrite(r2,1,16,rnd);
        fclose(rnd);
       } 
     else return(-1);
     if((l1!=16)&&(l2!=16)) return(-2); 
     return(0);
    } 
int UpRandom(unsigned long r)
    {
     FILE *rnd;
     unsigned char r3[16],r2[16],r4[32];
     int i,l1,l2,cc;
     InitRandom(r4);
     for(i=0;i<16;i++) r2[i]=r2[i]^r4[i     ];
     for(i=0;i<16;i++) r3[i]=r3[i]^r4[i+16];
     IRandom(r3,r);     
     IRandom(r2,r);
     rnd=fopen("random.bin","wb");
     if(rnd!=NULL)
       {
        l1=fwrite(r3,1,16,rnd);
        l2=fwrite(r2,1,16,rnd);
        fclose(rnd);
       } 
     else return(-1);
     if((l1!=16)&&(l2!=16)) return(-2); 
     return(0);
    } 