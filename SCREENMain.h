#ifndef SCREENMainH
#define SCREENMainH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENMain:public SCREEN
{
    public:

      SCREENMain(void);
      ~SCREENMain(void);

      void DisplayMain(void);                                                   //���MAIN��
      void DoKeyWork(BYTE);                                                     //�ھ�KEY���ȧ@��
      void DisplayDateTime(void);

    private:

      DISP_WORD dateWord[8];                                                    //��ܤ������m
      DISP_WORD timeWord[6];                                                    //��ܮɶ�����m
      DISP_WORD mark[10];                                                        //��ܴ�Ъ���m
      int cSelect;                                                              //�{�b��ܪ��ﶵ

      BYTE mainBitmap[3840];                                                    //����
      void LoadBitmapFromFile(void);                                            //�N����Load�i�O����
      void InitDispWord(void);                                                  //��l�ƪťզ�m

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
      void DoKeyAWork(void);

      void DoKeyF1Work(void);
      void DoKeyF2Work(void);
      void DoKeyF3Work(void);

      void DoKeyUPWork(void);
      void DoKeyDOWNWork(void);
      void DoKeyLEFTWork(void);
      void DoKeyRIGHTWork(void);
      void DoKeyEnterWork(void);
};
//---------------------------------------------------------------------------
extern SCREENMain screenMain;
#endif
