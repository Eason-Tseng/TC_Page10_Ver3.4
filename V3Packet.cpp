#include <iostream>
using namespace std;
//**這邊的用途只是簡易編輯V3的協定，方便快速的複製最後print出來的結果，之後用packet sender之類的工具做通訊測試**//

int main(int argc, char** argv) {
    
    
     cout << "Hello world!" << endl;

unsigned char pack[40];

int packLength=15;                //整串訊息長度
int address=129;                //簡單說就是LCN
pack[0]=0xaa;                        //頭
pack[1]=0xbb;                        //頭
pack[2]=0x11;                        //流水號
pack[3]=address/256;
pack[4]=address%256;
pack[5]=packLength/256;
pack[6]=packLength%256;

//**************從這開始放V3的內容，因為長度會變動，所以請自行根據需要去做填寫，長度就在最上面的packLength那邊修改就好******************//
pack[7]=0x5f;                        
pack[8]=0x1c;
pack[9]=0x02;
pack[10]=0x01;
pack[11]=0x1e;
pack[12]=0x00;
pack[13]=0xff;
pack[14]=0x03;
pack[15]=0x03;
pack[16]=0x05;
pack[17]=0x00;
pack[18]=0x1e;
pack[19]=0x00;
pack[20]=0xff;
pack[21]=0x03;
pack[22]=0x03;
pack[23]=0x05;
pack[24]=0x00;
//********************V3內容的尾巴************************************************////
pack[packLength-3]=0xaa;
pack[packLength-2]=0xcc;
pack[packLength-1]=0;
for(int i=0;i<packLength-1;i++)
pack[packLength-1]^=pack[i];

printf("addres=%x %x length=%x %x CKS=%x\n",pack[3],pack[4],pack[5],pack[6],pack[packLength-1]);
for(int i=0;i<packLength;i++)
printf("%02x ",pack[i]);
printf("\n");
    
    return 0;
}