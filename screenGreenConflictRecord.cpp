//---------------------------------------------------------------------------
#include "screenGreenConflictRecord.h"
#include "SCREENOperStat.h"
#include "LCD240x128.h"
#include "SMEM.h"
#include "SCREENMain.h"
#include <stdio.h>
#include <string.h>

ScreenGreenConflictRecord screenGreenConflictRecord;
//---------------------------------------------------------------------------
ScreenGreenConflictRecord::ScreenGreenConflictRecord(void)
{
    loadBitmapFromFile();
    initDispWord();
}
//---------------------------------------------------------------------------
ScreenGreenConflictRecord::~ScreenGreenConflictRecord(void)
{
}
//---------------------------------------------------------------------------
void ScreenGreenConflictRecord::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//GreenConflict.bit","rb");
    if (bitmap) {
        fread(GreenConflictRecordBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenGreenConflictRecord::initDispWord(void)
{
  for (int i=0;i<6;i++)
  {
    for (int j=0;j<4;j++) //年
    {
      GreenConflictWord[i][j].X=4+j*8;
      GreenConflictWord[i][j].Y=26+i*16;
      GreenConflictWord[i][j].width=8;  
      GreenConflictWord[i][j].height=16;
    }

    for (int j=4;j<6;j++) //月
    {
      GreenConflictWord[i][j].X=20+j*8;
      GreenConflictWord[i][j].Y=26+i*16;
      GreenConflictWord[i][j].width=8;  
      GreenConflictWord[i][j].height=16;
    }

    for (int j=6;j<8;j++) //日
    {
      GreenConflictWord[i][j].X=36+j*8;
      GreenConflictWord[i][j].Y=26+i*16;
      GreenConflictWord[i][j].width=8;  
      GreenConflictWord[i][j].height=16;
    }

    for (int j=8;j<10;j++) //時
    {
      GreenConflictWord[i][j].X=52+j*8;
      GreenConflictWord[i][j].Y=26+i*16;
      GreenConflictWord[i][j].width=8;  
      GreenConflictWord[i][j].height=16;
    }

    for (int j=10;j<12;j++) //分
    {
      GreenConflictWord[i][j].X=68+j*8;
      GreenConflictWord[i][j].Y=26+i*16;
      GreenConflictWord[i][j].width=8;  
      GreenConflictWord[i][j].height=16;
    }

    for (int j=12;j<14;j++) //第一接點
    {
      GreenConflictWord[i][j].X=84+j*8;
      GreenConflictWord[i][j].Y=26+i*16;
      GreenConflictWord[i][j].width=8;  
      GreenConflictWord[i][j].height=16;
    }

    for (int j=14;j<16;j++) //第二接點
    {
      GreenConflictWord[i][j].X=92+j*8;
      GreenConflictWord[i][j].Y=26+i*16;
      GreenConflictWord[i][j].width=8;  
      GreenConflictWord[i][j].height=16;
    }

    for (int j=16;j<18;j++) //第三接點
    {
      GreenConflictWord[i][j].X=100+j*8;
      GreenConflictWord[i][j].Y=26+i*16;
      GreenConflictWord[i][j].width=8;  
      GreenConflictWord[i][j].height=16;
    }
  }
}
//---------------------------------------------------------------------------
void ScreenGreenConflictRecord::DoKeyWork(BYTE key)                                           
{
try {
    switch (key) {
      case 0x90:
        DoKeyF1Work();
      break;
      case 0x93:
        DoKeyF4Work();
      break;
      case 0x98:
        DoKeyEnterWork();
      default:
      break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenGreenConflictRecord::DisplayGreenConflictRecord(void)
{
  try 
  {
    if (smem.GetcFace()!=cGREENCONFLICTRECORD) 
    {  
      smem.SetcFace(cGREENCONFLICTRECORD);
      lcd240x128.DISPLAY_GRAPHIC(0,GreenConflictRecordBitmap,128,30);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenGreenConflictRecord::vShowGreenConflictRecord(void)
{
  try
  {
    FILE *fd;
    char buff[6][20];
    fd=fopen("//cct//Data//SETTING//GreenConflict.txt","a+");
    
    if (fd) 
    {
    fread(buff,120,1,fd); 
    fclose(fd);
    printf("%s \n",buff);
    }
    for(int i =0; i<6; i++)
    {
      for(int j = 0; j < 18; j++)
      {
        if(buff[i][j] == '0') buff[i][j] = 0x00;
        else if (buff[i][j] == '1') buff[i][j] = 0x01;
        else if (buff[i][j] == '2') buff[i][j] = 0x02;
        else if (buff[i][j] == '3') buff[i][j] = 0x03;
        else if (buff[i][j] == '4') buff[i][j] = 0x04;
        else if (buff[i][j] == '5') buff[i][j] = 0x05;
        else if (buff[i][j] == '6') buff[i][j] = 0x06;
        else if (buff[i][j] == '7') buff[i][j] = 0x07;
        else if (buff[i][j] == '8') buff[i][j] = 0x08;
        else if (buff[i][j] == '9') buff[i][j] = 0x09;
        else buff[i][j] = 0x10;
      }
    }
    BYTE space[16]={0};
    for(int i=0; i<6; i++)   //第幾筆資料
    {
      for(int j = 0; j < 18; j++)   //每筆資料的數據
      {
        if(buff[i][j] == 0x10)
        {
          lcd240x128.DISPLAY_GRAPHIC_XY(GreenConflictWord[i][j].X,GreenConflictWord[i][j].Y,space,GreenConflictWord[i][j].height,GreenConflictWord[i][j].width/8);
        }
        else
        {
          lcd240x128.DISPLAY_GRAPHIC_XY(GreenConflictWord[i][j].X,GreenConflictWord[i][j].Y,word8x16[buff[i][j]],GreenConflictWord[i][j].height,GreenConflictWord[i][j].width/8);
        }  
      }
    }  
  } catch(...) {}
}
//---------------------------------------------------------------------------
void ScreenGreenConflictRecord::DoKeyF1Work(void)
{
  screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenGreenConflictRecord::DoKeyF4Work(void)
{
  screenOperStat.DisplayOperStat();
}
//---------------------------------------------------------------------------
void ScreenGreenConflictRecord::DoKeyEnterWork(void)
{
}
//---------------------------------------------------------------------------
