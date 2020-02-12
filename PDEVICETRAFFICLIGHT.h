//---------------------------------------------------------------------------

#include "var.h"
//---------------------------------------------------------------------------
class PDEVICETRAFFICLIGHT
{
    public:
        unsigned char returnnum[3]; //Hi = 第幾燈卡 (1~6), Lo = 方向 (1 = 左轉,2 = 直行,3 = 圓綠)
        PDEVICETRAFFICLIGHT(void);
        ~PDEVICETRAFFICLIGHT(void);
        bool DoWorkViaPDevice(MESSAGEOK);                             
        void GreenConflictcode(BYTE, BYTE, BYTE); //Eason_Ver3.4

    private:
        int iAskW77E58CounterReturn;

        DATA_Bit GC_DATA0;
        DATA_Bit GC_DATA1;
        DATA_Bit GC_DATA2;
        void BRTGreenConflict(BYTE, BYTE, BYTE, BYTE);


};
//---------------------------------------------------------------------------
