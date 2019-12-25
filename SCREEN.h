//---------------------------------------------------------------------------
#ifndef SCREENH
#define SCREENH

#include "var.h"
//---------------------------------------------------------------------------
    struct DISP_WORD{      //顯示在畫面上的字
           int X;          //X軸座標
           int Y;          //Y軸座標
           int width;      //顯示寬度
           int height;     //顯示高度
    };
//---------------------------------------------------------------------------
class SCREEN
{
    public:
        SCREEN(void);
        ~SCREEN(void);

        bool DISPLAY8X16(BYTE,unsigned int);                                    //顯示8X16的字
        bool DISPLAY16X16(BYTE,BYTE,unsigned int);                              //顯示16X16的字
        bool DISPLAY12X24(BYTE,unsigned int);                                   //顯示12X24的字
        bool DISPLAY24X24(BYTE,BYTE,unsigned int);                              //顯示24X24的字

        bool GetWord8X16FromFile(BYTE);                                         //得到8X16的bitmap
        bool GetWord16X16FromFile(BYTE,BYTE);                                   //得到16X16的bitmap
        bool GetWord12X24FromFile(BYTE);                                        //得到12X24的bitmap
        bool GetWord24X24FromFile(BYTE,BYTE);                                   //得到24X24的bitmap

        void GetDefaultWord8x16FromFile(void);                                  //從檔案中得到DefaultBitmap 8x16  : 0123456789ABCDEF
        void GetDefaultWord16x16FromFile(void);                                 //從檔案中得到DefaultBitmap 16x16 : 0123456789ABCDEF
        void GetDefaultWord12x24FromFile(void);                                 //從檔案中得到DefaultBitmap 12x24 : 0123456789ABCDEF
        void GetDefaultWord24x24FromFile(void);                                 //從檔案中得到DefaultBitmap 24x24 : 0123456789ABCDEF
        void GetDefaultArrow8x16FromFile(void);                                 //從檔案中得到DefaultBitmap 8x16  : 上下左右
        void GetDefaultArrow16x16FromFile(void);                                //從檔案中得到DefaultBitmap 16x16 : 上下左右
        void GetDefaultSelectFormFile(void);                                    //從檔案中得到DefaultBitmap 16x16 : 未選取(空白),選取及游標底線


        BYTE word8x16Temp[16];                                                  //顯示8x16文字的暫存區
        BYTE word16x16Temp[32];                                                 //顯示16x16文字的暫存區
        BYTE word12x24Temp[48];                                                 //顯示12x16文字的暫存區
        BYTE word24x24Temp[72];                                                 //顯示24x24文字的暫存區

        BYTE word8x16Light[16];
        BYTE word8x16Die[16];
        BYTE word8x16Flash[16];

        void ClearWord8X16(void) {for (int i=0;i<16;i++) word8x16Temp[i]=0;}    //清空8x16暫存區
        void ClearWord16X16(void) {for (int i=0;i<32;i++) word16x16Temp[i]=0;}  //清空16x16暫存區
        void ClearWord12X24(void) {for (int i=0;i<48;i++) word12x24Temp[i]=0;}  //清空12x16暫存區
        void ClearWord24X24(void) {for (int i=0;i<72;i++) word24x24Temp[i]=0;}  //清空24x24暫存區

        BYTE word8x16[22][16];                                                  //放Bitmap 8x16 : 0123456789ABCDEFabcdef
        BYTE word16x16[22][32];                                                 //放Bitmap 16X16: 0123456789ABCDEFabcdef
        BYTE word12x24[22][48];                                                 //放Bitmap 12X24: 0123456789ABCDEFabcdef
        BYTE word24x24[22][72];                                                 //放Bitmap 24X24: 0123456789ABCDEFabcdef
        BYTE arrow8x16[4][16];                                                  //放Bitmap 8X16 : 上下左右
        BYTE arrow16x16[4][32];                                                 //放Bitmap 16X16: 上下左右
        BYTE select16x16[2][32];                                                //放Bitmap 16X16: 未選取(空白),選取

        void setCursor24x24(int,int);                                           //點游標
        void clearCursor24x24(int,int);                                         //清除游標
        void setCursor16x16(int,int);                                           //點游標
        void clearCursor16x16(int,int);                                         //清除游標
        void setCursor8x16(int,int);                                            //點游標
        void clearCursor8x16(int,int);                                          //清除游標
        void setSelectPoint16x16(int,int,int,int);
        void clearSelectPoint16x16(int,int,int,int);
        void clearSelectPoint24x24(int,int,int,int);

    private:
};
//---------------------------------------------------------------------------
#endif
