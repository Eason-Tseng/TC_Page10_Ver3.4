//---------------------------------------------------------------------------
#ifndef WRITEJOBH
#define WRITEJOBH

#include "var.h"
#include "JOB.h"

//---------------------------------------------------------------------------
class WRITEJOB:public JOB
{
    public:

        WRITEJOB(void);
        ~WRITEJOB(void);

        bool WriteWorkByMESSAGEOUT(MESSAGEOK);

        bool WriteSUCCESS92(BYTE,BYTE);                                         //92�~���^�����\(14) AA BB SEQ LCN LCN LEN LEN OF 80 CommandID1 CommandID2 AA CC CKS
        bool WriteFAIL92(BYTE,BYTE,BYTE,BYTE);                                  //92�~���^������(16) AA BB SEQ LCN LCN LEN LEN OF 81 CommandID1 CommandID2 ErrorCode ParameterNumber AA CC CKS
        bool WriteSUCCESS87(void);                                              //87�~���^�����\(  )
        bool WriteFAIL87(void);                                                 //87�~���^������

        bool WriteACK92(BYTE);                                                  //92�~�����{���X(8) AA DD SEQ LCN LCN LEN LEN CKS
        bool WriteNAK92(BYTE,BYTE);                                             //92�~���t�{���X(9) AA EE SEQ LCN LCN LEN LEN ERR CKS
        bool WriteACK77(void);                                                  //77�~�����{���X(5) AA DD LCN LCN CKS
        bool WriteNAK77(BYTE);                                                  //77�~���t�{���X(6) AA EE LCN LCN ERR CKS

        bool WritePhysicalOut(BYTE *,int,int);                                  //��ڼg�X(��J��w,����,���g�X��DEVICE)
        bool WritePhysicalOutNoSetSeqNoResend(BYTE *,int, int);

        //OT Pass
        bool WriteACK92_To_Term(BYTE, BYTE, BYTE, unsigned char);

        bool WriteAskW77E58FWVer();
        bool WriteLetW77E58AutoControl();
        int CheckDoubleAA(BYTE *, int);

    private:
        BCDSW bcdSwitchHi;
        BCDSW bcdSwitchLo;

};
//---------------------------------------------------------------------------
extern WRITEJOB writeJob;
#endif

