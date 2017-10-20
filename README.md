# FPE-in-C
formatting encryption with AES algorithm in c,it's simple but useful
转载请注明出处：http://www.jianshu.com/p/04da32ba7747
### 要点
- 格式保持加密
- AES加密算法

### 格式保持加密
格式保持加密是指输入的明文和输出的密文是以保持相同的格式，就是说密文的长度不变，而格式也与明文相同。如果不保持格式，就意味着长度会变化且密文不具有明文所有的格式，数据库需要为此修改，很多方面都会受到影响，所以格式保持加密在生产生活中十分重要。

这里完成格式保持加密的思路是建立明密文置换表，先对自然数进行AES加密，加密后的权值可以进行排序，排序就使得我们加密的域内所有字符可以对应其他字符，如：
```
weight(0) = 0x56c644080098fc5570f2b329323dbf62
weight(1) = 0x08ee98c0d05e3dad3eb3d6236f23e7b7
weight(2) = 0x47d2e1bf72264fa01fb274465e56ba20
weight(3) = 0x077de40941c93774857961a8a772650d

Sorting [0,1,2,3] by weight gives [3,1,2,0], so the cipher is
F(0) = 3
F(1) = 1
F(2) = 2
F(3) = 0.
```

经过加密后：0->3,1->1,2->2,3->0
那么假设我们的字符集是{a,b,c,d},经过上述过程就可以得到
a->d,b->b,c->c,d->a
每个字符都与其他字符建立了对应关系，解密则是相反变换。
这就实现了格式保持加密。
### AES加密算法
高级加密标准。过程如下：
```
int key[128];
int P[128];
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
  // 列混淆
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
```

### 格式保持加密的过程：
```
int numre[10] = {0,1,2,3,4,5,6,7,8,9};
char charre[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
char bcharre[26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
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
```
