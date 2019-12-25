#ifndef SCREENModifyDTH
#define SCREENModifyDTH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENModifyDT:public SCREEN
{
    public:
      SCREENModifyDT(void);
      ~SCREENModifyDT(void);

      void DisplayModifyDT(void);
      void DoKeyWork(BYTE);                                                     //根據KEY的值作事
      void DisplayCurrentDateTime(void);                                        //顯示現在時間

    private:
      DISP_WORD cDateWord[8];
      DISP_WORD cTimeWord[6];

      DISP_WORD dateWord[8];                                                    //顯示日期的位置
      BYTE dateTemp[8];
      DISP_WORD timeWord[6];                                                    //顯示時間的位置
      BYTE timeTemp[6];
      int cPosition;                                                            //左右移動的位置
      int cSelect;                                                              //上下切換   0:日期 1:時間

      void InitDispWord(void);                                                  //初始化此畫面的座標等參數
      void DisplayDate(void);                                                   //顯示日期
      void DisplayTime(void);                                                   //顯示時間


      BYTE modifyDTBitmap[3840];
      void LoadBitmapFromFile(void);                                            //將底圖Load進記憶體

      void DoKey0Work(void);
      void DoKey1Work(void);
      void DoKey2Work(void);
      void DoKey3Work(void);
      void DoKey4Work(void);
      void DoKey5Work(void);
      void DoKey6Work(void);
      void DoKey7Work(void);
      void DoKey8Work(void);
      void DoKey9Work(void);
      void DoKeyF1Work(void);
      void DoKeyF4Work(void);
      void DoKeyLEFTWork(void);
      void DoKeyRIGHTWork(void);
      void DoKeyEnterWork(void);
};
//---------------------------------------------------------------------------
extern SCREENModifyDT screenModifyDT;
#endif
