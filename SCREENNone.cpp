//---------------------------------------------------------------------------
#include "SCREENNone.h"
#include "LCD240x128.h"
#include "SMEM.h"
#include "SCREENLogo.h"

SCREENNone screenNone;
//---------------------------------------------------------------------------
SCREENNone::SCREENNone(void)
{
    for (int i=0;i<3840;i++)  noneBitmap[i]=0;
}
//---------------------------------------------------------------------------
SCREENNone::~SCREENNone(void)
{
}
//---------------------------------------------------------------------------
void SCREENNone::DoKeyWork(BYTE key)                                            //�w�L�o,�i�Ӫ��O0x80~0x98����,5X5����
{
try {
    switch (key) {
        default:
          DoKeyDefaultWork();
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNone::DisplayNone(void)
{
try {
    if (smem.GetcFace()!=cNONE) {   //�W���O���F�קK�@�̨�,�]��timer���S���H�ӫ��ɷ|�@����refresh�@��
        smem.SetcFace(cNONE);
        lcd240x128.DISPLAY_GRAPHIC(0,noneBitmap,128,30);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNone::DoKeyDefaultWork(void)
{
    screenLogo.DisplayLogo();
}
//---------------------------------------------------------------------------
