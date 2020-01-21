//---------------------------------------------------------------------------
#include "PTRAFFICCCJ.h"

#include "CDataToMessageOK.h"
#include "WRITEJOB.h"
#include "CTIMER.h"
#include "DIGITALIO.h"
#include "CHexToInt.h"
#include "CSTC.h"
#include "CTools.h"

#include "SCREENTFDStatus.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <error.h>

#include "SCREENLast92TCPlanSegmentUpdate.h"
#include "SCREENMain.h"
#include "SCREENCtlSetup.h"
#include "LCD240x128.h"
#include "SMEM.h"

#include "CTIMER.h"

#include "CPlanInfo.h"
#include "CPhaseInfo.h"
#include "CSegmentInfo.h"

//---------------------------------------------------------------------------
PTRAFFICCCJ::PTRAFFICCCJ(void)
{
  uc5F90LastContralCode = 0xFF;
}
//---------------------------------------------------------------------------
PTRAFFICCCJ::~PTRAFFICCCJ(void)
{
}
//---------------------------------------------------------------------------
bool PTRAFFICCCJ::DoWorkViaPTraffic(MESSAGEOK message)
{
try {

  if (message.packet[0]==0xAA) {
      if (message.packet[1]==0xBB) {
        if (message.ReadorWrite==cREAD ) {
          vReturnWriteACK92(message.packet[2]);
        }

        switch (message.packet[8]) {
          case 0x10:
            vWriteControlStrategy5F10(message);
            break;

          case 0x90:
            v5F90(message);
            break;

          case 0x40:
//                  vQurryControlStrategy5F40(message);
            break;

          case 0x1C:
              vGoToNextPhaseStepControl_5F1C(message);
            break;

          case 0x4C:
                  vQueryPhaseStepControl_5F4C(message);
            break;

          case 0x70:
              vQueryCCJSec_5F70(message);
            break;

          case 0x12:
              vCCJSetIPCTime_5F12(message);
            break;

          case 0x42:
              vCCJQueryIPCTime_5F42(message);
            break;

          case 0x9F:
              vCCJTFDActuateExtend5F9F(message);
            break;

          case 0x9A:
              vCCJTFDActuateStatusScreen5F9A(message);
            break;

          case 0x18:
              vWriteRunPlan5F18(message);
          break;

          case 0x14:
            vWritePlan5F14(message);
            break;

          case 0x44:
            vQueryPlan5F44(message);
            break;
          case 0x15:
            vWritePlanDBMS5F15(message);
            break;

          case 0x45:
            vQueryPlanDBMS5F45(message);
            break;

          case 0x91:
            vSetSendStep5F91(message);
            break;

          case 0xA2:
            vCCJTOMActuateExtendShort5FA2(message);
            break;

          case 0x48:
            vQueryCCTPlan5F48(message);
            break;

          case 0xA3:
            vCloseRedCount5FA3(message);
            break;

          case 0xA4:
            vUsingNormalTODPlan5FA4(message);
            break;

          case 0xA5:
            vActTOD5FA5(message);
            break;

          default:
            break;


        }
    } else if (message.packet[1]==0xDD) {
        if (message.ReadorWrite==cREAD) {
//                smem.ClearWaitMsg(message.packet[2]);
        } else if (message.ReadorWrite==cWRITE) {
                       writeJob.WritePhysicalOut(message.packet,message.packetLength,DEVICECCJDYNCTL);
        }
    } else if (message.packet[1]==0xEE) {
//              smem.ClearWaitMsg(message.packet[2]);
    }
  }

      return true;

    return true;

  } catch (...) {}
}

//---------------------------------------------------------------------------
bool PTRAFFICCCJ::vWriteControlStrategy5F10(MESSAGEOK DataMessageIn)
{
try{

  unsigned short int usiAmegidsDevLCN;
  DATA_Bit _ControlStrategy;
  int EffectTime;

  unsigned char data[32];
  MESSAGEOK _MsgOK;

  _ControlStrategy.DBit = DataMessageIn.packet[9];
  EffectTime = DataMessageIn.packet[10] * 60;

  if( DataMessageIn.packetLength < 14 ) { vReturnToCenterNACK(0x5F, 0x10, 0x08, 0x00); return false; }
  else if( DataMessageIn.packetLength > 14 ) { vReturnToCenterNACK(0x5F, 0x10, 0x08, DataMessageIn.packetLength - 12); return false; }

  if(smem.vGetUCData(amegidsDynEnable) > 0) {
    printf("[OT] amegidsDynEnable > 0\n");
    usiAmegidsDevLCN = smem.vGetUSIData(amegidsDynDevLCN);
    data[0] = 0xCF;
    data[1] = 0x40;
    data[2] = 0x5F;
    data[3] = 0x10;
    data[4] = DataMessageIn.packet[9];
    data[5] = DataMessageIn.packet[10];

    _MsgOK = oDataToMessageOK.vPackageINFOTo92ProtocolSetADDR(data, 6, DataMessageIn.packet[2], usiAmegidsDevLCN);
    _MsgOK.InnerOrOutWard = cOutWard;
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEAMEGIDS);

  } else {

//  _ControlStrategy.DBit = smem.vGetUCData(TC92_ucControlStrategy);              //Read Data from share memory
//  EffectTime = smem.vGetINTData(TC92_iEffectTime);
    int iReturnCommandSet = smem.vGet0FCommandSet();                              //Get Oper Level

    vReturnToCenterACK(0x5F, 0x10);

    _ControlStrategy.DBit = DataMessageIn.packet[9];
    EffectTime = DataMessageIn.packet[10] * 60;                                   //EffectTime save using Sec, but protocal using Min.

    data[0] = 0x5F;
    data[1] = 0x91;
    data[2] = DataMessageIn.packet[9];
    data[3] = DataMessageIn.packet[10];

    smem.vSetUCData(TC92_ucControlStrategy, _ControlStrategy.DBit);
    smem.vSetINTData(TC92_iEffectTime, EffectTime);

    smem.vSet5F18EffectTime(EffectTime);  //OT20110517
    smem.vSetINTData(TC92_iEffectTime, EffectTime);  //OT20110517

  //then save!?

/* ��������Q����
  if(_ControlStrategy.switchBit.b2 == true) {
    stc.Lock_to_Set_Control_Strategy(STRATEGY_CADC);
    return true;
  }
  if(_ControlStrategy.switchBit.b1 == true) {
    stc.Lock_to_Set_Control_Strategy(STRATEGY_TOD);
    return true;
  }
*/

//For disable Dynamic

//OT20111128
    // if(MACHINELOCATE == MACHINELOCATEATYULIN)                                     // For CCJ
    if(smem.vGetUCData(TC_CCT_MachineLocation) == MACHINELOCATEATYULIN)
    {

      _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4, true);
      _MsgOK.InnerOrOutWard = cOutWard;
      writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);

    } else {

      if(_ControlStrategy.switchBit.b1 == true &&
         _ControlStrategy.switchBit.b5 == false &&
         _ControlStrategy.switchBit.b6 == false    )
      {
        if( stc.Lock_to_LoadControlStrategy() == STRATEGY_ALLDYNAMIC )
        {
          _intervalTimer.vAllDynamicToTODCount(10);
  //      _intervalTimer.vAllDynamicToTODCount(EffectTime);
        }
        smem.vSet5F18EffectTime(2);          //OT20110517
      }

      else if(_ControlStrategy.DBit == 0x30)
      {
        printf("Get Amegids 0F 10 30\n\n");
        if( stc.Lock_to_LoadControlStrategy() == STRATEGY_TOD ) {
          printf("Get Amegids, now is TOD\n\n");
          stc.Lock_to_Set_Control_Strategy(STRATEGY_ALLDYNAMIC);
          printf("Get Amegids, now changing to ALLDYNAMIC\n\n");

          int usiCurrentSubphaseStep = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);
          if(usiCurrentSubphaseStep == 0) {
            if(Protocal5F1CStopStep == usiCurrentSubphaseStep) {
              _intervalTimer.vAllDynamicToTODCount(EffectTime);
            }
            else {
              _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_green_time));
            }
          }
          else if(usiCurrentSubphaseStep == 1)
            _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_pedgreenflash_time));
          else if(usiCurrentSubphaseStep == 2)
            if(Protocal5F1CStopStep == usiCurrentSubphaseStep) {
              _intervalTimer.vAllDynamicToTODCount(EffectTime);
            }
            else {
              _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_pedred_time));
            }
          else if(usiCurrentSubphaseStep == 3)
            _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_yellow_time));
          else if(usiCurrentSubphaseStep == 4)
            _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_allred_time));

        }
      }
    }

  }

  return true;

} catch(...) {}
}


//---------------------------------------------------------------------------
bool PTRAFFICCCJ::v5F90(MESSAGEOK DataMessageIn)
{
try{
  unsigned char  ucContralCode;
  int smemEffectTime, commandEffectTime;
  DATA_Bit _ControlStrategy;
  unsigned char data[16];
  unsigned char uc5F18Type = 1;  //1:one time, 2:one seg
  MESSAGEOK _MsgOK;

  _ControlStrategy.DBit = smem.vGetUCData(TC92_ucControlStrategy);              //Read Data from share memory
  smemEffectTime = smem.vGetINTData(TC92_iEffectTime);
  if(smemEffectTime == 0) smemEffectTime = 66;                                  //protect
  int iReturnCommandSet = smem.vGet0FCommandSet();                              //Get Oper Level

  unsigned char ucCurrentSubphaseStep = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step)  + 1;

/*  if(iReturnCommandSet <= 1) { vReturnToCenterNACK(0x5F, 0x10, 0x80, 0x00); return false; } // not include level "A""
  if( DataMessageIn.packetLength < 14 ) { vReturnToCenterNACK(0x5F, 0x10, 0x08, 0x00); return false; }
  else if( DataMessageIn.packetLength > 14 ) { vReturnToCenterNACK(0x5F, 0x10, 0x08, DataMessageIn.packetLength - 12); return false; }
*/
/*  disable because DongDa XiBin 5f9011 5f9012
vReturnToCenterACK(0x5F, 0x90);
*/

  ucContralCode = DataMessageIn.packet[9];

//OT20110921
  uc5F18Type = DataMessageIn.packet[10];
//  commandEffectTime = DataMessageIn.packet[10] * 60;
//  if(commandEffectTime != smemEffectTime) { smem.vSetINTData(TC92_iEffectTime, commandEffectTime); }


  //then save!?

/*
  if(_ControlStrategy.switchBit.b2 == true) {
    stc.Lock_to_Set_Control_Strategy(STRATEGY_CADC);
    return true;
  }
  if(_ControlStrategy.switchBit.b1 == true) {
    stc.Lock_to_Set_Control_Strategy(STRATEGY_TOD);
    return true;
  }
*/

//For disable Dynamic

  printf("[OTMsg 5F90] current ControlStrategy:%x, ucCurrentSubphaseStep:%d\n", _ControlStrategy.DBit, ucCurrentSubphaseStep);

  if(ucContralCode == 0x11) {      //v2 Dyn, using 5F18
    printf("Get CCJ 0x5F 0x90 0x11.\n");
    _ControlStrategy.DBit = smem.vGetUCData(TC92_ucControlStrategy);
    _ControlStrategy.switchBit.b4 = true;

    //OT20110729
    _ControlStrategy.switchBit.b2 = true;

    smem.vSetUCData(TC92_ucControlStrategy, _ControlStrategy.DBit);

    //OT20110921
    if(uc5F18Type == 0x01) {
      smem.vSetBOOLData(TC92_PlanOneTime5F18_Change, true);
     //force close 5F18
      if(smem.vGet5F18EffectTime() > 0) {
        smem.vSet5F18EffectTime(2);
      }
    } else if(uc5F18Type == 0x02) {
    }

    data[0] = 0x0F;
    data[1] = 0x80;
    data[2] = 0x5F;
    data[3] = 0x90;
    data[4] = 0x11;
    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 5, true);
    _MsgOK.InnerOrOutWard = cOutWard;
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);
  } else if(ucContralCode == 0x12) {  // stop CCJ control.
    printf("Get CCJ 0x5F 0x90 0x12.\n");
    _ControlStrategy.DBit = smem.vGetUCData(TC92_ucControlStrategy);
    _ControlStrategy.switchBit.b4 = false;

    //OT20110729
    _ControlStrategy.switchBit.b2 = false;

    smem.vSetUCData(TC92_ucControlStrategy, _ControlStrategy.DBit);
    smem.vSetBOOLData(TC92_PlanOneTime5F18_Change, false);

    //OT20110921, force close 5F18
    if(smem.vGet5F18EffectTime() > 0) {
      smem.vSet5F18EffectTime(2);
    }


//OT990327, mark for one time plan    smem.vSetBOOLData(TC92_PlanOneTime5F18_Change, true);

    data[0] = 0x0F;
    data[1] = 0x80;
    data[2] = 0x5F;
    data[3] = 0x90;
    data[4] = 0x12;
    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 5, true);
    _MsgOK.InnerOrOutWard = cOutWard;
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);
  } else if (ucContralCode == 0x13) {  //OT20131121, means heart Beat
    data[0] = 0x5F;
    data[1] = 0x90;
    data[2] = 0x13;
    data[3] = smem.vGetUCData(TC_CSTC_ControlStrategy);
    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4, true);
    _MsgOK.InnerOrOutWard = cOutWard;
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);

//OT20131225
    smem.vAddCCJHeartBeatCount(1);
  }

  if(uc5F90LastContralCode != ucContralCode) {                                  // contralCode change

    if(ucCurrentSubphaseStep == 1) {                                              // only when green can be controled
      printf("[OTMsg 5F90] ucCurrentSubphaseStep == 1\n");

      if(ucContralCode == 0x00)                                                     // stop
      {
        printf("[OTMsg 5F90] ucContralCode == 0x00\n");
        if( stc.Lock_to_LoadControlStrategy() == STRATEGY_ALLDYNAMIC ) {
          _intervalTimer.vAllDynamicToTODCount(20);
//      _intervalTimer.vAllDynamicToTODCount(EffectTime);
        }

      }

      else if(ucContralCode == 0x01)                                                // start
      {
        if( stc.Lock_to_LoadControlStrategy() == STRATEGY_TOD ) {
          stc.Lock_to_Set_Control_Strategy(STRATEGY_ALLDYNAMIC);

          _ControlStrategy.DBit = smem.vGetUCData(TC92_ucControlStrategy);
          _ControlStrategy.switchBit.b1 = false;
          _ControlStrategy.switchBit.b5 = true;

          smem.vSetUCData(TC92_ucControlStrategy, _ControlStrategy.DBit);

          int usiCurrentSubphaseStep = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);
          if(usiCurrentSubphaseStep == 0) {
            if(Protocal5F1CStopStep == usiCurrentSubphaseStep) {
              _intervalTimer.vAllDynamicToTODCount(commandEffectTime);
            }
            else {
              _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_green_time));
            }
          }
          else if(usiCurrentSubphaseStep == 1)
            _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_pedgreenflash_time));
          else if(usiCurrentSubphaseStep == 2)
            if(Protocal5F1CStopStep == usiCurrentSubphaseStep) {
              _intervalTimer.vAllDynamicToTODCount(commandEffectTime);
            }
            else {
              _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_pedred_time));
            }
          else if(usiCurrentSubphaseStep == 3)
            _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_yellow_time));
          else if(usiCurrentSubphaseStep == 4)
            _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_allred_time));
        }
      }
    uc5F90LastContralCode = ucContralCode;
    }

  }


  return true;

} catch(...) {}
}

//-------------------------------------------------------------------
bool PTRAFFICCCJ::vGoToNextPhaseStepControl_5F1C(MESSAGEOK DataMessageIn)
{
try {

    printf("[Get] CCJ 5F1C\n");

    //BT9512260001 START
    char msg[256];
    memset(msg,0,sizeof(msg));
    //BT9512260001 END

    short int siSubPhaseID, siStepID, siEffectTime;
    siSubPhaseID = DataMessageIn.packet[9];
    siStepID = DataMessageIn.packet[10];
    siEffectTime = DataMessageIn.packet[11];

    unsigned short int usiOriSubphaseStep = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);
    unsigned short int usiCurrentSubphaseStep;

/*
    int iReturnCommandSet = smem.vGet0FCommandSet();
    if (iReturnCommandSet <= 2 ) { vReturnToCenterNACK(0x5F, 0x1C, 0x80, 0x00); return false; }

    if ( DataMessageIn.packetLength < 15 ) { vReturnToCenterNACK(0x5F, 0x1C, 0x08, 0x00); return false; }
    else if ( DataMessageIn.packetLength > 15 ) { vReturnToCenterNACK(0x5F, 0x1C, 0x08, DataMessageIn.packetLength - 15); return false; }

    if(siSubPhaseID > 8) { vReturnToCenterNACK(0x5F, 0x1C, 0x04, 0x01); return false; }
    if(siStepID > 8) { vReturnToCenterNACK(0x5F, 0x1C, 0x04, 0x02); return false; }
*/

    DATA_Bit uc5F10_ControlStrategy;
    int i5F10_EffectTime;

    uc5F10_ControlStrategy.DBit = smem.vGetUCData(TC92_ucControlStrategy);
    i5F10_EffectTime = smem.vGetINTData(TC92_iEffectTime);

    if(uc5F10_ControlStrategy.switchBit.b5 == false)
      { vReturnToCenterNACK(0x5F, 0x1C, 0x80, 0x00); return false; }

//      CSTC_exec_phase_current_subphase

    if(siSubPhaseID == 0 && siStepID == 0 && siEffectTime == 0) {  //Go To Next Step
//      stc.Lock_to_Set_Control_Strategy(STRATEGY_MANUAL);
//      stc.Lock_to_Set_Control_Strategy(STRATEGY_TOD);

    if (!stc.Lock_to_Set_Control_Strategy(STRATEGY_ALLDYNAMIC))
      stc.Lock_to_Set_Next_Step();

    usleep(50);
    usiCurrentSubphaseStep = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);
    if(usiCurrentSubphaseStep == 0) {
      if(Protocal5F1CStopStep == usiCurrentSubphaseStep) {
        _intervalTimer.vAllDynamicToTODCount(i5F10_EffectTime);
      }
      else {
        _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_green_time));
      }
    }
    else if(usiCurrentSubphaseStep == 1)
      _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_pedgreenflash_time));
    else if(usiCurrentSubphaseStep == 2) {
      if(Protocal5F1CStopStep == usiCurrentSubphaseStep) {
        _intervalTimer.vAllDynamicToTODCount(i5F10_EffectTime);
      }
      else {
        _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_pedred_time));
      }
    }
    else if(usiCurrentSubphaseStep == 3)
      _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_yellow_time));
    else if(usiCurrentSubphaseStep == 4)
      _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_allred_time));

/*
      stc.vLockToSetControlStrategyToManualNotBySignal();
      usleep(50);
      stc.vLockToSetControlStrategyToTODNotBySignal();
*/
//      return true;
    }
    else {

    /* check next phase */

    bool bRetCmp = stc.vCompareNextSubphase(siSubPhaseID);

    if(bRetCmp) {

      if (!stc.Lock_to_Set_Control_Strategy(STRATEGY_ALLDYNAMIC)){
        //OT Debug Signal 951116
        smem.vSetBOOLData(TC_SIGNAL_NEXT_STEP_OK, false);
        // replace stc.Lock_to_Set_Next_Step();
        stc.vSetNextStepNotUseSignal();
      }//if (!stc.Lock_to_Set_Control_Strategy(STRATEGY_ALLDYNAMIC))

    printf("!!![OTMsg]!!! sync CSTC START\n");

    //usleep(1000);
    //usleep(50);
    //OT Debug Signal 951116
    for(int i = 0; i < 2000; i++) {
      if(smem.vGetBOOLData(TC_SIGNAL_NEXT_STEP_OK) == true) {
        smem.vSetBOOLData(TC_SIGNAL_NEXT_STEP_OK, false);
        printf("!!![OTMsg]!!! sync CSTC using %d count ( 500ms per)\n", i);
        //i = 100;100,000
        i=2000;
        }//if(smem.vGetBOOLData(TC_SIGNAL_NEXT_STEP_OK) == true)
      printf("!!![OTMsg]!!! sync CSTC Already finished:%d\n", i);
      usleep(50);
    }// for(int i = 0; i < 100; i++) {

    usiCurrentSubphaseStep = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);

    sprintf(msg,"PTRAFFICCCJ OriStep = %d CurrentStep = %d", usiOriSubphaseStep, usiCurrentSubphaseStep);
    //smem.vWriteMsgToDOM(msg);
    memset(msg,0,sizeof(msg));

    if(usiCurrentSubphaseStep == 0) {
      if(Protocal5F1CStopStep == usiCurrentSubphaseStep) {
        _intervalTimer.vAllDynamicToTODCount(i5F10_EffectTime);
        sprintf(msg,"PTRAFFICCCJ usiCurrentSubphaseStepN = 0 t=%d",i5F10_EffectTime);
      }//if(Protocal5F1CStopStep == usiCurrentSubphaseStep)
      else {
        _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_green_time));
        sprintf(msg,"PTRAFFICCCJ usiCurrentSubphaseStepN = 0 t=%d",stc.vGetUSIData(CSTC_exec_plan_green_time));
      }//if(Protocal5F1CStopStep == usiCurrentSubphaseStep)
    }//if(usiCurrentSubphaseStep == 0)
    else if(usiCurrentSubphaseStep == 1){
      _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_pedgreenflash_time));
      sprintf(msg,"PTRAFFICCCJ usiCurrentSubphaseStepN = 1 t=%d",stc.vGetUSIData(CSTC_exec_plan_pedgreenflash_time));
    }//else if(usiCurrentSubphaseStep == 1)
    else if(usiCurrentSubphaseStep == 2) {
      if(Protocal5F1CStopStep == usiCurrentSubphaseStep) {
        _intervalTimer.vAllDynamicToTODCount(i5F10_EffectTime);
        sprintf(msg,"PTRAFFICCCJ usiCurrentSubphaseStepN = 2 t=%d",i5F10_EffectTime);
      }//if(Protocal5F1CStopStep == usiCurrentSubphaseStep)
      else {
        _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_pedred_time));
        sprintf(msg,"PTRAFFICCCJ usiCurrentSubphaseStepN = 2 t=%d",stc.vGetUSIData(CSTC_exec_plan_pedred_time));
      }//if(Protocal5F1CStopStep == usiCurrentSubphaseStep)
    }//else if(usiCurrentSubphaseStep == 2)
    else if(usiCurrentSubphaseStep == 3){
      _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_yellow_time));
      sprintf(msg,"PTRAFFICCCJ usiCurrentSubphaseStepN = 3 t=%d",stc.vGetUSIData(CSTC_exec_plan_yellow_time));
    }//else if(usiCurrentSubphaseStep == 3)
    else if(usiCurrentSubphaseStep == 4){
      _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_allred_time));
      sprintf(msg,"PTRAFFICCCJ usiCurrentSubphaseStepN = 4 t=%d",stc.vGetUSIData(CSTC_exec_plan_allred_time));
    }//else if(usiCurrentSubphaseStep == 4)
    else{
              sprintf(msg,"PTRAFFICCCJ usiCurrentSubphaseStepN = %d",usiCurrentSubphaseStep);
    }//else
    }

    }



    vCCJPhaseStep_5F7C();

 //BT9512260001 START
 //smem.vWriteMsgToDOM(msg);
 //BT9512260001 END

//    unsigned short int usiCSTC_RunningPhaseCurrentPhase = stc.vGetUSIData(CSTC_exec_plan_phase_order);  //Should +1
//    stc.Lock_to_Load_Phase_for_Center(usiCSTC_RunningPhaseCurrentPhase);

    return true;

} catch(...) {}
}

//---------------------------------------------------------------------------
bool PTRAFFICCCJ::vQueryPhaseStepControl_5F4C(MESSAGEOK DataMessageIn)
{
try{

//For Amegids of NCKU, not Ver3 Protocal
//  unsigned short int usiEffectTime = _intervalTimer.vGetEffectTime();
  unsigned short int usiStepSec = stc.vGetStepTime();

  unsigned char data[6];
  data[0] = 0x5F;
  data[1] = 0xCC;
  data[2] = smem.vGetUCData(TC92_ucControlStrategy);
  data[3] = stc.vGetUSIData(CSTC_exec_phase_current_subphase) + 1;
  data[4] = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step)  + 1;

//  stc.vGetStepTime();

//  if( stc.Lock_to_LoadControlStrategy == STRATEGY_ALLDYNAMIC )

//For Amegids of NCKU, not Ver3 Protocal
//    data[5] = (usiEffectTime>>8) & 0x00ff;
//    data[6] = usiEffectTime & 0x00ff;
    data[5] = (usiStepSec>>8) & 0x00ff;
    data[6] = usiStepSec & 0x00ff;

//  else
//    data[4] = 0;

  MESSAGEOK _MsgOK;
  _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 7, true);
  _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
  writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);
  return true;

} catch(...) {}
}


//---------------------------------------------------------------------------
bool PTRAFFICCCJ::vQueryCCJSec_5F70(MESSAGEOK DataMessageIn)
{
try{

//For Amegids of NCKU, not Ver3 Protocal
//  unsigned short int usiEffectTime = _intervalTimer.vGetEffectTime();
  unsigned short int usiStepSec = stc.vGetStepTimeUntilNextGreen();
  unsigned short int usiCycleSec = stc.vGetStepTimeUntilNextCycle();
  printf("[OTMsg] usiCycleSec:%d\n", usiCycleSec);

  unsigned char data[9];
  data[0] = 0x5F;
  data[1] = 0xF0;
  data[2] = smem.vGetUCData(TC92_ucControlStrategy);
  data[3] = stc.vGetUSIData(CSTC_exec_phase_current_subphase) + 1;
  data[4] = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step)  + 1;

//  stc.vGetStepTime();

//  if( stc.Lock_to_LoadControlStrategy == STRATEGY_ALLDYNAMIC )

//For Amegids of NCKU, not Ver3 Protocal
//    data[5] = (usiEffectTime>>8) & 0x00ff;
//    data[6] = usiEffectTime & 0x00ff;
    data[5] = (usiStepSec>>8) & 0x00ff;
    data[6] = usiStepSec & 0x00ff;
    data[7] = (usiCycleSec>>8) & 0x00ff;
    data[8] = usiCycleSec & 0x00ff;

//  else
//    data[4] = 0;

  MESSAGEOK _MsgOK;
  _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 9, true);
  _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
  writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);
  return true;

} catch(...) {}
}

//----------------------------------------------------------
bool PTRAFFICCCJ::vCCJSetIPCTime_5F12(MESSAGEOK DataMessageIn)
{
try{
  //���e������
    int SecDif;

    fprintf(stderr, "%s[MeSsAgE] Receiver 5F12, Lengths is %d.%s\n", ColorRed, DataMessageIn.packetLength, ColorNormal);

    if( DataMessageIn.packetLength < 19 ) { vReturnToCenterNACK(0x5F, 0x12, 0x08, 0x00); return false; }
    if( DataMessageIn.packetLength > 19 ) { vReturnToCenterNACK(0x5F, 0x12, 0x08, DataMessageIn.packetLength - 12); return false; }
    if( DataMessageIn.packet[9] < 59 ) { vReturnToCenterNACK(0x5F, 0x12, 0x04, 0x01); return false; }
    if( DataMessageIn.packet[10] > 12 || DataMessageIn.packet[10] == 0 ) { vReturnToCenterNACK(0x5F, 0x12, 0x04, 0x02); return false; }
    if( DataMessageIn.packet[11] > 31 || DataMessageIn.packet[11] == 0 ) { vReturnToCenterNACK(0x5F, 0x12, 0x04, 0x03); return false; }
    if( DataMessageIn.packet[13] > 23 ) { vReturnToCenterNACK(0x5F, 0x12, 0x04, 0x04); return false; }
    if( DataMessageIn.packet[14] > 60 ) { vReturnToCenterNACK(0x5F, 0x12, 0x04, 0x05); return false; }
    if( DataMessageIn.packet[15] > 60 ) { vReturnToCenterNACK(0x5F, 0x12, 0x04, 0x06); return false; }

//  else
//  {
    unsigned int YY=DataMessageIn.packet[9],MM=DataMessageIn.packet[10],DD=DataMessageIn.packet[11],
                 hh=DataMessageIn.packet[13],mm=DataMessageIn.packet[14],ss=DataMessageIn.packet[15];
    unsigned int YYYY=YY+1911;

    struct tm _CenterTime;                                                        //����SecDif�Ϊ�
    time_t _CenterTimeSec;
    unsigned int tmpYY = YY+11;
    unsigned int tmpMM = MM - 1;

    _CenterTime.tm_year = tmpYY;  // to AD
    _CenterTime.tm_mon = tmpMM;
    _CenterTime.tm_mday = DD;
    _CenterTime.tm_hour = hh;
    _CenterTime.tm_min = mm;
    _CenterTime.tm_sec = ss;
    _CenterTime.tm_isdst = -1;
    _CenterTimeSec = mktime(&_CenterTime);

    //�o��{�b�ɶ�,���F�]date -s MMDDhhmmYYYY
    time_t currenttime=time(NULL);

    if(_CenterTimeSec >= currenttime)                                             //�DSecDif
       SecDif = _CenterTimeSec - currenttime;
    else
       SecDif = currenttime - _CenterTimeSec;

    printf("[Message] SecDif is %d\n", SecDif);


    vReturnToCenterACK(0x5F, 0x12);

    struct tm *now=localtime(&currenttime);
    unsigned int temphh=(now->tm_hour),tempmm=(now->tm_min);

    char date[22]={0},time[17]={0};

    stc.TimersRead_BeforeResetCMOSTime();  //OTBUG =1
    _intervalTimer.TimersRead_BeforeResetCMOSTime();

    smem.vSetTimerMutexRESET(1);
    while(smem.vGetTimerMutexCTIMER() == 0 || smem.vGetTimerMutexCSTC() == 0) {
      usleep(100);
    }

//    stc.TimersRead_BeforeResetCMOSTime();  //OTBUG =1
//    _intervalTimer.TimersRead_BeforeResetCMOSTime();

    smem.vSetSystemClockTime(YYYY, MM, DD, hh, mm, ss);
/*move to smem
    sprintf(date,"date -s %#02d%#02d%#02d%#02d%#04d",MM,DD,temphh,tempmm,YYYY);

  //OTMARKPRINTF  printf("\n%s\n",date);

    system(date);

    sprintf(time,"date -s %#02d:%#02d:%#02d",hh,mm,ss);

  //OTMARKPRINTF  printf("\n%s\n",time);

    system(time);
*/

    stc.TimersReset_AfterResetCMOSTime();  //OTBUG =1
    _intervalTimer.TimersReset_AfterResetCMOSTime();

//move to smem    system("hwclock -w");

//    smem.vSetTimerMutexCTIMER(false);
//    smem.vSetTimerMutexCSTC(false);
    smem.vSetTimerMutexRESET(0);

    smem.vSendTimerUpdateToCCJ_5F9E();

    system("hwclock -w");

//    }
  } catch (...) {}
}

//----------------------------------------------------------
bool PTRAFFICCCJ::vCCJQueryIPCTime_5F42(MESSAGEOK DataMessageIn)
{
try{

  time_t currenttime=time(NULL);
  struct tm *now=localtime(&currenttime);
  unsigned int temphh=(now->tm_hour),tempmm=(now->tm_min);
  unsigned char data[9];

  data[0] = 0x0F;
  data[1] = 0xC2;
  data[2] = (now->tm_year) - 11;
  data[3] = (now->tm_mon) + 1;
  data[4] = (now->tm_mday);
  data[5] = (now->tm_wday);
  data[6] = (now->tm_hour);
  data[7] = (now->tm_min);
  data[8] = (now->tm_sec);

  MESSAGEOK _MsgOK;
  _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 9,true);
  _MsgOK.InnerOrOutWard = cOutWard;
//    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
  writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);

  } catch (...) {}
}



//---------------------------------------------------------------------------
bool PTRAFFICCCJ::vReturnWriteACK92(BYTE sequence)
{    //AA DD SEQ LCN LCN LEN LEN CKS
try {
    MESSAGEOK messageOut;
    messageOut.protocol=PROTOCOL92;
    messageOut.packetLcn=smem.GetAddress();
    messageOut.packetLength=8;
    messageOut.cksStatus=true;
    messageOut.success=true;
    messageOut.ReadorWrite=cWRITE;

    messageOut.packet[0]=0xAA;
    messageOut.packet[1]=0xDD;
    messageOut.packet[2]=sequence;

    if (smem.vGetAdjudicateReturnAddressBCDorHEX() == cBCD) {                     //Select BCD mode
      messageOut.packet[3]=oHexToInt.vIntToFuckingBCD(1, messageOut.packetLcn);
      messageOut.packet[4]=oHexToInt.vIntToFuckingBCD(0, messageOut.packetLcn);
    }
    else {                                                                      //Normal(HEX) mode
      messageOut.packet[3]=HI(messageOut.packetLcn);
      messageOut.packet[4]=LO(messageOut.packetLcn);
    }

    messageOut.packet[5]=HI(messageOut.packetLength);
    messageOut.packet[6]=LO(messageOut.packetLength);
    messageOut.packet[7]=0;
    for (int i=0;i<7;i++) messageOut.packet[7]^=messageOut.packet[i];

    writeJob.WritePhysicalOut(messageOut.packet, messageOut.packetLength, DEVICECCJDYNCTL);
    return true;
  } catch (...) {}
}
//---------------------------------------------------------------------------
bool PTRAFFICCCJ::vReturnWriteNACK92(BYTE sequence,BYTE error)
{    //AA EE SEQ LCN LCN LEN LEN ERR CKS
try {
    MESSAGEOK messageOut;
    messageOut.protocol=PROTOCOL92;
    messageOut.packetLcn=smem.GetAddress();
    messageOut.packetLength=9;
    messageOut.cksStatus=true;
    messageOut.success=true;
    messageOut.ReadorWrite=cWRITE;

    messageOut.packet[0]=0xAA;
    messageOut.packet[1]=0xEE;
    messageOut.packet[2]=sequence;

    if (smem.vGetAdjudicateReturnAddressBCDorHEX() == cBCD) {                     //Select BCD mode
      messageOut.packet[3]=oHexToInt.vIntToFuckingBCD(1, messageOut.packetLcn);
      messageOut.packet[4]=oHexToInt.vIntToFuckingBCD(0, messageOut.packetLcn);
    }
    else {                                                                      //Normal(HEX) mode
      messageOut.packet[3]=HI(messageOut.packetLcn);
      messageOut.packet[4]=LO(messageOut.packetLcn);
    }

    messageOut.packet[5]=HI(messageOut.packetLength);
    messageOut.packet[6]=LO(messageOut.packetLength);
    messageOut.packet[7]=error;
    messageOut.packet[8]=0;
    for (int i=0;i<8;i++) messageOut.packet[8]^=messageOut.packet[i];

    writeJob.WritePhysicalOut(messageOut.packet, messageOut.packetLength, DEVICECCJDYNCTL);
    return true;
  } catch (...) {}
}

//----------------------------------------------------------
bool PTRAFFICCCJ::vCCJPhaseStep_5F7C(void)
{
try {
  unsigned char data[5];
  MESSAGEOK _MsgOK;

  data[0] = 0x5F;
  data[1] = 0x7C;
  data[2] = smem.vGetUCData(TC92_ucControlStrategy);
  data[3] = stc.vGetUSIData(CSTC_exec_phase_current_subphase)+1;
  data[4] = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step)+1;

  _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 5, false);
  _MsgOK.InnerOrOutWard = cOutWard;
  writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);

  printf("[OTMsg~~~~~~~~~~~~~~~~~~~2] send 5F7C!!!\n");

  return true;

}catch(...){}
}

//----------------------------------------------------------
bool PTRAFFICCCJ::vCCJTFDActuateExtend5F9F(MESSAGEOK DataMessageIn)
{
try {
  unsigned char ucTCControlStrategy;
  ucTCControlStrategy = smem.vGetUCData(TC92_ucControlStrategy);

  if(ucTCControlStrategy == 0x01 &&  smem.vGetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable) == true ) {  //0x01 = TOD
    switch(DataMessageIn.packet[10]) {
      case(0):                                                                  //Close TFD Actuate
        smem.vSetBOOLData(TC_Actuate_By_TFD, false);

        if(DataMessageIn.packet[11] > 0) {
          stc.vChangeTODCurrentStepSec(2, 0);
        }

        vReturnToCenterACK(0x5F, 0x9F);

      break;

      case(1):                                                                  //Start TFD Actuate
        smem.vSetBOOLData(TC_Actuate_By_TFD, true);
        if(smem.vGetBOOLData(TC_CCTActuate_TOD_Running) == false) {
          stc.vChangeTODCurrentStepSec(2, 0);
          printf("Enter TC_Actuate_By_TFD OK!\n");
        }
        vReturnToCenterACK(0x5F, 0x9F);
      break;

      case(2):
        stc.vActuateExtendTODTime(DataMessageIn.packet[9], DataMessageIn.packet[11]);  //Extend TFD Actuate Time
        vReturnToCenterACK(0x5F, 0x9F);
      break;

      default:
      break;
    }
  }

}catch(...){}
}

//----------------------------------------------------------
bool PTRAFFICCCJ::vCCJTOMActuateExtendShort5FA2(MESSAGEOK DataMessageIn)
{
try {
  unsigned char ucTCControlStrategy;
  ucTCControlStrategy = smem.vGetUCData(TC92_ucControlStrategy);

  printf("get 5fa2, ucTCControlStrategy=%x, TC_CCT_In_LongTanu_ActuateType_FunctionEnable:%d\n", ucTCControlStrategy, smem.vGetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable));

  if(ucTCControlStrategy == 0x01 &&  smem.vGetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable) == true ) {  //0x01 = TOD
    switch(DataMessageIn.packet[10]) {
      case(1):
        smem.vSetBOOLData(NextStepForceEnable, true);
        stc.vSetNextStepNotUseSignal();
        vReturnToCenterACK(0x5F, 0xA2);

        if(smem.vGetUCData(DynShowRedCountForAct) == 1) {
          // stc.CalculateAndSendRedCountInDyn(0);
        }

      break;

      case(2):                                                                  //Start TFD Actuate
        stc.vActuateChangeTODTimeForCCJTOMNanLu(DataMessageIn.packet[9], DataMessageIn.packet[11]);  //Extend TFD Actuate Time
        vReturnToCenterACK(0x5F, 0xA2);

        if(smem.vGetUCData(DynShowRedCountForAct) == 1) {
          // stc.CalculateAndSendRedCountInDyn(0);
        }

      break;

      default:
      break;
    }
  }

}catch(...){}
}

//----------------------------------------------------------
bool PTRAFFICCCJ::vCCJTFDActuateStatusScreen5F9A(MESSAGEOK DataMessageIn)
{
try {
  bool bDIO;
  unsigned char ucStatusCode;
  unsigned char ucVDZoneStatus;
  unsigned char ucNumVD;

  if(DataMessageIn.packet[9] == 0)
    bDIO = false;
  else {
    bDIO = true;
  }

  ucStatusCode = DataMessageIn.packet[10];

  ucNumVD = DataMessageIn.packet[11];

  ucVDZoneStatus = 0;
  for(int i = 0; i < ucNumVD; i++) {
    if(DataMessageIn.packet[14 + i*3] > ucVDZoneStatus) {
      ucVDZoneStatus = DataMessageIn.packet[14 + i*3];
    }
  }

  screenTFDStatus.vChangeScreenTFDStatus(bDIO, ucStatusCode, ucNumVD);

}catch(...){}
}

//---------------------------------------------------------------------------
bool PTRAFFICCCJ::vWriteRunPlan5F18(MESSAGEOK DataMessageIn)
{
try{
  DATA_Bit _ControlStrategy;
  unsigned char data[16];
  unsigned int uiTimeOutPlan;
  MESSAGEOK _MsgOK;

  int iSetPlanID = DataMessageIn.packet[9];

  if( DataMessageIn.packetLength < 13) { vReturnToCenterNACK(0x5F, 0x18, 0x08, 0x00); return false; }
  else if( DataMessageIn.packetLength > 13) { vReturnToCenterNACK(0x5F, 0x18, 0x08, DataMessageIn.packetLength - 12); return false; }
//don't use normal v3 protocal  vReturnToCenterACK(0x5F, 0x18);

  _ControlStrategy.DBit = smem.vGetUCData(TC92_ucControlStrategy);

//OT20110729  if(_ControlStrategy.DBit == 0x02) {  //OT20110518
 if((_ControlStrategy.DBit & 0x02) > 0 ) {  //OT20110518
    smem.vSetINTData(TC92_PlanOneTime5F18_PlanID, iSetPlanID);
    smem.vSetBOOLData(TC92_PlanOneTime5F18_Change, true);

//bug, move out    smem.vSetThisCycleRunCCJPlan5F18(true);  //990402

//OT20110610    iSetPlanID = smem.vGetINTData(TC92_PlanOneTime5F18_PlanID);
//OT1000218
    uiTimeOutPlan = stc.vDetermineTimeToNextPlan();
    smem.vSet5F18EffectTime(uiTimeOutPlan);

    data[0] = 0x0F;
    data[1] = 0x80;
    data[2] = 0x5F;
    data[3] = 0x18;
    data[4] = iSetPlanID;
    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 5, true);
    _MsgOK.InnerOrOutWard = cOutWard;
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);

    //vChangeTODCurrentStepSec will check phase.
//OT20110610    stc.vChangeTODCurrentStepSec(1, 0);

  } else if( _ControlStrategy.switchBit.b4 == false ) {

    iSetPlanID = smem.vGetINTData(TC92_PlanOneTime5F18_PlanID);

    data[0] = 0x0F;
    data[1] = 0x81;
    data[2] = 0x5F;
    data[3] = 0x18;
    data[4] = iSetPlanID;
    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 5, true);
    _MsgOK.InnerOrOutWard = cOutWard;
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);

    //return NAK
  }


} catch (...) {}
}

//---------------------------------------------------------------------------
bool PTRAFFICCCJ::vWritePlan5F14(MESSAGEOK DataMessageIn)
{
try{

  if( DataMessageIn.packetLength < 14+(7*DataMessageIn.packet[10]) ) { vReturnToCenterNACK(0x5F, 0x14, 0x08, 0x00); return false; }
  else if( DataMessageIn.packetLength > 14+(7*DataMessageIn.packet[10]) ) { vReturnToCenterNACK(0x5F, 0x14, 0x08, DataMessageIn.packetLength - 12); return false; }

    screenLast92TCPlanSegmentUpdate.DisplayPlanUpdate();

/*
PlanID(�ɨ�p��):[9]
SubPhaseCount(���ۼ�):[10]

MinGreen(�u��):[11]
MaxGreen(����):[12],[13]
Yellow(��):[14]
AllRed(����):[15]
PedGreenFlash(��H��{):[16]
PedRed(��H��):[17]
*/

  int i=0;
  bool bMollacOK=false;

//OT Debug 0523
  if (DataMessageIn.packet[10]==0)                    //�{���ɬ�
    bMollacOK=stc.Lock_to_Reset_Plan_for_Center(DataMessageIn.packet[9],1);
  else  bMollacOK=stc.Lock_to_Reset_Plan_for_Center(DataMessageIn.packet[9],DataMessageIn.packet[10]);

  if (bMollacOK) {

    stc._for_center_plan._planid=DataMessageIn.packet[9];
    stc._for_center_plan._subphase_count=DataMessageIn.packet[10];

  //OTMARKPRINTF  printf("Plan:%d, PhaseCount:%d\n",stc._for_center_plan._planid,stc._for_center_plan._subphase_count);

    if (stc._for_center_plan._subphase_count==0) {   //�{���ɬ�
        stc._for_center_plan._subphase_count=1;      //�����@��Phase

        stc._for_center_plan._ptr_subplaninfo[0]._min_green=0;
        stc._for_center_plan._ptr_subplaninfo[0]._max_green=0;
        stc._for_center_plan._ptr_subplaninfo[0]._yellow=0;
        stc._for_center_plan._ptr_subplaninfo[0]._allred=0;
        stc._for_center_plan._ptr_subplaninfo[0]._pedgreen_flash=0;
        stc._for_center_plan._ptr_subplaninfo[0]._pedred=0;

    } else {

        for (i=0;i<stc._for_center_plan._subphase_count;i++) {

           //OTMARKPRINTF  printf("Phase:%d\n",i+1);

             stc._for_center_plan._ptr_subplaninfo[i]._min_green=DataMessageIn.packet[11+7*i];
             stc._for_center_plan._ptr_subplaninfo[i]._max_green=DataMessageIn.packet[12+7*i]*256+DataMessageIn.packet[13+7*i];
             stc._for_center_plan._ptr_subplaninfo[i]._yellow=DataMessageIn.packet[14+7*i];
             stc._for_center_plan._ptr_subplaninfo[i]._allred=DataMessageIn.packet[15+7*i];
             stc._for_center_plan._ptr_subplaninfo[i]._pedgreen_flash=DataMessageIn.packet[16+7*i];
             stc._for_center_plan._ptr_subplaninfo[i]._pedred=DataMessageIn.packet[17+7*i];

           //OTMARKPRINTF  printf("minGreen:%d,maxGreen:%d,yellow:%d,allred:%d,pedGFlash:%d,pedRed%d\n",
//OTMARKPRINTF             stc._for_center_plan._ptr_subplaninfo[i]._min_green,stc._for_center_plan._ptr_subplaninfo[i]._max_green,stc._for_center_plan._ptr_subplaninfo[i]._yellow,
//OTMARKPRINTF             stc._for_center_plan._ptr_subplaninfo[i]._allred,stc._for_center_plan._ptr_subplaninfo[i]._pedgreen_flash,stc._for_center_plan._ptr_subplaninfo[i]._pedred);

        }
    }

    stc.Lock_to_Save_Plan_from_Center();             //�x�s�^�h

  //OTMARKPRINTF  printf("SAVE Plan:%d OK!!\n",DataMessageIn.packet[9]);

    if(DataMessageIn.InnerOrOutWard != cComingFromScreen) {
      vReturnToCenterACK(0x5F, 0x14);
    }


    return true;
}
} catch (...) {}
}

//---------------------------------------------------------------------------
bool PTRAFFICCCJ::vQueryPlan5F44(MESSAGEOK DataMessageIn)
{
try{

  int iQueryPlan = DataMessageIn.packet[9];

  bool bPlanStatus;
  bPlanStatus = smem.vGetTCPhasePlanSegTypeData(TC_Plan, iQueryPlan);
  if(bPlanStatus == false) {
    vReturnToCenterNACK(0x5F, 0x44, 0x02, 0x0); return false;
  }
  if( DataMessageIn.packetLength < 13) { vReturnToCenterNACK(0x5F, 0x44, 0x08, 0x00); return false; }
  else if( DataMessageIn.packetLength > 13) { vReturnToCenterNACK(0x5F, 0x44, 0x08, DataMessageIn.packetLength - 12); return false; }


  stc.Lock_to_Load_Plan_for_Center(iQueryPlan);

  unsigned char data[256];
  int iDataPtr = 4; //Form 6 Start to send SignalStatus
  data[0] = 0x5F;
  data[1] = 0xC4;
  data[2] = stc._for_center_plan._planid;
  data[3] = stc._for_center_plan._subphase_count;
  for(int i = 0; i < stc._for_center_plan._subphase_count; i++)
  {
    data[iDataPtr] = stc._for_center_plan._ptr_subplaninfo[i]._min_green;      iDataPtr++;
//OT Debug 950410  -2 line
    data[iDataPtr] = stc._for_center_plan._ptr_subplaninfo[i]._max_green / 256;      iDataPtr++;
    data[iDataPtr] = stc._for_center_plan._ptr_subplaninfo[i]._max_green % 256;      iDataPtr++;
    data[iDataPtr] = stc._for_center_plan._ptr_subplaninfo[i]._yellow;         iDataPtr++;
    data[iDataPtr] = stc._for_center_plan._ptr_subplaninfo[i]._allred;         iDataPtr++;
    data[iDataPtr] = stc._for_center_plan._ptr_subplaninfo[i]._pedgreen_flash; iDataPtr++;
    data[iDataPtr] = stc._for_center_plan._ptr_subplaninfo[i]._pedred;         iDataPtr++;
  }

  MESSAGEOK _MsgOK;
  _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, iDataPtr, true);
  _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
  writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);

  return true;

} catch (...) {}
}

//---------------------------------------------------------------------------
bool PTRAFFICCCJ::vWritePlanDBMS5F15(MESSAGEOK DataMessageIn)
{
try{

  int iPhaseOrder = DataMessageIn.packet[11];
  int iSubphaseCount = DataMessageIn.packet[12];

  if(smem.vGetTCPhasePlanSegTypeData(TC_Phase, iPhaseOrder) == false) {
    vReturnToCenterNACK(0x5F, 0x15, 0x04, 0x03); return false;
  }
  stc.Lock_to_Load_Phase_for_Center(iPhaseOrder);

  if( stc._for_center_phase._subphase_count != iSubphaseCount && iSubphaseCount != 0) {
    vReturnToCenterNACK(0x5F, 0x15, 0x04, 0x04); return false;
  }

  if( DataMessageIn.packetLength < 20+(2*DataMessageIn.packet[12])) { vReturnToCenterNACK(0x5F, 0x15, 0x08, 0x00); return false; }
  else if( DataMessageIn.packetLength > 20+(2*DataMessageIn.packet[12])) { vReturnToCenterNACK(0x5F, 0x15, 0x08, DataMessageIn.packetLength - 12); return false; }

//OT Debug 0523
//  vReturnToCenterACK(0x5F, 0x15);

  /*
  PlanID(�ɨ�p��):[9]
  Direct(��Ǥ�V):[10]
  PhaseOrder(�ɬ۽s��):[11]
  SubPhaseCount(���ۼ�):[12]

  Green(��O�ɶ�):[13][14],[15][16],[17][18]
  CycleTime(�g��):[19][20]
  Offset(�ɮt):[21][22]
  */

  int i=0;
  bool bMollacOK=false;

  //OT Debug 0523
  bool bRet;

  if (iSubphaseCount == 0)    //�{���ɬ�
      bMollacOK=stc.Lock_to_Reset_Plan_for_Center(DataMessageIn.packet[9],1);
  else  bMollacOK=stc.Lock_to_Reset_Plan_for_Center(DataMessageIn.packet[9], iSubphaseCount);

  if (bMollacOK) {

      stc._for_center_plan._planid=DataMessageIn.packet[9];
      stc._for_center_plan._dir=DataMessageIn.packet[10];
    //OTMARKPRINTF  printf("Plan:%d,Dir:%d\n",stc._for_center_plan._planid,stc._for_center_plan._dir);

/*
      if (iPhaseOrder == 0x30) iPhaseOrder = 0x20;
      else if (iPhaseOrder==0xB0) iPhaseOrder = 0x80;
*/

      stc._for_center_plan._phase_order = iPhaseOrder;
      stc._for_center_plan._subphase_count= iSubphaseCount;
    //OTMARKPRINTF  printf("Phase_Order:%d,Phase_Count:%d\n",stc._for_center_plan._phase_order,stc._for_center_plan._subphase_count);

      for (i=0;i<stc._for_center_plan._subphase_count;i++) {
         //OTMARKPRINTF  printf("Phase:%d\n",i+1);
           stc._for_center_plan._ptr_subplaninfo[i]._green=DataMessageIn.packet[13+2*i]*256+DataMessageIn.packet[14+2*i];

           if(stc._for_center_plan._ptr_subplaninfo[i]._pedgreen_flash > 0 && stc._for_center_plan._ptr_subplaninfo[i]._pedgreen_flash < stc._for_center_plan._ptr_subplaninfo[i]._green)
             stc._for_center_plan._ptr_subplaninfo[i]._green = stc._for_center_plan._ptr_subplaninfo[i]._green - stc._for_center_plan._ptr_subplaninfo[i]._pedgreen_flash;
           if(stc._for_center_plan._ptr_subplaninfo[i]._pedred > 0 && stc._for_center_plan._ptr_subplaninfo[i]._pedred < stc._for_center_plan._ptr_subplaninfo[i]._green)
             stc._for_center_plan._ptr_subplaninfo[i]._green = stc._for_center_plan._ptr_subplaninfo[i]._green - stc._for_center_plan._ptr_subplaninfo[i]._pedred;
         //OTMARKPRINTF  printf("Green:%d\n",stc._for_center_plan._ptr_subplaninfo[i]._green);
      }

      stc._for_center_plan._cycle_time=DataMessageIn.packet[13+2*i]*256+DataMessageIn.packet[14+2*i];
    //OTMARKPRINTF  printf("cycle_time:%d\n",stc._for_center_plan._cycle_time);
      stc._for_center_plan._offset=DataMessageIn.packet[15+2*i]*256+DataMessageIn.packet[16+2*i];
    //OTMARKPRINTF  printf("offset:%d\n",stc._for_center_plan._offset);

      if (stc._for_center_plan._subphase_count==0) {            //�{���ɬ�
          stc._for_center_plan._subphase_count=1;               //�����@��Phase

          //OT Debug 0523
          stc._for_center_plan._ptr_subplaninfo[i]._green=10;
      }

//OT Debug 0523
      bRet = stc.Lock_to_Save_Plan_from_Center();                      //�x�s�^�h
      if(bRet) smem.vSetTCPhasePlanSegTypeData(TC_Plan, DataMessageIn.packet[9], true);
      else {
          vReturnToCenterNACK(0x5F, 0x15, 0x04, 0x05); return false;
      }
      vReturnToCenterACK(0x5F, 0x15);
//      smem.vSaveLast92TC_5F15Time();
      screenLast92TCPlanSegmentUpdate.DisplayPlanUpdate();

    //OTMARKPRINTF  printf("SAVE Plan:%d OK!\n",DataMessageIn.packet[9]);

  }
} catch (...) {}
}

//---------------------------------------------------------------------------
bool PTRAFFICCCJ::vQueryPlanDBMS5F45(MESSAGEOK DataMessageIn)
{
try{
  int iQueryPlan = DataMessageIn.packet[9];
  unsigned short int usiGreenTMP;

  bool bPlanStatus;
  bPlanStatus = smem.vGetTCPhasePlanSegTypeData(TC_Plan, iQueryPlan);
  if(bPlanStatus == false) {
    vReturnToCenterNACK(0x5F, 0x45, 0x02, 0x0); return false;
  }
  if( DataMessageIn.packetLength < 13) { vReturnToCenterNACK(0x5F, 0x45, 0x08, 0x00); return false; }
  else if( DataMessageIn.packetLength > 13) { vReturnToCenterNACK(0x5F, 0x45, 0x08, DataMessageIn.packetLength - 12); return false; }


  stc.Lock_to_Load_Plan_for_Center(iQueryPlan);

  unsigned char data[256];
  int iDataPtr = 6;
  data[0] = 0x5F;
  data[1] = 0xC5;
  data[2] = stc._for_center_plan._planid;
  data[3] = stc._for_center_plan._dir;
  data[4] = stc._for_center_plan._phase_order;
  data[5] = stc._for_center_plan._subphase_count;

  for(int i = 0; i < data[5]; i++)
  {
    usiGreenTMP = 0;

    //OT Debug 0523 -3line
    if(stc._for_center_plan._phase_order != 0xB0) {                             //Flash PhaseOrder In HsinChu
      usiGreenTMP += stc._for_center_plan._ptr_subplaninfo[i]._green;
    } else { }
    usiGreenTMP += stc._for_center_plan._ptr_subplaninfo[i]._pedgreen_flash;
    usiGreenTMP += stc._for_center_plan._ptr_subplaninfo[i]._pedred;

    data[iDataPtr] = HI(usiGreenTMP); iDataPtr++;
    data[iDataPtr] = LO(usiGreenTMP); iDataPtr++;
  }

  //OT Debug 0523 -3line
  if(stc._for_center_plan._phase_order != 0xB0) {                             //Flash PhaseOrder In HsinChu
    data[iDataPtr] = stc._for_center_plan._cycle_time / 256; iDataPtr++;
    data[iDataPtr] = stc._for_center_plan._cycle_time % 256; iDataPtr++;
  } else {data[iDataPtr] = 0; data[iDataPtr+1] = 0; iDataPtr+=2; }

  data[iDataPtr] = stc._for_center_plan._offset / 256; iDataPtr++;
  data[iDataPtr] = stc._for_center_plan._offset % 256; iDataPtr++;

  MESSAGEOK _MsgOK;
  _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, iDataPtr, true);
  _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
  writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);

  return true;

  } catch (...) {}
}

//---------------------------------------------------------------------------
bool PTRAFFICCCJ::vSetSendStep5F91(MESSAGEOK DataMessageIn)
{
try{
  unsigned char  ucContral5F91Code;

  unsigned char data[16];
  MESSAGEOK _MsgOK;

  ucContral5F91Code = DataMessageIn.packet[9];

  if(ucContral5F91Code == 1) {                                                  //Enable Send
    smem.vSetBOOLData(CCJ_SendStepToCCJ, true);
  } else {
    smem.vSetBOOLData(CCJ_SendStepToCCJ, false);
  }
  vReturnToCenterACK(0x5F, 0x91);

  return true;

} catch (...) {}
}

//---------------------------------------------------------------------------
bool PTRAFFICCCJ::vQueryCCTPlan5F48(MESSAGEOK DataMessageIn)                    //then return 5FC8
{
try{
  unsigned char data[256];
  int iDataPtr = 6; //Form 6 Start to send SignalStatus

  unsigned short int usiGreen;

  unsigned short int usiRunningPlanID = stc.vGetUSIData(CSTC_exec_plan_plan_ID);
  stc.Lock_to_Load_Plan_for_Center(usiRunningPlanID);

  if(DataMessageIn.packet[9] == 0xAA) {

//  int iDataPtr = 6; //Form 6 Start to send SignalStatus
  data[0] = 0x5F;
  data[1] = 0xC8;
  data[2] = usiRunningPlanID;
  data[3] = stc._for_center_plan._dir;
  data[4] = stc._for_center_plan._phase_order;
  data[5] = stc._for_center_plan._subphase_count;

//OT Debug CECI
  for(int i = 0; i < data[5]; i++)                                              //data[5] = subphasecount
  {
    usiGreen = 0;
    if(stc._for_center_plan._phase_order != 0xB0) {                             //Flash PhaseOrder In HsinChu
    usiGreen += stc._for_center_plan._ptr_subplaninfo[i]._green;
    } else { }
    usiGreen += stc._for_center_plan._ptr_subplaninfo[i]._pedgreen_flash;
    usiGreen += stc._for_center_plan._ptr_subplaninfo[i]._pedred;

    data[iDataPtr] = HI(usiGreen);
    iDataPtr++;
    data[iDataPtr] = LO(usiGreen);
    iDataPtr++;
  }

  if(stc._for_center_plan._phase_order != 0xB0) {                               //Flash PhaseOrder In HsinChu
    data[iDataPtr] = HI(stc._for_center_plan._cycle_time);
    iDataPtr++;
    data[iDataPtr] = LO(stc._for_center_plan._cycle_time);
    iDataPtr++;
  } else {
    data[iDataPtr] = 0; data[iDataPtr+1] = 0; iDataPtr+=2;
  }

  data[iDataPtr] = HI(stc._for_center_plan._offset);
  iDataPtr++;
  data[iDataPtr] = LO(stc._for_center_plan._offset);
  iDataPtr++;

  MESSAGEOK _MsgOK;
  _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, iDataPtr, true);
  _MsgOK.InnerOrOutWard = cOutWard;
  writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);

  } else {
    int i18PlanID = smem.vGetINTData(TC92_PlanOneTime5F18_PlanID);
    data[0] = 0x5F;
    data[1] = 0xC8;
    data[2] = i18PlanID & 0xFF;

    MESSAGEOK _MsgOK;
    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 3, true);
    _MsgOK.InnerOrOutWard = cOutWard;
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);
  }

} catch (...) {}
}

//---------------------------------------------------------------------------
bool PTRAFFICCCJ::vCloseRedCount5FA3(MESSAGEOK DataMessageIn)
{
try{

//  stc.CalculateAndSendRedCount(10);
  printf("from CCJTraffic, get 5FA3\n");

  if(smem.vGetUCData(DynShowRedCountForAct) == 0) {  //close redcount for this phase

    smem.vSetForceClockRedCountOneTime(true);
    stc.CalculateAndSendRedCount(10);  //force close redcount.

  }

  smem.vSetThisCycleRunCCJActure5FA2(true);  //send msg for center, show that is running dyn. act.
  smem.vSetTC5F08StatusChange(true);

  return true;

} catch (...) {}
}

//---------------------------------------------------------------------------
//Set Plan Change to TOD, but don't comptision
bool PTRAFFICCCJ::vUsingNormalTODPlan5FA4(MESSAGEOK DataMessageIn)
{
try {

  printf("from CCJTraffic, get 5FA4\n");

  unsigned char data[16];
  MESSAGEOK _MsgOK;
  DATA_Bit _ControlStrategy;

  unsigned short int usiSetPlanID = stc.vGetTODRunningPlanID();

  if( DataMessageIn.packetLength < 12) { vReturnToCenterNACK(0x5F, 0xA4, 0x08, 0x00); return false; }
  else if( DataMessageIn.packetLength > 12) { vReturnToCenterNACK(0x5F, 0xA4, 0x08, DataMessageIn.packetLength - 12); return false; }
//Don't use normail v3 protocal  vReturnToCenterACK(0x5F, 0xA4);

  _ControlStrategy.DBit = smem.vGetUCData(TC92_ucControlStrategy);
  if( _ControlStrategy.switchBit.b4 == false ) {

    usiSetPlanID = smem.vGetINTData(TC92_PlanOneTime5F18_PlanID);

    data[0] = 0x0F;
    data[1] = 0x81;
    data[2] = 0x5F;
    data[3] = 0xA4;
    data[4] = usiSetPlanID;
    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 5, true);
    _MsgOK.InnerOrOutWard = cOutWard;
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);

    //return NAK
  } else {
    smem.vSetINTData(TC92_PlanOneTime5F18_PlanID, usiSetPlanID);
    smem.vSetBOOLData(TC92_PlanOneTime5F18_Change, false);

//bug, move out    smem.vSetThisCycleRunCCJPlan5F18(true);  //990402

    usiSetPlanID = smem.vGetINTData(TC92_PlanOneTime5F18_PlanID);

    data[0] = 0x0F;
    data[1] = 0x80;
    data[2] = 0x5F;
    data[3] = 0xA4;
    data[4] = usiSetPlanID;
    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 5, true);
    _MsgOK.InnerOrOutWard = cOutWard;
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);

  }


  /*

  unsigned char data[16];
  MESSAGEOK _MsgOK;
  DATA_Bit _ControlStrategy;

  unsigned short int usiSetPlanID = stc.vGetTODRunningPlanID();

  if( DataMessageIn.packetLength < 12) { vReturnToCenterNACK(0x5F, 0xA4, 0x08, 0x00); return false; }
  else if( DataMessageIn.packetLength > 12) { vReturnToCenterNACK(0x5F, 0xA4, 0x08, DataMessageIn.packetLength - 12); return false; }
//Don't use normail v3 protocal  vReturnToCenterACK(0x5F, 0xA4);

  _ControlStrategy.DBit = smem.vGetUCData(TC92_ucControlStrategy);
  if( _ControlStrategy.switchBit.b4 == false ) {

    usiSetPlanID = smem.vGetINTData(TC92_PlanOneTime5F18_PlanID);

    data[0] = 0x0F;
    data[1] = 0x81;
    data[2] = 0x5F;
    data[3] = 0xA4;
    data[4] = usiSetPlanID;
    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 5, true);
    _MsgOK.InnerOrOutWard = cOutWard;
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);

    //return NAK
  } else {
    smem.vSetINTData(TC92_PlanOneTime5F18_PlanID, usiSetPlanID);
    smem.vSetBOOLData(TC92_PlanOneTime5F18_Change, true);

//bug, move out    smem.vSetThisCycleRunCCJPlan5F18(true);  //990402

    usiSetPlanID = smem.vGetINTData(TC92_PlanOneTime5F18_PlanID);

    data[0] = 0x0F;
    data[1] = 0x80;
    data[2] = 0x5F;
    data[3] = 0xA4;
    data[4] = usiSetPlanID;
    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 5, true);
    _MsgOK.InnerOrOutWard = cOutWard;
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);

  }

  */


} catch (...) {}
}

//---------------------------------------------------------------------------
//Set Plan Change to TOD, but don't comptision
bool PTRAFFICCCJ::vActTOD5FA5(MESSAGEOK DataMessageIn)
{
try {

  time_t stc_time;
  CPhaseInfo stc_phase;
  CPlanInfo stc_plan;
  CPlanInfo stc_runningPlan;
  CSegmentInfo stc_segment;

  unsigned short int usiTmp, usiTmp2, usiPtrTmp;
  unsigned char data[255];
  MESSAGEOK _MsgOK;

  data[0] = 0x0F;
  data[1] = 0x80;
  data[2] = 0x5F;
  data[3] = 0xA5;
  _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4, true);
  _MsgOK.InnerOrOutWard = cOutWard;
  writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);


  stc.Lock_to_Determine_SegmentPlanPhase_For_Act(&stc_time, &stc_phase, &stc_plan, &stc_segment, &stc_runningPlan);

  printf("stc_phase._phase_order:%X\n", stc_phase._phase_order);
  printf("stc_plan._planid:%d\n", stc_plan._planid);
  printf("stc_segment._segment_type:%d\n", stc_segment._segment_type);
  printf("stc_segment._segment_count:%d\n", stc_segment._segment_count);

  struct tm *now=localtime(&stc_time);
  unsigned char cHour=now->tm_hour;
  unsigned char cMin=now->tm_min;
  unsigned char cSec=now->tm_sec;
  printf("now->%d:%d:%d\n", now->tm_hour, now->tm_min, now->tm_sec);


/*
    time_t currenttime = time(NULL);
    struct tm *now=localtime(&currentTime);
    unsigned char cHour=now->tm_hour;
    unsigned char cMin=now->tm_min;
    unsigned char cSec=now->tm_sec;

    unsigned short int usiStepSec = stc.vGetStepTimeUntilNextGreen();
    unsigned short int usiCycleSec = stc.vGetStepTimeUntilNextCycle();
    printf("[OTMsg] usiCycleSec:%d\n", usiCycleSec);
*/
    int iPtr = 5;
    data[0] = 0x5F;
    data[1] = 0xA5;

    data[2] = cHour;
    data[3] = cMin;
    data[4] = cSec;

    data[iPtr] = stc_plan._planid; iPtr++;
    data[iPtr] = stc_plan._phase_order; iPtr++;
    data[iPtr] = stc_plan._subphase_count; iPtr++;
    for(int i = 0; i < stc_plan._subphase_count; i++) {
      data[iPtr] = stc_plan._ptr_subplaninfo[i]._min_green; iPtr++;
      data[iPtr] = HI(stc_plan._ptr_subplaninfo[i]._max_green); iPtr++;
      data[iPtr] = LO(stc_plan._ptr_subplaninfo[i]._max_green); iPtr++;
      data[iPtr] = HI(stc_plan._ptr_subplaninfo[i]._green); iPtr++;
      data[iPtr] = LO(stc_plan._ptr_subplaninfo[i]._green); iPtr++;
      data[iPtr] = stc_plan._ptr_subplaninfo[i]._yellow; iPtr++;
      data[iPtr] = stc_plan._ptr_subplaninfo[i]._allred; iPtr++;
      data[iPtr] = stc_plan._ptr_subplaninfo[i]._pedgreen_flash; iPtr++;
      data[iPtr] = stc_plan._ptr_subplaninfo[i]._pedred; iPtr++;
    }
    data[iPtr] = HI(stc_plan._cycle_time); iPtr++;
    data[iPtr] = LO(stc_plan._cycle_time); iPtr++;
    data[iPtr] = HI(stc_plan._offset); iPtr++;
    data[iPtr] = LO(stc_plan._offset); iPtr++;

    data[iPtr] = stc_runningPlan._planid; iPtr++;
    data[iPtr] = stc_runningPlan._phase_order; iPtr++;
    data[iPtr] = stc_runningPlan._subphase_count; iPtr++;
    for(int i = 0; i < stc_runningPlan._subphase_count; i++) {
      data[iPtr] = stc_runningPlan._ptr_subplaninfo[i]._min_green; iPtr++;
      data[iPtr] = HI(stc_runningPlan._ptr_subplaninfo[i]._max_green); iPtr++;
      data[iPtr] = LO(stc_runningPlan._ptr_subplaninfo[i]._max_green); iPtr++;
      data[iPtr] = HI(stc_runningPlan._ptr_subplaninfo[i]._green); iPtr++;
      data[iPtr] = LO(stc_runningPlan._ptr_subplaninfo[i]._green); iPtr++;
      data[iPtr] = stc_runningPlan._ptr_subplaninfo[i]._yellow; iPtr++;
      data[iPtr] = stc_runningPlan._ptr_subplaninfo[i]._allred; iPtr++;
      data[iPtr] = stc_runningPlan._ptr_subplaninfo[i]._pedgreen_flash; iPtr++;
      data[iPtr] = stc_runningPlan._ptr_subplaninfo[i]._pedred; iPtr++;
    }
    data[iPtr] = HI(stc_runningPlan._cycle_time); iPtr++;
    data[iPtr] = LO(stc_runningPlan._cycle_time); iPtr++;
    data[iPtr] = HI(stc_runningPlan._offset); iPtr++;
    data[iPtr] = LO(stc_runningPlan._offset); iPtr++;

    for(int i = 0; i < 8; i++) {
      data[iPtr] = smem.vGetVDPhaseTriggerSwitch(i); iPtr++;

      usiTmp = 0;
      usiTmp2 = 0;
      for(int j = 0; j < 8; j++) {
        if(j == 0) { usiPtrTmp = iPtr; iPtr++; }
        usiTmp = smem.vGetVDPhaseMapTable(i, j);
        if(usiTmp > 0) {
          usiTmp2++;  //iMaxVDNum
          data[iPtr] = HI(usiTmp); iPtr++;
          data[iPtr] = LO(usiTmp); iPtr++;
        }
      }
      data[usiPtrTmp] = usiTmp2;
    }

//    data[3] = stc.vGetUSIData(CSTC_exec_phase_current_subphase) + 1;
//    data[4] = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step)  + 1;

/*
      data[5] = (usiStepSec>>8) & 0x00ff;
      data[6] = usiStepSec & 0x00ff;
      data[7] = (usiCycleSec>>8) & 0x00ff;
      data[8] = usiCycleSec & 0x00ff;
*/

    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, iPtr, true);
    _MsgOK.InnerOrOutWard = cOutWard;

    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);
    return true;


  } catch (...) {}
}



//ACK------------------------------------------------------------------------
int PTRAFFICCCJ::vReturnToCenterACK(unsigned char ucDevCode,
                                     unsigned char ucControlCode)
{
try{
    unsigned char data[4];

    data[0] = 0x0F;
    data[1] = 0x80;
    data[2] = ucDevCode;
    data[3] = ucControlCode;

    MESSAGEOK _MsgOK;

    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4,true);
    _MsgOK.InnerOrOutWard = cOutWard;
//    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);

    return 0;
  } catch (...) {}
}
//---------------------------------------------------------------------------
int PTRAFFICCCJ::vReturnToCenterNACK(unsigned char ucDevCode,
                                     unsigned char ucControlCode,
                                     unsigned char ErrorCode,
                                     unsigned char ParameterNumber)
{
try{
    unsigned char data[6];

    data[0] = 0x0F;
    data[1] = 0x81;
    data[2] = ucDevCode;
    data[3] = ucControlCode;
    data[4] = ErrorCode;
    data[5] = ParameterNumber;

    MESSAGEOK _MsgOK;

    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 6,true);
    _MsgOK.InnerOrOutWard = cOutWard;
//    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);

    return 0;
  } catch (...) {}
}
