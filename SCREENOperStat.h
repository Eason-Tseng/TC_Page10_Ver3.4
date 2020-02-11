#ifndef SCREENOperStatH
#define SCREENOperStatH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENOperStat:public SCREEN
{
    public:
        SCREENOperStat(void);
        ~SCREENOperStat(void);

        void DisplayOperStat(void);
        void DoKeyWork(BYTE);                                                   //�ھ�KEY���ȧ@��

        void vShowGPSTime(time_t);
        void vShowMachineLocation(void);

        void vShowGreenConflict(void);

    private:
        BYTE operStatBitmap[3840];                                              //����

        void LoadBitmapFromFile(void);                                          //�N����Load�i�O����

        void DoKeyAWork();
        void DoKeyF1Work();
        void DoKeyF2Work();
        void DoKeyF3Work();
        void DoKeyF4Work();
        void DoKeyEnterWork();

        DISP_WORD dateWord[8];                                                    //��ܤ������m
        DISP_WORD timeWord[6];                                                    //��ܮɶ�����m
        DISP_WORD GreenConflictWord[6];
        DISP_WORD HWStatusWord[4];
        DISP_WORD MachineLocationWord;
        void InitDispWord(void);                                                  //��l�ƪťզ�m
};
//---------------------------------------------------------------------------
extern SCREENOperStat screenOperStat;
#endif

