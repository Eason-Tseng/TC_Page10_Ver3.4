#ifndef SCREENGreenConflictRecordH
#define SCREENGreenConflictRecordH

#include "SCREEN.h"
#include "var.h"
//---------------------------------------------------------------------------
class ScreenGreenConflictRecord:public SCREEN
{
    public:
        ScreenGreenConflictRecord(void);
        ~ScreenGreenConflictRecord(void);

        void DisplayGreenConflictRecord(void);
        void vShowGreenConflictRecord(void);                                             
        void DoKeyWork(BYTE);                                             

    private:
        DISP_WORD GreenConflictWord[6][18];
        BYTE GreenConflictRecordBitmap[3840];                                               
        void initDispWord(void);
        void loadBitmapFromFile(void);
        void DoKeyF1Work();
        void DoKeyF4Work();
        void DoKeyEnterWork();
};
//---------------------------------------------------------------------------
extern ScreenGreenConflictRecord screenGreenConflictRecord;
#endif

