#include<stdio.h>
#include<memory.h>
int result[26][128];
int weight[26];
int key[128];
int P[128];
int C[128];
int numre[10] = {0,1,2,3,4,5,6,7,8,9};
char charre[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
char bcharre[26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
const int FSb[256] =
{
0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5,
0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0,
0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC,
0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A,
0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0,
0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B,
0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85,
0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5,
0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17,
0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88,
0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C,
0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9,
0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6,
0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E,
0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94,
0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68,
0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};
void addRoundKey(int *P,int *key){
  // 轮密钥加
  for(int i = 0 ; i < 128 ; i++)P[i] = P[i] ^ key[i];
}
void replace(int *P){
  // 字节替代
  for(int i = 0 , j = 0 , k = 0 ,n = 0 , m = 0; i < 121 ; ){
    j = (P[i + 3] + 2 * (P[i + 2] + 2 * (P[i + 1] + 2 * P[i])));
    k = (P[i + 7] + 2 * (P[i + 6] + 2 * (P[i + 5] + 2 * P[i + 4])));
    n = FSb[j * 16 + k];
    // printf("%d\n",n);
    for(m = i + 7 ; m >= i ; m--){
      P[m] = n % 2;
      n /= 2;
    }
    i += 8;
  }
}
void shiftRows(int *P){
  // 行移位
  for(int i = 0 , j = 0 , k = 0 ,temp = 0 ; i < 121 ;){
    for(k = 0; k < j ; k++){
      temp = P[i + 7];
      P[i + 7] = P[i + 6];P[i + 6] = P[i + 5];
      P[i + 5] = P[i + 4];P[i + 4] = P[i + 3];
      P[i + 3] = P[i + 2];P[i + 2] = P[i + 1];
      P[i + 1] = P[i + 0];P[i + 0] = temp;
    }
    i += 8;
  }
}
void mixColumns(int *P){
  int a=2,b=3,c=1,d=1,temp=0;
  for(int i = 0 , j = 0 , k = 0 ,n = 0 ; i < 128 ; ){
    for(j = 31 + i ; j >= i ; ){
      n = (P[j] + 2 * (P[j - 1] + 2 * (P[j - 2] + 2 * (P[j - 3] + 2 * (P[j - 4] + 2 * (P[j - 5] + 2 * (P[j - 6] + 2 * P[j - 7])))))));
      n *= d;
      temp = d;d = c;c = b;b = a;a = temp;
      temp = d;d = c;c = b;b = a;a = temp;
      for(k = j ; k >= j - 7 ; k--){
        P[k] = n % 2;
        n /= 2;
      }
      j -= 8;
    }
    i += 32;
  }
}
void revertAndSort(){
  for(int i = 0 , j = 0 , k = 0
     , n = 0 ,max = 0; i < 26 ; i++){
    max = weight[i];
    for(j = i+1 ; j < 26 ; j++){
      if(weight[j] > max){
        max = weight[j];
        int temp = weight[j];weight[j] = weight[i];weight[i] = temp;
        char tempch = charre[j];charre[j] = charre[i];charre[i] = tempch;
        tempch = bcharre[j];bcharre[j] = bcharre[i];bcharre[i] = tempch;
        if(j < 10){
          temp = numre[j];numre[j]= numre[i];numre[i] = temp;
        }
      }
    }
  }
}
char encryption(char ch){
  if(ch >= 'a' && ch <= 'z'){
    ch = charre[ch - 97];
  }else if(ch >= 'A' && ch <= 'Z'){
    ch = bcharre[ch - 65];
  }else if(ch >= '0' && ch <= '9'){
    ch = numre[ch - 48] + 48;
  }
  return ch;
}
char decryption(char ch){
  int i = 0;
  if(ch >= 'a' && ch <= 'z'){
    for(i = 0 ; i < 26 ; i++){
      if(charre[i] == ch)break;
    }
    ch = i+97;
  }else if(ch >= 'A' && ch <= 'Z'){
    for(i = 0 ; i < 26 ; i++){
      if(bcharre[i] == ch)break;
    }
    ch = i+65;
  }else if(ch >= '0' && ch <= '9'){
    for(i = 0 ; i < 11 ; i++){
      if(numre[i] == ch - 48)break;
    }
    ch = i+48;
  }
  return ch;
}
int ch2bin(char ch,int *key,int start){
  for(int i = 7,num = ch ; i >= 0 ; i--){
    key[start + i] = num % 2;
    num /= 2;
  }
  return 8;
}
void AES(int num){
  memset(P,0,sizeof(P));
  int numt = num;
  for(int i = 127 ; i >= 0 && numt != 0; i--){
    P[i] = numt % 2;
    numt /= 2;
  }
  for(int i = 0 ; i < 10 ; i++){
    addRoundKey(P,key);
    replace(P);
    shiftRows(P);
    mixColumns(P);
  }
  weight[num] = 0;
  for(int i = 127 , k = 1; i >= 7 ;){
    k = (P[i] + 2 * (P[i - 1] + 2 * (P[i - 2] + 2 * (P[i - 3] + 2 * (P[i - 4] + 2 * (P[i - 5] + 2 * (P[i - 6] + 2 * P[i - 7])))))));
    weight[num] += k;
    i -= 8;
  }
  revertAndSort();
}
void init(){
  // 初始化
  // memset(key,0,sizeof(key));
  memset(C,0,sizeof(C));
  for(int i = 0 ; i < 26 ; i++){
    AES(i);
    // printf("weight is %d\n",weight[i]);
  }
  // for(int i = 0 ; i < 52 ; i++){
  //   for(int j = i + 1 ; j < 52 ; j++){
  //     if(weight[j] == weight[i]){
  //       printf("same :%d with %d and %d",weight[i],i,j);
  //       return;
  //     }
  //   }
  // }

}
int main(){
  printf("请输入128位密钥：\n");
  char ch;
  int start = 0;
  // 输入密钥
  while(1){
    scanf("%c",&ch);
    start += ch2bin(ch,key,start);
    if(start >= 128)break;
  }
  init();
  printf("请输入要加密的明文：\n");
  getchar();
  while(1){
    scanf("%c",&ch);
    if(ch == '\n'){
      printf("\n");
      break;
    }
    printf("%c",encryption(ch));
  }
  printf("请输入要解密的密文：\n");
  while(1){
    scanf("%c",&ch);
    if(ch == '\n'){
      printf("\n");
      break;
    }
    printf("%c",decryption(ch));
  }
  return 0;
}
