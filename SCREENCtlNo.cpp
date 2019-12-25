#include "SCREENCtlNo.h"
#include "SCREENMain.h"
#include "SCREENCtlSetup.h"
#include "LCD240x128.h"
#include "SMEM.h"
#include "SCREENABOErr.h"
#include "SCREENLcnEncode.h"

SCREENCtlNo screenCtlNo;
//---------------------------------------------------------------------------
SCREENCtlNo::SCREENCtlNo(void)
{
    LoadBitmapFromFile();
    InitDispWord();
}
//---------------------------------------------------------------------------
SCREENCtlNo::~SCREENCtlNo(void)
{
}
//---------------------------------------------------------------------------
void SCREENCtlNo::DoKeyWork(BYTE key)                                           //已過濾,進來的是0x80~0x98之間
{
try {
    switch (key) {
        case 0x80:
        case 0x81:
        case 0x82:
        case 0x83:
        case 0x84:
        case 0x85:
        case 0x86:
        case 0x87:
        case 0x88:
        case 0x89:
        case 0x8A:
        case 0x8B:
        case 0x8C:
        case 0x8D:
        case 0x8E:
        case 0x8F:
        case 0x92://F3
        break;

        case 0x90://F1,ESC
          DoKeyF1Work();
        break;
        case 0x91://F2
          DoKeyF2Work();
        break;
        case 0x93://F4
          DoKeyF4Work();
        break;

        case 0x94://UP
        case 0x95://DOWN
        case 0x96://LEFT
        case 0x97://RIGHT
        case 0x98://Enter
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCtlNo::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//4_1_1.bit","rb");
    if (bitmap) {
        fread(ctlNoBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCtlNo::DisplayCtlNo(void)
{
try {
    if (smem.vGet0FCommandSet()>2) {
        smem.SetcFace(cCTLNO);
        smem.vWriteMsgToDOM("Enter Controller Number Screen");
        lcd240x128.DISPLAY_GRAPHIC(0,ctlNoBitmap,128,30);
        DisplayNum();
    } else {
        smem.SetLastFace(cCTLSETUP);
        screenABOErr.DisplayABOErr();
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCtlNo::InitDispWord(void)
{
    for (int i=0;i<5;i++) {
         lcn[i].X=64+i*24;
         lcn[i].Y=60;
         lcn[i].width=24;
         lcn[i].height=24;
    }
}
//---------------------------------------------------------------------------
void SCREENCtlNo::DisplayNum(void)
{
try {
    int address=smem.GetAddress();
    if (smem.GetcFace()==cCTLNO) {
        lcd240x128.DISPLAY_GRAPHIC_XY(lcn[0].X,lcn[0].Y,word24x24[address/10000],lcn[0].height,lcn[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(lcn[1].X,lcn[1].Y,word24x24[(address/1000)%10],lcn[1].height,lcn[1].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(lcn[2].X,lcn[2].Y,word24x24[(address/100)%10],lcn[2].height,lcn[2].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(lcn[3].X,lcn[3].Y,word24x24[(address/10)%10],lcn[3].height,lcn[3].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(lcn[4].X,lcn[4].Y,word24x24[address%10],lcn[4].height,lcn[4].width/8);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCtlNo::DoKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENCtlNo::DoKeyF2Work(void)
{
    screenLcnEncode.DisplayLcnEncode();
}
//---------------------------------------------------------------------------
void SCREENCtlNo::DoKeyF4Work(void)
{
    screenCtlSetup.DisplayCtlSetup();
}
//---------------------------------------------------------------------------
