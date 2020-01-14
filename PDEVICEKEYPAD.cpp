//---------------------------------------------------------------------------
#include "PDEVICEKEYPAD.h"
#include "SMEM.h"
#include "WRITEJOB.h"
#include "SCREENNone.h"
#include "SCREENLogo.h"
#include "SCREENLogin.h"
#include "SCREENLoginErr.h"
#include "SCREENMain.h"
#include "SCREENCtlSetup.h"
#include "SCREENCtlNo.h"
#include "SCREENLastDown.h"
#include "SCREENModifyDT.h"
#include "SCREENFWVerDT.h"
#include "SCREENHWCycle.h"
#include "SCREENModifyPWD.h"
#include "SCREENDBSetup.h"
#include "SCREENOperStat.h"
#include "SCREENOperMode.h"
#include "SCREENCommStat.h"

#include "SCREENSendCycle.h"
#include "SCREENUpdateDB.h"

#include "SCREENOnline.h"
#include "SCREENOffline.h"
#include "SCREENLockdb.h"
#include "SCREENTotalRun.h"
#include "SCREENABOErr.h"
#include "SCREENCmdSet.h"
#include "SCREENHWReset.h"
#include "SCREENLcnEncode.h"

#include "screenPlan.h"
//Removed #include "screenPlanMenu.h"
#include "screenWeekDay.h"
//Removed #include "screenPlanSel.h"
#include "screenSegtype.h"
#include "screenSegtypeMenu.h"
#include "screenSpecialDay.h"
#include "screenSegtypeSel.h"
#include "screenRunTableList.h"
#include "screenStrategy.h"
#include "screenRTMSInfo.h"
#include "screenCurrentLightStatus.h"
#include "screenCurrentCommPacket.h"

//Removed #include "screenPlanEditStep1.h"
//Removed #include "screenPlanEditStep2_keySubPhaseCount.h"
//Removed #include "screenPlanEditStep3.h"
//Removed #include "screenPlanEditStep4.h"

#include "screenRedCountHWCheck.h"
#include "screenRedCountHWCheckSel.h"
#include "screenRedCountMenu.h"
#include "screenRedCountHWVerMenu.h"
#include "SCREENRedCountMap.h"

//Removed #include "screenSegTypeEditStep1.h"
//Removed #include "screenSegTypeEditStep2.h"
//Removed #include "screenSegTypeEditStep3.h"

#include "screenReverseMenu.h"
#include "screenReverse.h"
#include "screenRevWeekDay.h"
#include "screenRevSpecialDay.h"

//OT Debug 0523
#include "screenActuateEdit.h"

//OT Pass
#include "screenPassModeSet.h"

//OT Debug Direct
#include "SCREENDirMap.h"
#include "screenDirLightBoardMappingMenu.h"
#include "SCREENLightBoardMap.h"

//OT Debug 0523
#include "screenPlanEditF2.h"
#include "screenSegtypeEditF2.h"
#include "screenWeekDayEditF2.h"
#include "screenSpecialDayView.h"
#include "screenSpecialDayEditF2.h"
#include "screenPhasePreview.h"
#include "screenSegtypePreview.h"
#include "screenPlanPreview.h"

#include "SCREENLast92TCPlanSegmentUpdate.h"

#include "screenChainMenu.h"
#include "screenMotherChainSet.h"
#include "screenChildChainSet.h"
#include "screenChainStatus.h"

#include "screenLightPreview.h"
#include "screenLightEdit.h"

#include "SCREENNetworkSetConfirm.h"
#include "SCREENNetworkSet.h"
#include "SCREENNetmaskSet.h"

#include "SCREENMBSelect.h"
#include "SCREENUart2.h"
#include "SCREENWarning.h"

#include "SCREENTFDStatus.h"

#include "screenRedCountHWCheckDevID.h"
#include "screenRedCountHWCheckCycleNum.h"
#include "screenRedCountHWCheckNum88.h"

#include "screenRailwayChainSet.h"

#include "screenActuateArwenStatus.h"

/*OTCombo0714*/
#include "CDataToMessageOK.h"
#include "CSTC.h"
#include "unistd.h"

#include <stdio.h>
//---------------------------------------------------------------------------
PDEVICEKEYPAD::PDEVICEKEYPAD(void)
{

//  pthread_mutex_init(&mutexKeyPad,NULL);

  cSwitch=999;      //����}���]���O

  _LastDATA_P6.DBit = 0xFF;

  ucLastData[0] = 0x00;
  ucLastData[1] = 0x01;
  ucLastData[2] = 0x02;
  ucLastData[3] = 0x03;
  ucLastData[4] = 0x04;
  ucLastData[5] = 0x05;

//  firstTime=true;
//  stc.SetDefaultLCNPhaseOrder(1,0);
}
//---------------------------------------------------------------------------
PDEVICEKEYPAD::~PDEVICEKEYPAD(void)
{
}
//---------------------------------------------------------------------------
bool PDEVICEKEYPAD::DoWorkViaPDevice(MESSAGEOK message)
{
try {
    unsigned short int usiTmp;
    int address=0;
    unsigned char ucCSTCControlStrategy;
    unsigned char ucNewCSTCControlStrategy;

    ucCSTCControlStrategy = smem.vGetUCData(TC_CSTC_ControlStrategy);
    ucNewCSTCControlStrategy = 0xFF;

    switch (message.packet[2]) {
            case 0x01:                                 //keypad response
                 smem.SetRequestKeypad(0);

                 DATA_P1=message.packet[3];            //25����L�[���s(���s���ϥ�)
                 DATA_P2=message.packet[4];            //���ϥ�(TC�Ψ�ŪAD��)
                 DATA_P3.DBit=message.packet[5];       //SW27~SW34(�Ȩϥ�SW27���ݩβ{���ާ@)
                 DATA_P4=message.packet[6];            //SW35(Address Low Byte)
                 DATA_P5=message.packet[7];            //SW36(Address Hi Byte)
                 DATA_P6.DBit=message.packet[8];            //SW37

                 if(DATA_P1 != 0) { // next step or press key
                   smem.SetLastKeypadTime();
                 } else if(ucLastData[2] != DATA_P3.DBit) {  //switch button
                   smem.SetLastKeypadTime();
                 } else if(ucLastData[3] != DATA_P4) {  //LCN
                   smem.SetLastKeypadTime();
                 } else if(ucLastData[4] != DATA_P5) {  //LCN
                   smem.SetLastKeypadTime();
                 } else if(ucLastData[5] != DATA_P6.DBit) {  //Phase
                   smem.SetLastKeypadTime();
                 } else {

                   /*
                   ucLastData[0] = DATA_P1;
                   ucLastData[1] = DATA_P2;
                   ucLastData[2] = DATA_P3.DBit;
                   ucLastData[3] = DATA_P4;
                   ucLastData[4] = DATA_P5;
                   ucLastData[5] = DATA_P6.DBit;
                   return true;
                   */
                 }
                 ucLastData[0] = DATA_P1;
                 ucLastData[1] = DATA_P2;
                 ucLastData[2] = DATA_P3.DBit;
                 ucLastData[3] = DATA_P4;
                 ucLastData[4] = DATA_P5;
                 ucLastData[5] = DATA_P6.DBit;


//OTCombo0714
                 if (DATA_P1==0x99) {                     //���B��
                   if (cSwitch==3)                                     //�p�G�b���
                         stc.Lock_to_Set_Next_Step();
                 }
                 else if (DATA_P1>=0x80 && DATA_P1<=0x98)              //����L,0x99��ʤ���,0�]�����\�i�J
                     DoDisplayLcdWorkByData_P1(DATA_P1);          //�@������䳣�O�b����LCD�ܤ�

                 address=65535-((int)DATA_P5*256+(int)DATA_P4);   //�o����f�s��
                 if (address!=smem.GetAddress()) {
                     if (address>=0) smem.SetAddress(address);
                     else smem.SetAddress(0);

                     screenCtlNo.DisplayNum();
                 }

                 //���ݾާ@��1,�{���ާ@��0
//                 if (DATA_P3.switchBit.b1==0 && smem.GetOperMode()==1)        //�p�G�쥻�O���ݾާ@,�{�b������{���ާ@

/*OTCombo0713
                 if (DATA_P3.switchBit.b1!=smem.GetOperMode())                  //�@���o�{���@�˴N�g�J�s����
                     smem.SetOperMode(DATA_P3.switchBit.b1);
*/
//OTCombo0714
                 if (smem.vGetBOOLData(TC_SignalConflictError) == true) { //anothor flash control, GreenConflict
                   if(ucCSTCControlStrategy != 80) {
                     while(ucNewCSTCControlStrategy != 80) {
//                            if (cSwitch!=2) {
                       cSwitch=2;
                       stc.Lock_to_Set_Control_Strategy(STRATEGY_FLASH);
                       ucNewCSTCControlStrategy = smem.vGetUCData(TC_CSTC_ControlStrategy);
                       if(ucNewCSTCControlStrategy != 80) { sleep(1); }
                     }
                   }
                 }
                 else if (DATA_P3.switchBit.b1==0) {                                               //����
                     if(ucCSTCControlStrategy != 90) {
                       while(ucNewCSTCControlStrategy != 90) {
//                     if (cSwitch!=1) {                                                       //�p�G���O����
                         cSwitch=1;                                                          //�����̤j
                         stc.Lock_to_Set_Control_Strategy(STRATEGY_ALLRED);                  //�]�w�����
                         ucNewCSTCControlStrategy = smem.vGetUCData(TC_CSTC_ControlStrategy);

                         //force close 5F18
                         if(smem.vGet5F18EffectTime() > 0) {
                           smem.vSet5F18EffectTime(2);
                         }

                         if(ucNewCSTCControlStrategy != 90) { sleep(1); }
                       }
                     }
                 } else if (DATA_P3.switchBit.b2==0) {                                        //�{��
                            if(ucCSTCControlStrategy != 80) {
                              while(ucNewCSTCControlStrategy != 80) {
//                            if (cSwitch!=2) {
                                cSwitch=2;
                                stc.Lock_to_Set_Control_Strategy(STRATEGY_FLASH);
                                ucNewCSTCControlStrategy = smem.vGetUCData(TC_CSTC_ControlStrategy);
                                if(ucNewCSTCControlStrategy != 80) { sleep(1); }
                              }
                            }
                 } else if (DATA_P3.switchBit.b3==0) {                                        //���
                            if(ucCSTCControlStrategy != 70) {

                              //change to TOD first.
                              while(ucNewCSTCControlStrategy != 10) {
//                              if(cSwitch==1 || cSwitch==2) {                    //�����e���{��or����
//                                                            while(ucNewCSTCControlStrategy != 80) {

//                              printf("[OTOTOT] GOTO TOD\n");

printf("Lock_to_Set_Control_Strategy by KEYPAD!!!\n");
printf("Lock_to_Set_Control_Strategy by KEYPAD!!!\n");
printf("Lock_to_Set_Control_Strategy by KEYPAD!!!\n");


                                stc.Lock_to_Set_Control_Strategy(STRATEGY_TOD); //���ܦ^TOD��phaseŪ�i��
                                usleep(200);                                    //�p�G�S��sleep, strategy�ܤӧ�, CSTC��������
                                ucNewCSTCControlStrategy = smem.vGetUCData(TC_CSTC_ControlStrategy);
                                if(ucNewCSTCControlStrategy != 10) { sleep(1); }
                              }
                              while(ucNewCSTCControlStrategy != 70) {
                                cSwitch=3;
                                printf("[OTOTOT] GOTO MANUAL\n");
                                stc.Lock_to_Set_Control_Strategy(STRATEGY_MANUAL);
                                ucNewCSTCControlStrategy = smem.vGetUCData(TC_CSTC_ControlStrategy);
                                if(ucNewCSTCControlStrategy != 70) { sleep(1); }
                              }
                            }
                 } else {                                                                    //�۰�
                      if(ucCSTCControlStrategy == 10 || ucCSTCControlStrategy == 95 || ucCSTCControlStrategy == 96) {
                      } else  {
//                        while(ucNewCSTCControlStrategy != 10) {
                        while(  !(ucNewCSTCControlStrategy == 10 || ucNewCSTCControlStrategy == 96) ) {
//                        if (cSwitch!=0) {
                            cSwitch=0;

printf("Lock_to_Set_Control_Strategy by KEYPAD2!!!\n");
printf("Lock_to_Set_Control_Strategy by KEYPAD2!!!\n");
printf("Lock_to_Set_Control_Strategy by KEYPAD2!!!\n");

                            stc.Lock_to_Set_Control_Strategy(STRATEGY_TOD);
                            ucNewCSTCControlStrategy = smem.vGetUCData(TC_CSTC_ControlStrategy);
                            if(ucNewCSTCControlStrategy != 10) { sleep(1); }
                        }
                      }
                 }

           //OT FIX Door
                 if(DATA_P3.switchBit.b4 == 1) {                                  //DoorOpen
//                 if(DATA_P3.switchBit.b4 == 0) {                                //DoorOpen
                   if(smem.vGetBOOLData(TC_DoorStatus_Change) == false) {
                     //OT FIX Door
                     smem.vSetBOOLData(TC_DoorStatus_Change, true);
                     unsigned char uc0F04[4];
                     uc0F04[0] = 0x0F;
                     uc0F04[1] = 0x04;
                     MESSAGEOK _MSG;
                     //send HW Status
                     uc0F04[2] = smem.vGetHardwareStatus(3);        //���oHWstat
                     uc0F04[3] = smem.vGetHardwareStatus(4);        //���oHWstat
                     _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(uc0F04, 4, true);
                     _MSG.InnerOrOutWard = cOutWard;
                     writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICECENTER92);
                   }
                   //OT FIX Door
                 } else {
                   if(smem.vGetBOOLData(TC_DoorStatus_Change) == true) {
                     //OT Fix Door
                     smem.vSetBOOLData(TC_DoorStatus_Change, false);
                     unsigned char uc0F04[4];
                     uc0F04[0] = 0x0F;
                     uc0F04[1] = 0x04;
                     MESSAGEOK _MSG;
                     //send HW Status
                     uc0F04[2] = smem.vGetHardwareStatus(3);        //���oHWstat
                     uc0F04[3] = smem.vGetHardwareStatus(4);        //���oHWstat
                     _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(uc0F04, 4, true);
                     _MSG.InnerOrOutWard = cOutWard;
                     writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICECENTER92);
                     screenNone.DisplayNone();
                   }
                 }

                 //train trigger.
                 if(DATA_P3.switchBit.b5 == 0) {
                   smem.vSetUCData(TC_TrainChainNOW, 1);
                 } else {
                   smem.vSetUCData(TC_TrainChainNOW, 0);
                 }

//OT Debug 0523
                 if(DATA_P3.switchBit.b6 == 0 && cSwitch != 3 && smem.vGetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable)) {                                //Actuate
                   printf("Get Walkman Button Signal.\n");

                   if( smem.vGetUCData(TC_CCT_Send_ActuateArwen_Protocal) > 0 ) {
                       ucData[0] = 0x5F;
                       ucData[1] = 0xA6;
                       ucData[2] = HI(60000);
                       ucData[3] = LO(60000);
                       _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(ucData, 4, true);
                       _MSG.InnerOrOutWard = cOutWard;
                       writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICECCJDYNCTL);
                   } else if(smem.vGetUCData(TC92_ucControlStrategy) == 0x01) {
                       printf("smem.vGetUCData(TC92_ucControlStrategy):%d.\n", smem.vGetUCData(TC92_ucControlStrategy));
                       if(smem.vGetBOOLData(TC_CCTActuate_TOD_Running) == false) {
                         smem.vSetBOOLData(TC_CCT_In_LongTanu_ActuateType_Switch, true);
                         stc.vChangeTODCurrentStepSec(2, 0);
                       } else {
                         usiTmp = stc.vGetUSIData(CSTC_exec_phase_current_subphase);
                         if(usiTmp != 0) {                                        //Not First Subphase, can re-trigger
                           smem.vSetBOOLData(TC_CCT_In_LongTanu_ActuateType_Switch, true);
                         }
                       }
                   }

/*
                   if(smem.vGetUCData(TC92_ucControlStrategy) == 0x01) {        //0x04 == TOD

                   if( smem.vGetUCData(TC_CCT_Send_ActuateArwen_Protocal) > 0 ) {
                       ucData[0] = 0x5F;
                       ucData[1] = 0xA6;
                       ucData[2] = HI(60000);
                       ucData[3] = LO(60000);
                       _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(ucData, 4, true);
                       _MSG.InnerOrOutWard = cOutWard;
                       writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICECCJDYNCTL);
                     } else {

                       if(smem.vGetBOOLData(TC_CCTActuate_TOD_Running) == false) {
                         smem.vSetBOOLData(TC_CCT_In_LongTanu_ActuateType_Switch, true);
                         stc.vChangeTODCurrentStepSec(2, 0);
                       } else {
                         usiTmp = stc.vGetUSIData(CSTC_exec_phase_current_subphase);
                         if(usiTmp != 0) {                                        //Not First Subphase, can re-trigger
                           smem.vSetBOOLData(TC_CCT_In_LongTanu_ActuateType_Switch, true);
                         }
                       }

                     }
                   }
*/
                 }

                 if(DATA_P3.switchBit.b6 == 0 && cSwitch != 3 && smem.vGetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable)) {                                //Actuate
                   printf("Get Walkman Button Signal.\n");
                   printf("smem.vGetUCData(TC92_ucControlStrategy):%d.\n", smem.vGetUCData(TC92_ucControlStrategy));
                   if(smem.vGetUCData(TC92_ucControlStrategy) == 0x01) {        //0x04 == TOD

                   if( smem.vGetUCData(TC_CCT_Send_ActuateArwen_Protocal) > 0 ) {
                       ucData[0] = 0x5F;
                       ucData[1] = 0xA6;
                       ucData[2] = HI(60000);
                       ucData[3] = LO(60000);
                       _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(ucData, 4, true);
                       _MSG.InnerOrOutWard = cOutWard;
                       writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICECCJDYNCTL);
                     } else {

                       if(smem.vGetBOOLData(TC_CCTActuate_TOD_Running) == false) {
                         smem.vSetBOOLData(TC_CCT_In_LongTanu_ActuateType_Switch, true);
                            //                                                  stc.AllRed5Seconds();
                            // stc.Lock_to_Set_Control_Strategy(STRATEGY_TOD);
                         stc.vChangeTODCurrentStepSec(2, 0);
                       } else {
                         usiTmp = stc.vGetUSIData(CSTC_exec_phase_current_subphase);
                         if(usiTmp != 0) {                                        //Not First Subphase, can re-trigger
                           smem.vSetBOOLData(TC_CCT_In_LongTanu_ActuateType_Switch, true);
                         }
                       }

                     }
                   }
                 }


                 //DATA_P2���B�z
                 _LastDATA_P6.DBit = smem.vGetUCData(TC_KeyPadP6Value);

                 if(DATA_P6.switchBit.b1 == 0) DATA_P6.switchBit.b1 = 1; else DATA_P6.switchBit.b1 = 0;
                 if(DATA_P6.switchBit.b2 == 0) DATA_P6.switchBit.b2 = 1; else DATA_P6.switchBit.b2 = 0;
                 if(DATA_P6.switchBit.b3 == 0) DATA_P6.switchBit.b3 = 1; else DATA_P6.switchBit.b3 = 0;
                 if(DATA_P6.switchBit.b4 == 0) DATA_P6.switchBit.b4 = 1; else DATA_P6.switchBit.b4 = 0;
                 if(DATA_P6.switchBit.b5 == 0) DATA_P6.switchBit.b5 = 1; else DATA_P6.switchBit.b5 = 0;
                 if(DATA_P6.switchBit.b6 == 0) DATA_P6.switchBit.b6 = 1; else DATA_P6.switchBit.b6 = 0;
                 if(DATA_P6.switchBit.b7 == 0) DATA_P6.switchBit.b7 = 1; else DATA_P6.switchBit.b7 = 0;
                 if(DATA_P6.switchBit.b8 == 0) DATA_P6.switchBit.b8 = 1; else DATA_P6.switchBit.b8 = 0;
                 if(_LastDATA_P6.DBit != DATA_P6.DBit) {

                   printf("DATA_P6 Change%d\n", DATA_P6.DBit);
                   printf("_LastDATA_P6.DBit Change%d\n", _LastDATA_P6.DBit);
                   vSavePlan(DATA_P6.DBit);
                   smem.vSetUCData(TC_KeyPadP6Value, DATA_P6.DBit);

                 }
                 _LastDATA_P6.DBit = DATA_P6.DBit;

                 if (smem.GetKeypadReturn()==false) {
                     smem.SetKeypadReturn(true);

                     SendStartMsg();
                 }


            break;

            case 0x03:                                 //keypad request
                 writeJob.WritePhysicalOut(message.packet,message.packetLength,DEVICEKEYPAD);
            break;
            default:
            break;
    }

    return true;

  } catch (...) {}
}

void PDEVICEKEYPAD::DoDisplayLcdWorkByData_P1(BYTE key)
{
try {
    switch (smem.GetcFace()) {
            case cNONE:
                 screenNone.DoKeyWork(key);
            break;
            case cLOGO:
                 screenLogo.DoKeyWork(key);
            break;
            case cLOGIN:
                 screenLogin.DoKeyWork(key);
            break;
            case cLOGINERR:
                 screenLoginErr.DoKeyWork(key);
            break;
            case cMAIN:
                 screenMain.DoKeyWork(key);
            break;
            case cCTLSETUP:
                 screenCtlSetup.DoKeyWork(key);
            break;
            case cCTLNO:
                 screenCtlNo.DoKeyWork(key);
            break;
            case cLASTDOWN:
                 screenLastDown.DoKeyWork(key);
            break;
            case cMODIFYDT:
                 screenModifyDT.DoKeyWork(key);
            break;
            case cFWVERDT:
                 screenFWVerDT.DoKeyWork(key);
            break;
            case cHWCYCLE:
                 screenHWCycle.DoKeyWork(key);
            break;
            case cMODIFYPWD:
                 screenModifyPWD.DoKeyWork(key);
            break;
            case cDBSETUP:
                 screenDBSetup.DoKeyWork(key);
            break;
            case cOPERSTAT:
                 screenOperStat.DoKeyWork(key);
            break;
            case cOPERMODE:
                 screenOperMode.DoKeyWork(key);
            break;
            case cCOMMSTAT:
                 screenCommStat.DoKeyWork(key);
            break;

            case cSENDCYCLE:
                 screenSendCycle.DoKeyWork(key);
            break;
            case cUPDATEDB:
                 screenUpdateDB.DoKeyWork(key);
            break;

            case cONLINE:
                 screenOnline.DoKeyWork(key);
            break;
            case cOFFLINE:
                 screenOffline.DoKeyWork(key);
            break;
            case cLOCKDB:
                 screenLockdb.DoKeyWork(key);
            break;
            case cTOTALRUN:
                 screenTotalRun.DoKeyWork(key);
            break;
            case cABOERROR:
                 screenABOErr.DoKeyWork(key);
            break;
            case cCMDSET:
                 screenCmdSet.DoKeyWork(key);
            break;
            case cHWRESET:
                 screenHWReset.DoKeyWork(key);
            break;
            case cLCNENCODE:
                 screenLcnEncode.DoKeyWork(key);
            break;

//OTADD
            case cSEGTYPE:
                 screenSegtype.doKeyWork(key);
            break;
            case cSEGTYPEMENU:
                 screenSegtypeMenu.doKeyWork(key);
            break;
            case cWEEKDAY:
                 screenWeekDay.doKeyWork(key);
            break;
            case cSPECIALDAY:
                 screenSpecialDay.doKeyWork(key);
            break;
            case cSEGTYPESEL:
                 screenSegtypeSel.doKeyWork(key);
            break;

//Removed
/*
            case cPLANMENU:
                 screenPlanMenu.doKeyWork(key);
            break;
            case cPLANSEL:
                 screenPlanSel.doKeyWork(key);
            break;
*/
            case cPLAN:
                 screenPlan.doKeyWork(key);
            break;

            case cRUNTABLE:
                 screenRunTableList.doKeyWork(key);
            break;

            case cSTRATEGY:
                 screenStrategy.doKeyWork(key);
            break;

            case cRTMSInfo:
                 screenRTMSInfo.doKeyWork(key);
            break;

            case cCURRENTLIGHTSTATUS:
              screenCurrentLightStatus.doKeyWork(key);
            break;

            case cCURRENTCOMMPACKET:
              screenCurrentCommPacket.doKeyWork(key);
            break;

//Removed
/*
            case cPLANEditStep1:
              screenPlanEditStep1.doKeyWork(key);
            break;

            case cPLANEditStep2:
              screenPlanEditStep2_keySubPhaseCount.doKeyWork(key);
            break;

            case cPLANEditStep3:
              screenPlanEditStep3.doKeyWork(key);
            break;

            case cPLANEditStep4:
              screenPlanEditStep4.doKeyWork(key);
            break;
*/

            case cREDCOUNTMENU:
              screenRedCountMenu.doKeyWork(key);
            break;
            case cREDCOUNTHWCHECKSEL:
              screenRedCountHWCheckSel.doKeyWork(key);
            break;
            case cREDCOUNTHWCHECK:
              screenRedCountHWCheck.doKeyWork(key);
            break;

            case cREDCOUNTHWVERMENU:
              screenRedCountHWVerMenu.doKeyWork(key);
            break;

            case cREDCOUNTMAPPING:
              screenRedCountMap.DoKeyWork(key);
            break;

//Removed
/*
            case cSegTypeEditStep1:
              screenSegTypeEditStep1.doKeyWork(key);
            break;

            case cSegTypeEditStep2:
              screenSegTypeEditStep2.doKeyWork(key);
            break;

            case cSegTypeEditStep3:
              screenSegTypeEditStep3.doKeyWork(key);
            break;
*/

            case cREVERSETIMTMENU:
              screenReverseMenu.doKeyWork(key);
            break;
            case cREVWEEKDAY:
              screenRevWeekDay.doKeyWork(key);
            break;
            case cREVSPECIALDAY:
              screenRevSpecialDay.doKeyWork(key);
            break;
            case cREVERSE:
              screenReverse.doKeyWork(key);
            break;

//OT Debug 0523
            case cActuateEdit:
              screenActuateEdit.doKeyWork(key);
            break;

//OT Pass
            case cPassModeSet:
              screenPassModeSet.doKeyWork(key);
            break;

            //OT Debug Direct
            case cDIRMAPPING:
              screenDirMap.DoKeyWork(key);
            break;
            case cLIGHTBOARDMAPPING:
              screenLightBoardMap.DoKeyWork(key);
            break;
            case cDIRLIGHTBOARDMAPPINGMENU:
              screenDirLightBoardMappingMenu.doKeyWork(key);
            break;

            case cPLANF2:
              screenPlanEditF2.doKeyWork(key);
            break;

            case cSEGTYPEEDITF2:
                 screenSegtypeEditF2.doKeyWork(key);
            break;

            case cWEEKDAYEDITF2:
                 screenWeekDayEditF2.doKeyWork(key);
            break;

            case cSPECIALDAYVIEW:
                 screenSpecialDayView.doKeyWork(key);
            break;

            case cSPECIALDAYEDITF2:
                 screenSpecialDayEditF2.doKeyWork(key);
            break;

            case cPHASESEL:
                 screenPhasePreview.doKeyWork(key);
            break;

            case cSEGTYPEPREVIEW:
                 screenSegtypePreview.doKeyWork(key);
            break;

            case cPLANPREVIEW:
                 screenPlanPreview.doKeyWork(key);
            break;

            case cPHASEPREVIEW:
//                 screenSegtypePreview.doKeyWork(key);
            break;

            case cLAST92TCPLANSEGMENTUPDATE:
              screenLast92TCPlanSegmentUpdate.DoKeyWork(key);
            break;

            case cCHAINMENU:
              screenChainMenu.doKeyWork(key);
            break;
            case cMOTHERCHAINSET:
              screenMotherChainSet.DoKeyWork(key);
            break;
            case cCHILDCHAINSET:
                  screenChildChainSet.DoKeyWork(key);
            break;
            case cCHAINSTATUS:
                  screenChainStatus.doKeyWork(key);
            break;

            case cLIGHTPREVIEW:
                  screenLightPreview.doKeyWork(key);
            break;
            case cLIGHTEDIT:
                  screenLightEdit.doKeyWork(key);
            break;

            case cNETWORKSET:
                  screenNetworkSet.DoKeyWork(key);
            break;
            case cNETMASKSET:
                  screenNetmaskSet.DoKeyWork(key);
            break;

            case cNETWORKSETCONFIRM:
                  screenNetworkSetConfirm.DoKeyWork(key);
            break;

            case cMBSELECT:
                  screenMBSelect.DoKeyWork(key);
            break;
            case cUART2SELECT:
                  screenUart2.DoKeyWork(key);
            break;
            case cWARNING:
                  screenWarning.DoKeyWork(key);
            break;
            case cTFDSTATUS:
                  screenTFDStatus.DoKeyWork(key);
            break;

            case cREDCOUNTHWCHECKDEVID:
                  screenRedCountHWCheckDevID.doKeyWork(key);
            break;
            case cREDCOUNTHWCHECKCYCLENUM:
                  screenRedCountHWCheckCycleNum.doKeyWork(key);
            break;
            case cREDCOUNTHWCHECKNUM88:
                  screenRedCountHWCheckNum88.doKeyWork(key);
            break;

            case cRAILWAYCHAINSET:
                  screenRailwayChainSet.DoKeyWork(key);
            break;

            case cACTUATEARWENSTATUS:
                  screenActuateArwenStatus.doKeyWork(key);
            break;


            default:
            //OTMARKPRINTF  printf("[Error Message] Key No Define!\n");
            break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void PDEVICEKEYPAD::SendStartMsg(void)
{
try {

  MESSAGEOK _MSG1,_MSG2,_MSG3,_MSG4;
  unsigned char data[10]={0};

data[0] = 0x0F;
data[1] = 0x04;
data[2] = smem.vGetHardwareStatus(3); //���oHWstat
data[3] = smem.vGetHardwareStatus(4); //���oHWstat

_MSG1 = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4, true);
_MSG1.InnerOrOutWard = cOutWard;
writeJob.WritePhysicalOut(_MSG1.packet, _MSG1.packetLength, DEVICECENTER92);
//�ǰe�w�骬�A

usleep(100000);

unsigned char data1[4]={0};

data1[0] = 0x0F;
data1[1] = 0x92;
data1[2] = 0;

_MSG2 = oDataToMessageOK.vPackageINFOTo92Protocol(data1, 3, true);
_MSG2.InnerOrOutWard = cOutWard;
writeJob.WritePhysicalOut(_MSG2.packet, _MSG2.packetLength, DEVICECENTER92);
//�ǰe�n�D���߹��

int iReturnCommandSet = smem.vGet0FCommandSet();
if (iReturnCommandSet >= 2 ) { //Advanced

usleep(100000);

unsigned char data2[8]={0};
YMDHMS ResetTime;
ResetTime = smem.vGetLastResetTime();
data2[0] = 0x0F;
data2[1] = 0x00;
data2[2] = ResetTime.Month;
data2[3] = ResetTime.Day;
data2[4] = ResetTime.Hour;
data2[5] = ResetTime.Min;

_MSG3 = oDataToMessageOK.vPackageINFOTo92Protocol(data2, 6,true);
_MSG3.InnerOrOutWard = cOutWard;
writeJob.WritePhysicalOut(_MSG3.packet, _MSG3.packetLength, DEVICECENTER92);
//�ǰe�^���W���_�q�ɶ�
}

usleep(100000);

unsigned char data3[5]={0};

data3[0] = 0x0F;
data3[1] = 0x90;
data3[2] = 0x52;
data3[3] = 0x52;

_MSG4 = oDataToMessageOK.vPackageINFOTo92Protocol(data3, 4, true);
_MSG4.InnerOrOutWard = cOutWard;
writeJob.WritePhysicalOut(_MSG4.packet, _MSG4.packetLength, DEVICECENTER92);
//�^���w�魫�m���\
}

} catch (...) {}
}
//---------------------------------------------------------------------------
bool PDEVICEKEYPAD::vSavePlan(unsigned char ucPhaseID)
{

  bool bPhaseStatus;
  unsigned char ucPhaseSubphaseCount;
  unsigned char ucPhaseSignalCount;
  unsigned char ucDir;

  bPhaseStatus = smem.vGetTCPhasePlanSegTypeData(TC_Phase, ucPhaseID);
  if(bPhaseStatus == false) { //let plan 48 be flash light
    ucPhaseID = 0xB0;
  }

  stc.Lock_to_Load_Plan_for_Panel(48);

  if(stc.Lock_to_Load_Phase_for_Panel(ucPhaseID)) {
      if(stc._panel_phase._subphase_count == stc._panel_plan._subphase_count &&
         stc._panel_phase._phase_order == stc._panel_plan._phase_order &&
         stc._panel_plan._planid == 48
          ) {
      // donothing

      } else {
        ucPhaseSubphaseCount = stc._panel_phase._subphase_count;
        ucPhaseSignalCount = stc._panel_phase._signal_count;
        stc.Lock_to_Reset_Plan_for_Panel(48, ucPhaseSubphaseCount);

        stc._panel_plan._planid = 48;
        stc._panel_plan._dir = 0x01;

        stc._panel_plan._phase_order = ucPhaseID;
        stc._panel_plan._subphase_count = ucPhaseSubphaseCount;
        stc._panel_plan._cycle_time = 25 + (ucPhaseSubphaseCount-1)*20;
        stc._panel_plan._offset = 0;


        for (int i = 0; i < ucPhaseSubphaseCount; i++) {
             stc._panel_plan._ptr_subplaninfo[i]._green = 10;
             stc._panel_plan._ptr_subplaninfo[i]._min_green = 0;
             stc._panel_plan._ptr_subplaninfo[i]._max_green = 255;
             stc._panel_plan._ptr_subplaninfo[i]._yellow = 3;
             stc._panel_plan._ptr_subplaninfo[i]._allred = 2;
             stc._panel_plan._ptr_subplaninfo[i]._pedgreen_flash = 5;
             stc._panel_plan._ptr_subplaninfo[i]._pedred = 0;
        }
        if(ucPhaseID != 0xB0) { stc._panel_plan._ptr_subplaninfo[0]._green = 15; }
        else {
          stc._panel_plan._ptr_subplaninfo[0]._yellow = 0;
          stc._panel_plan._ptr_subplaninfo[0]._allred = 0;
          stc._panel_plan._ptr_subplaninfo[0]._pedgreen_flash = 0;
        }

        stc._panel_plan._cycle_time = stc._panel_plan.calculated_cycle_time();

        stc.Lock_to_Save_Plan_from_Panel();

        smem.vSetTCPhasePlanSegTypeData(TC_Plan, 48, true);

      }
  }
}


