//---------------------------------------------------------------------------

#ifndef STORAGEH
#define STORAGEH

#include "var.h"
#include "IODEVICE.h"

#include <pthread.h>
#include <stdio.h>
//---------------------------------------------------------------------------
class STORAGE:public IODEVICE
{
    public:

        STORAGE(void);
        ~STORAGE(void);

        bool OpenFile(char *,int);                                              //開檔
        bool CloseFile(void);                                                   //關檔
        bool ReadFile(void);                                                    //讀檔
        bool WriteFile(void);                                                   //寫檔
        bool WriteReturn(void);                                                 //寫換行

        bool WriteSSSettingFile(short int,int,int);
        bool ReadSSSettingFile(short int *,int *,int *);

        bool WriteVDRecordFile(int,YMDHMS,unsigned char,SeqNoContent, time_t);
        bool ReadVDRecordFile(int,YMDHMS *,unsigned char *,SeqNoContent *, time_t *);

        bool vWriteVDHistoryRecordFile(int,YMDHMS,unsigned char,SeqNoContent, time_t);
        bool vReadVDHistoryRecordFile(int,YMDHMS *,unsigned char *,SeqNoContent *, time_t *);


        bool vWrite92COMMSettingFile( unsigned char,
                                      char *,
                                      int,
                                      unsigned char,
                                      int *,                           //偵測器與控制器車道對應
                                      int,
                                      unsigned short int *,
                                      //OT Pass
                                      unsigned char,
                                      unsigned short int,
                                      //OT Debug Direct
                                      unsigned short int *,
                                      unsigned short int *,
//OT 961211
                                      int,int,int,int,int,
                                      int,int,int,int,int,
                                      int,int,int,int,int,

                                      unsigned char, unsigned char, unsigned char,
                                      unsigned char,

                                      //OT20111020
                                      int,int,int,int, int,int,int,int


                                      );

        bool vRead92COMMSettingFile( unsigned char *,
                                      char *,
                                      int *,
                                      unsigned char *,
                                      int *,
                                      int *,
                                      unsigned short int *,
                                      //OT Pass
                                      unsigned char *,
                                      unsigned short int *,
                                      //OT Debug Direct
                                      unsigned short int *,
                                      unsigned short int *,
//OT 961211
                                      int *,int *,int *,int *,int *,
                                      int *,int *,int *,int *,int *,
                                      int *,int *,int *,int *,int *,

                                      unsigned char *, unsigned char *, unsigned char *,
                                      unsigned char *,

                                      //OT2011020
                                      int *,int *,int *,int *, int *,int *,int *,int *

                                      );

        bool vWrite92VDSettingFile( unsigned char,
                                             unsigned char,
                                             unsigned char,
                                             unsigned char,
                                             short int,
                                             unsigned short int,
                                             unsigned short int,
                                             short int *,
                                             unsigned char );
        bool vRead92VDSettingFile( unsigned char *,
                                             unsigned char *,
                                             unsigned char *,
                                             unsigned char *,
                                             short int *,
                                             unsigned short int *,
                                             unsigned short int *,
                                             short int *,
                                             unsigned char *);

        bool vWrite92TCSettingFile(unsigned char, int, int, int, bool, unsigned short int, unsigned char, unsigned short int, unsigned char, unsigned char);
        bool vRead92TCSettingFile(unsigned char *, int *, int *, int *, bool *, unsigned short int *, unsigned char *, unsigned short int *, unsigned char *, unsigned char *);


         bool vWriteShareMEMALLFile(char *);

         bool vWriteMsgToFile(char *);
         bool vWriteReverseLog(char *);     //jacky20140507

         bool vWriteLastAliveTimeStampFile(time_t);                    //when SS disconnect, use this function to save time
         bool vReadLastAliveTimeStampFile(time_t *);

         bool vReadLCNFromFile(  bool *, unsigned int *);

         bool vWriteLast92TC_5F15Time(time_t);
         bool vReadLast92TC_5F15Time(time_t *);


         bool vWriteChainSettFile(unsigned char,
                                  unsigned char,
                                  unsigned char,
                                  unsigned char,
                                  unsigned char,
                                  unsigned char,
                                  unsigned char,
                                  unsigned char,
                                  short int *, short int *);


         bool vReadChainSettFile( unsigned char *,
                                  unsigned char *,
                                  unsigned char *,
                                  unsigned char *,
                                  unsigned char *,
                                  unsigned char *,
                                  unsigned char *,
                                  unsigned char *,
                                  short int *,
                                  short int * );


         bool vWriteCom2TypeFromFile(int);
         bool vReadCom2TypeFromFile(int *);
         bool vWriteMotherBoardTypeFromFile(unsigned char);
         bool vReadMotherBoardTypeFromFile(unsigned char *);


    private:
        pthread_mutex_t mutexDisk;                                              //保護記憶體

        char nowOpen[64];
        FILE *fileD;

};
//---------------------------------------------------------------------------
#endif
