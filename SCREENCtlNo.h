#ifndef SCREENCtlNoH
#define SCREENCtlNoH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENCtlNo:public SCREEN
{
    public:

      SCREENCtlNo(void);
      ~SCREENCtlNo(void);

      void DisplayCtlNo(void);                                                  //顯示設備編號頁
      void DoKeyWork(BYTE);                                                     //根據KEY的值作事

      void DisplayNum(void);

    private:

      DISP_WORD lcn[5];                                                         //此頁的空白處,定義座標

      BYTE ctlNoBitmap[3840];                                                   //底圖

      void LoadBitmapFromFile(void);                                            //將底圖Load進記憶體
      void InitDispWord(void);                                                  //初始化此畫面的座標等參數


      void DoKeyF1Work(void);
      void DoKeyF2Work(void);
      void DoKeyF4Work(void);
};
//---------------------------------------------------------------------------
extern SCREENCtlNo screenCtlNo;
#endif

