#include "CTIMER.h"
#include "var.h"
#include "CDataToMessageOK.h"
#include "WRITEJOB.h"
#include "SMEM.h"
#include "DIGITALIO.h"

#include "SCREENMain.h"
#include "SCREENCtlSetup.h"
#include "SCREENModifyDT.h"
#include "SCREENNone.h"
#include "SCREENOnline.h"
#include "SCREENCommStat.h"
#include "SCREENTotalRun.h"
#include "screenCurrentLightStatus.h"
#include "screenChainStatus.h"
#include "SCREENOperStat.h"
#include "screenActuateArwenStatus.h"
#include "screenReverseMenu.h"
#include "screenGreenConflictRecord.h"

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#include <pthread.h>
#include <signal.h>
#include <time.h>

#include "CSTC.h"
//--------------------------------------------------------------------------
intervalTimer _intervalTimer;

pthread_t intervalTimer::thread_forVDTimer;

timer_t intervalTimer::_t1;
timer_t intervalTimer::_t2;
timer_t intervalTimer::_t3;
timer_t intervalTimer::_t4;
timer_t intervalTimer::_t5;
timer_t intervalTimer::_t6;
timer_t intervalTimer::_t7;
timer_t intervalTimer::_t8;
timer_t intervalTimer::_t9;
timer_t intervalTimer::_tMinchun;
//OTSS
timer_t intervalTimer::_t10;
timer_t intervalTimer::_tTrafficeLight;

struct itimerspec intervalTimer::_it1;
struct itimerspec intervalTimer::_it2;
struct itimerspec intervalTimer::_it3;
struct itimerspec intervalTimer::_it4;
struct itimerspec intervalTimer::_it5;
struct itimerspec intervalTimer::_it6;
struct itimerspec intervalTimer::_it7;
struct itimerspec intervalTimer::_it8;
struct itimerspec intervalTimer::_it9;
struct itimerspec intervalTimer::_itMinchun;
//OTSS
struct itimerspec intervalTimer::_it10;
struct itimerspec intervalTimer::_itTrafficeLight;

pthread_mutex_t intervalTimer::_ctimer_mutex = PTHREAD_MUTEX_INITIALIZER;

//intervalTimer-------------------------------------------------------------
intervalTimer::intervalTimer() {
//  iS0Count = 0;
//  iSKCount = 0;
  usiGreenConflictCount = 0;
}
//--------------------------------------------------------------------------
void intervalTimer::ThreadsGenerate(void) {
  try {
    //OTMARKPRINTF  printf("\nMAIN:                 pid=%d\n",getpid());

/*
    sigset_t main_mask;
    sigfillset( & main_mask ); //block all signal, Ctrl+C not workable
    sigprocmask( SIG_SETMASK, & main_mask, NULL );
  //OTMARKPRINTF  printf("CSTC::CSTC(): blocking all signals in main!!\n");
*/

    printf("CTimer Starting\n");

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_create(&(intervalTimer::thread_forVDTimer),
                   &attr,
                   &(intervalTimer::PTime),
                   NULL);

    pthread_attr_destroy(&attr);
  } catch (...) {}
}
//--------------------------------------------------------------------------
//**********************************************************
//****         Creating and Setting Timers
//**********************************************************
//--------------------------------------------------------------------------
void intervalTimer::TimersCreating(void) {
  try {

    struct sigevent _sig1, _sig2, _sig3, _sig4, _sig5, _sig6, _sig7, _sig8,
        _sig9, _sigMinchun;
    //OTSS
    struct sigevent _sig10;

    struct sigevent _sigTrafficeLight;


    _sig1.sigev_notify = SIGEV_SIGNAL;
    _sig1.sigev_signo = RTSIGNAL_Timer;
    _sig1.sigev_value.sival_int = 10;
    if (timer_create(CLOCK_REALTIME, &_sig1, &_t1)) exit(1);

    _sig2.sigev_notify = SIGEV_SIGNAL;
    _sig2.sigev_signo = RTSIGNAL_Timer;
    _sig2.sigev_value.sival_int = 11;
    if (timer_create(CLOCK_REALTIME, &_sig2, &_t2)) exit(1);

    _sig3.sigev_notify = SIGEV_SIGNAL;
    _sig3.sigev_signo = RTSIGNAL_Timer;
    _sig3.sigev_value.sival_int = 12;
    if (timer_create(CLOCK_REALTIME, &_sig3, &_t3)) exit(1);

    _sig4.sigev_notify = SIGEV_SIGNAL;
    _sig4.sigev_signo = RTSIGNAL_Timer;
    _sig4.sigev_value.sival_int = 13;
    if (timer_create(CLOCK_REALTIME, &_sig4, &_t4)) exit(1);

    _sig5.sigev_notify = SIGEV_SIGNAL;
    _sig5.sigev_signo = RTSIGNAL_Timer;
    _sig5.sigev_value.sival_int = 14;
    if (timer_create(CLOCK_REALTIME, &_sig5, &_t5)) exit(1);

    _sig6.sigev_notify = SIGEV_SIGNAL;
    _sig6.sigev_signo = RTSIGNAL_Timer;
    _sig6.sigev_value.sival_int = 15;
    if (timer_create(CLOCK_REALTIME, &_sig6, &_t6)) exit(1);

    _sig7.sigev_notify = SIGEV_SIGNAL;
    _sig7.sigev_signo = RTSIGNAL_Timer;
    _sig7.sigev_value.sival_int = 100;
    if (timer_create(CLOCK_REALTIME, &_sig7, &_t7)) exit(1);

    _sig8.sigev_notify = SIGEV_SIGNAL;
    _sig8.sigev_signo = RTSIGNAL_Timer;
    _sig8.sigev_value.sival_int = 101;
    if (timer_create(CLOCK_REALTIME, &_sig8, &_t8)) exit(1);

    _sig9.sigev_notify = SIGEV_SIGNAL;
    _sig9.sigev_signo = RTSIGNAL_Timer;
    _sig9.sigev_value.sival_int = 500;
    if (timer_create(CLOCK_REALTIME, &_sig9, &_t9)) exit(1);
    _sigMinchun.sigev_notify = SIGEV_SIGNAL;
    _sigMinchun.sigev_signo = RTSIGNAL_Timer;
    _sigMinchun.sigev_value.sival_int = 610;
    if (timer_create(CLOCK_REALTIME, &_sigMinchun, &_tMinchun)) exit(1);

    _sig10.sigev_notify = SIGEV_SIGNAL;
    _sig10.sigev_signo = RTSIGNAL_Timer;
    _sig10.sigev_value.sival_int = 501;
    if (timer_create(CLOCK_REALTIME, &_sig10, &_t10)) exit(1);

    _sigTrafficeLight.sigev_notify = SIGEV_SIGNAL;
    _sigTrafficeLight.sigev_signo = RTSIGNAL_Timer;
    _sigTrafficeLight.sigev_value.sival_int = 600;
    if (timer_create(CLOCK_REALTIME,
                     &_sigTrafficeLight,
                     &_tTrafficeLight))
      exit(1);

    printf("CTimer Create OK!\n");

  } catch (...) {}
}
//--------------------------------------------------------------------------
void intervalTimer::TimersSetting(void) {
  try {
    unsigned char ucTmp;
    int iTmp;

    _it1.it_value.tv_sec = 1;
    _it1.it_value.tv_nsec = 0;
    _it1.it_interval.tv_sec = 1;
    _it1.it_interval.tv_nsec = 0;
    if (timer_settime(_t1, 0, &_it1, NULL)) exit(1);

    _it2.it_value.tv_sec = 30;
    _it2.it_value.tv_nsec = 0;
    _it2.it_interval.tv_sec = 1;
    _it2.it_interval.tv_nsec = 0;
    if (timer_settime(_t2, 0, &_it2, NULL)) exit(1);

    _it3.it_value.tv_sec = 40;
    _it3.it_value.tv_nsec = 0;
    _it3.it_interval.tv_sec = 8;
    _it3.it_interval.tv_nsec = 0;
    if (timer_settime(_t3, 0, &_it3, NULL)) exit(1);

    _it4.it_value.tv_sec = 0;
    _it4.it_value.tv_nsec = 0;
    _it4.it_interval.tv_sec = 0;
    _it4.it_interval.tv_nsec = 0;
    if (timer_settime(_t4, 0, &_it4, NULL)) exit(1);

    _it5.it_value.tv_sec = 10;
    _it5.it_value.tv_nsec = 0;
    _it5.it_interval.tv_sec = 8;
    _it5.it_interval.tv_nsec = 0;
    if (timer_settime(_t5, 0, &_it5, NULL)) exit(1);

    ucTmp = smem.vGetHWCycleCodeFor_0F14_0FC4();
    switch (ucTmp) {
      case (0):
        iTmp = 0;
        break;
      case (1):
        iTmp = 1;
        break;
      case (2):
        iTmp = 2;
        break;
      case (3):
        iTmp = 5;
        break;
      case (4):
        iTmp = 60;
        break;
      case (5):
        iTmp = 300;
        break;
    }
    _it6.it_value.tv_sec = iTmp;
    _it6.it_value.tv_nsec = 0;
    _it6.it_interval.tv_sec = iTmp;
    _it6.it_interval.tv_nsec = 0;
    if (timer_settime(_t6, 0, &_it6, NULL)) exit(1);

    _it7.it_value.tv_sec = 0;
    _it7.it_value.tv_nsec = 0;
    _it7.it_interval.tv_sec = 0;
    _it7.it_interval.tv_nsec = 0;
    if (timer_settime(_t7, 0, &_it7, NULL)) exit(1);

    _it8.it_value.tv_sec = 0;
    _it8.it_value.tv_nsec = 0;
    _it8.it_interval.tv_sec = 0;
    _it8.it_interval.tv_nsec = 0;
    if (timer_settime(_t8, 0, &_it8, NULL)) exit(1);

/*
    _it9.it_value.tv_sec = 0;                                                   //5F1C
    _it9.it_value.tv_nsec = 0;
    _it9.it_interval.tv_sec = 0;
    _it9.it_interval.tv_nsec = 0;
    if ( timer_settime( _t9, 0, & _it9, NULL ) ) exit( 1 );
*/

//OTSS
    _it10.it_value.tv_sec =
        20;                                                 //W77E58, only one
    _it10.it_value.tv_nsec = 0;
    _it10.it_interval.tv_sec = 0;
    _it10.it_interval.tv_nsec = 0;                                  // 1/10 sec
    if (timer_settime(_t10, 0, &_it10, NULL)) exit(1);

    _itTrafficeLight.it_value.tv_sec = 0;
    _itTrafficeLight.it_value.tv_nsec = 200000000;  //0.2 sec
    _itTrafficeLight.it_interval.tv_sec = 2;
    _itTrafficeLight.it_interval.tv_nsec = 0;
    if (timer_settime(_tTrafficeLight, 0, &_itTrafficeLight, NULL)) exit(1);

    printf("CTimer Set OK!\n");

  } catch (...) {}
}
//--------------------------------------------------------------------------
void *intervalTimer::PTime(void *arg) {
  try {
    int tempFace = 0;

    unsigned char uc6F00[2];
    uc6F00[0] = 0x6F;
    uc6F00[1] = 0x00;

    unsigned char uc0F04[4];
    uc0F04[0] = 0x0F;
    uc0F04[1] = 0x04;
    MESSAGEOK _MSG;

    DATA_Bit DIOByte;

    //OTSS +3
    unsigned char ucXG[4];
    ucXG[0] = 0x58;       // X
    ucXG[1] = 0x47;       // G
    ucXG[2] = 0x0D;       // CR

    //OTMARKPRINTF  printf( "THREAD_VDINFO: pid=%d\n", getpid() );

    TimersCreating();
    TimersSetting();

    int VDsignum = 0;
    int VDrid = 9999;

    int SSInitCount = 0;

    int iCommuncationResetCount = 0;
    int iDBLockCount = 0;

    int iLastAliveStatusCount = 0;

    int iTC92SignalLightStatusVal = 5;
    int iTransmitCycle5F0F_Count = 0;
    iTC92SignalLightStatusVal =
        smem.vGetINTData(TC92SignalLightStatus_5F0F_IntervalTime);
    int iTC92SignalStepStatusVal = 5;
    int iTransmitCycle5F03_Count = 0;
    iTC92SignalStepStatusVal =
        smem.vGetINTData(TC92SignalStepStatus_5F03_IntervalTime);

    time_t _RunSec, _SSLastRun;

    //BT9512260001 START
    char msg[256];
    memset(msg, 0, sizeof(msg));
    //BT9512260001 END


    sigset_t Timer_mask;
    sigfillset(&Timer_mask);
    sigprocmask(SIG_SETMASK, &Timer_mask, NULL);
    sigemptyset(&Timer_mask); //unblock all signal
    sigaddset(&Timer_mask, RTSIGNAL_Timer);    //block VD_SIGNAL
    sigaddset(&Timer_mask, RTSIGNAL_RECODE_SMEM);    //block VD_SIGNAL


    siginfo_t VDTimer_siginfo;
    unsigned short int usiTmp;
    bool bConnectCenterStatus;
    DATA_Bit _ControlStrategy;

    unsigned short int usiCurrentSubphaseStep;
    unsigned short int usiCurrentSubphaseStepN;
    bool bStepChangeOK;

    int iTmp;
    unsigned char ucCounterWDTForLightboardRedcount;

//OT20110526
    int iMinCycleTime;
    unsigned char ucTmp;
    time_t currentTime;
    struct tm *now;
    time_t tmpTime;

    //jacky20151210
    unsigned char RequestKeypad = 0;

    while (1) {
      while (smem.vGetTimerMutexRESET() == 1) {
//          printf("timer set, ctimer wait!\n");
        usleep(100);
      }
      smem.vSetTimerMutexCTIMER(1);

      VDsignum = sigwaitinfo(&Timer_mask, &VDTimer_siginfo);

      switch (VDsignum) {
        case (RTSIGNAL_Timer):
          VDrid = VDTimer_siginfo.si_value.sival_int;

          switch (VDrid) {
            case (10):

              smem.vSendPedAndRedCountEverySec();   //台中版紅燈倒數
              //WatchDog
              printf(
                  "*********************************************************************\n");
              printf("Current Subphase %d \n",
                     stc.vGetUSIData(CSTC_exec_phase_current_subphase));
              printf("Current Step %d \n",
                     stc.vGetUSIData(CSTC_exec_phase_current_subphase_step));
              printf("Current Sec %d \n", stc.vGetStepTime());
              printf(
                  "*********************************************************************\n");

              CSTC::count5F1C_AlreadyPassedSec();
              //OT20110526
              currentTime = time(NULL);
              now = localtime(&currentTime);

              _intervalTimer.vRefreshWatchDog();

              smem.vSet5F18EffectTimeCountDown();

              smem.vSet_DIOByte(digitalIO.vReadDigitalIO());

              _intervalTimer.GetDoorStatus();

              _intervalTimer.vCheckSMEM();

              tempFace = smem.GetcFace();

              if (tempFace == cMAIN) screenMain.DisplayDateTime();
              else if (tempFace == cCTLSETUP) screenCtlSetup.DisplayDateTime();
              else if (tempFace == cMODIFYDT)
                screenModifyDT.DisplayCurrentDateTime();
              else if (tempFace == cCOMMSTAT) screenCommStat.UpdateComm();
              else if (tempFace == cTOTALRUN) screenTotalRun.DisplayTotalTime();
              else if (tempFace == cCURRENTLIGHTSTATUS) {
                screenCurrentLightStatus.vRefreshStepSec();
                screenCurrentLightStatus.DisplayDynSegStatus();
              } else if (tempFace == cCHAINSTATUS)
                screenChainStatus.vRefreshChainStatusData();
              // else if (tempFace == cOPERSTAT)   //Eason20200210
                // screenOperStat.vShowGreenConflict();
              else if (tempFace == cREVERSETIMTMENU)
                screenReverseMenu.vRefreshStepSec();
                else if (tempFace == cGREENCONFLICTRECORD)
                screenGreenConflictRecord.vShowGreenConflictRecord();
                
//                                  else if (tempFace == cACTUATEARWENSTATUS) screenActuateArwenStatus.vRefreshEverySec();
              screenActuateArwenStatus.vRefreshEverySec();
//Remove vSend92VD_6F02
//Remove smem.GetSSComm() == false

              if (iLastAliveStatusCount > 300) {
                smem.vSaveLastAliveStatus();
                iLastAliveStatusCount = 0;
              }
              iLastAliveStatusCount++;

//---------------------------------------------------------
              if (smem.vGetBOOLData(TC92_TransmitCycle5F0F_Change)) {
                iTC92SignalLightStatusVal =
                    smem.vGetINTData(TC92SignalLightStatus_5F0F_IntervalTime);
                smem.vSetBOOLData(TC92_TransmitCycle5F0F_Change, false);
              }
              if (iTransmitCycle5F0F_Count >= iTC92SignalLightStatusVal) {
                if ((iTC92SignalLightStatusVal != 0x00)
                    && (iTC92SignalLightStatusVal != 0xFF)) {
                  //OTMARKPRINTF  printf("[Message] Send Interval:%d\n", iTC92SignalLightStatusVal);
                  stc.ReportCurrentSignalMap_5F0F();
                }
                iTransmitCycle5F0F_Count = 0;
              }
              iTransmitCycle5F0F_Count++;
//---------------------------------------------------------
              if (smem.vGetBOOLData(TC92_TransmitCycle5F03_Change)) {
                iTC92SignalStepStatusVal =
                    smem.vGetINTData(TC92SignalStepStatus_5F03_IntervalTime);
                smem.vSetBOOLData(TC92_TransmitCycle5F03_Change, false);
              }
              if (iTransmitCycle5F03_Count >= iTC92SignalStepStatusVal) {
                if ((iTC92SignalStepStatusVal != 0x00)
                    && (iTC92SignalStepStatusVal != 0xFF)) {
                  //OTMARKPRINTF  printf("[Message] Send Step IntervalQQ:%d\n", iTC92SignalStepStatusVal);
                  stc.ReportCurrentStepStatus();
                  //OT disable 5F80 for standard soft
             //     stc.vReport5F80CCTProtocalSendStep();

                  // stc.ReportCurrentOperationMode_5F08();            //new add by cct
                  stc.vReportCCTRevStatus5F82();            //OT990629

//                  smem.vSetTC5F08StatusChange(true);  //interval data, force send.
                }
                iTransmitCycle5F03_Count = 0;
              }
              iTransmitCycle5F03_Count++;
//---------------------------------------------------------

              /* chain detect */
              _intervalTimer.vChildChain();

//MOveTo600                                  _intervalTimer.vSendHeartBeatToLCX405();

              /*For CCJ DongDa XiBin */
//              if (smem.vGetBOOLData(CCJ_SendStepToCCJ)) {
//                stc.ReportCurrentStepStatus5F03toCCJ();
//              }

              //check train chain
              if (smem.vGetUCData(TC_TrainChainEnable) == 1) {
                //Railway switch Enable.
                if (smem.vGetUCData(TC_TrainChainNOW) == 1) {  //train coming

                  if (smem.vGetUCData(TC_TrainChainLast) == 0) {
                    //log and colose redcount

                    //OT990617
                    smem.vSetUCData(TC_TrainChainNOW,
                                    1); // set status repeat, avoid TC_TrainChainLast retragger
                    smem.vWriteMsgToDOM("Get Train Coming");
//                                          smem.vSetUCData(TC_Redcount_Display_Enable, 0);
                    stc.CalculateAndSendRedCount(10); //close redcount

//                    stc.ReportCurrentOperationMode_5F08();
                    smem.vSetTC5F08StatusChange(true);  //interval data, force send.
                  }

                  stc.vTrainComing();
                  smem.vSetUCData(TC_TrainChainNOW, 1);
                } else {                                    //No Train
                  if (smem.vGetUCData(TC_TrainChainLast) == 1) {
                    //train leaving yet. //log and open redcount
                    smem.vSetUCData(TC_TrainChainNOW, 0);
                    // set status repeat, avoid TC_TrainChainLast retragger
                    stc.vTrainLeaving();
//                                      printf("TC_TrainChainLast = 1\n");
                    smem.vWriteMsgToDOM("Get Train Leaving");

//                                        smem.vSetUCData(TC_Redcount_Display_Enable, 1);
//                    stc.ReportCurrentOperationMode_5F08();
                    smem.vSetTC5F08StatusChange(true);  //interval data, force send.
                  }
                  smem.vSetUCData(TC_TrainChainNOW, 0);
                }
              }

              //check Rev Control Status
              if (smem.vGetUCData(TC_ReverseLane_Manual_Enable_Status) == 1) {
                //Enable Button
                _RunSec = time(NULL);
                _SSLastRun = smem.GetRevManualPadTime();
                //RevManualpad, borrow form SS.
//                                    printf("Rev _RunSec:%d\n", _RunSec);
//                                    printf("Rev _SSLastRun:%d\n", _SSLastRun);
                if (_RunSec >= _SSLastRun && _SSLastRun != 0 &&
                    (_RunSec - _SSLastRun) > 6) {
                  printf("Rev Comm Timeout!\n");
                  stc.vSetRevStep(3);                       // XX
                  stc.vLightRevSingal();
                  smem.vSetUCData(TC_ReverseLane_Control_Mode, 0);
                }
              }

              //990402
              if (smem.vGetTC5F08StatusChange() == true) {
                smem.vSetTC5F08StatusChange(false);
//                _intervalTimer.vReportCurrentOperationMode_5F08_inCtimer();
                stc.ReportCurrentOperationMode_5F08();
              }

              //OT990618
              if (smem.vGetUCData(revSyncEnable) == true) {
                if (iLastAliveStatusCount % smem.vGetUCData(revSyncInterval)
                    == 0) {
                  smem.vCheckRevSyncStatus();
                  stc.vSendRevSync();
                }
              }

              //jacky20140507
              stc.vReverselog();

              //OT20131210
              stc.vReportBF02CCTProtocalSendKaikinStep();

              //OT20140415
              unsigned short int usiAdjRemainSec;
              if (smem.vGetBOOLData(TCDynSegSwitch)) {
                usiAdjRemainSec = smem.vGetUSIData(TCDynSegRemainSec);
                if (usiAdjRemainSec > 0) {
                  smem.vSetUSIData(TCDynSegRemainSec, (usiAdjRemainSec - 1));
                } else {
                  smem.vSetBOOLData(TCDynSegSwitch, false);
                  smem.vSetUCData(TCDynSegAdjustType, 0);
                  smem.vSetUSIData(TCDynSegAdjustSec, 0);
                }
              }

              //OT20110526
              smem.vSetSystemTime(currentTime);


              if (smem.GetRequestKeypad() == 1) {
                if (RequestKeypad < 5) {
                  SendRequestToKeypad();
                  RequestKeypad++;
                } else if (RequestKeypad >= 5) {
                  smem.SetRequestKeypad(0);
                  RequestKeypad = 0;
                  stc.Lock_to_Set_Control_Strategy(STRATEGY_TOD);
                }
              } else if (smem.GetRequestKeypad() == 0) {
                RequestKeypad = 0;
              }

              break;
            case (11):
//Remove  _intervalTimer.vCheckAndReSendSS_S0_Status();
              smem.vCheckConnectStatus();

              //OT20110526
              iMinCycleTime = 600;

              ucTmp = smem.vGetHWCycleCodeFor_0F14_0FC4();
              switch (ucTmp) {
                case (0):  //means stop
                  iTmp = 600;
                  break;
                case (1):
                  iTmp = 1;
                  break;
                case (2):
                  iTmp = 2;
                  break;
                case (3):
                  iTmp = 5;
                  break;
                case (4):
                  iTmp = 60;
                  break;
                case (5):
                  iTmp = 300;
                  break;
              }
              if (iTmp > 0 && iTmp < iMinCycleTime) {
                iMinCycleTime = iTmp;
              }

              iTmp = smem.vGetINTData(TC92SignalLightStatus_5F0F_IntervalTime);
              if (iTmp == 0xFF) {  //stop send
                iTmp = 600;
              }
              if (iTmp > 0 && iTmp < iMinCycleTime) {
                iMinCycleTime = iTmp;
              }

              iTmp = smem.vGetINTData(TC92SignalStepStatus_5F03_IntervalTime);
              if (iTmp == 0xFF) {  //stop send
                iTmp = 600;
              }
              if (iTmp > 0 && iTmp < iMinCycleTime) {
                iMinCycleTime = iTmp;
              }
              iMinCycleTime += 2;  //set delay time.

              tmpTime = smem.vGetLastGetProtocolTime();
              if (currentTime >= tmpTime
                  && (currentTime - tmpTime) <= iMinCycleTime) {
                smem.vSaveCenterConnectStatus(true);
              } else {
                smem.vSaveCenterConnectStatus(false);
              }

              //OT20110825
              tmpTime = smem.vGetLastTrafficeLightTime();
//                                  printf("currentTime - tmpTime:%d\n", currentTime - tmpTime);
              if (tmpTime >= currentTime) {  //log time > system time
                smem.vSetTrafficLightBoardConnectStatus(true);
              } else {
                if ((currentTime - tmpTime) <= 60) {
                  smem.vSetTrafficLightBoardConnectStatus(true);
                } else {
                  smem.vSetTrafficLightBoardConnectStatus(false);
                }
              }


              break;
            case (12):
//Remove                                   _intervalTimer.vCheckAndReSendSS_SK_Status();
              _intervalTimer.vCheckScreenAndDoSomething();

//Remove                                    _SSLastRun = smem.vGetSSCommTime();         //檢�\uFFFDSmartSensor
              _RunSec = time(NULL);
//                                    bConnectCenterStatus = smem.vLoadCenterConnectStatus();
//                                    if(bConnectCenterStatus)  usiTmp = smem.vGet92VDTransmitType(1);
//                                    else usiTmp = smem.vGet92VDTimeGap();
//                                    if( ((_RunSec - _SSLastRun) > usiTmp*2) && (_RunSec > _SSLastRun) )
//                                      smem.SetSSComm(false);

              _intervalTimer.vCheckVDUpdateDBAndDoSomething();

              if ((_RunSec - smem.GetLastKeypadTime())
                  > 300) {  //TEST, 990325 for amegids, don't clear screen
                screenNone.DisplayNone();
              }

//                                    if (smem.GetKeypadReturn()==false)  SendRequestToKeypad();
              SendRequestToKeypad();

              break;
            case (13):                                                           //VD SIM
//                                   _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(uc6F00, 2,false);
//                                   _MSG.InnerOrOutWard = cInner;
//                                   writeJob.WriteWorkByMESSAGEOUT(_MSG);
              break;
            case (14):
//                                     _intervalTimer.vSendHeartBeatToLCX405();
//                                   _intervalTimer.vSSInit(SSInitCount);
//                                   SSInitCount++;
              break;


            case (15):  //0F04, HwStatus AutoReport
              uc0F04[2] = smem.vGetHardwareStatus(3);
              uc0F04[3] = smem.vGetHardwareStatus(4);
              _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(uc0F04, 4, true);
              _MSG.InnerOrOutWard = cOutWard;
              writeJob.WritePhysicalOut(_MSG.packet,
                                        _MSG.packetLength,
                                        DEVICECENTER92);

              break;

            case (100):
              _intervalTimer.vCommuncationReset(iCommuncationResetCount);                        //default not start
              iCommuncationResetCount++;
              if (iCommuncationResetCount >= 3) iCommuncationResetCount = 0;
              break;

            case (101):
              _intervalTimer.vDBLockRequest(iDBLockCount);                        //default not start
              iDBLockCount++;
              if (iDBLockCount >= 6) {                      //TimeOut
                iDBLockCount = 0;
                screenOnline.RequestTimeOut();
              }
              if (smem.GetDbOperStat() != 1 || smem.GetcFace() != cONLINE) {
                iDBLockCount = 0;
                _intervalTimer.vDBLockRequest(10);
              }
              break;
            case (500):                                      //\uFFFD��\uFFFD\uFFFD\uFFFD\uFFFDTOD,
              //vAllDynamicToTODCount()

              memset(msg, 0, sizeof(msg));

              usiCurrentSubphaseStep =
                  stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);
              printf("printfMsg [In Dyn] currentStep:%d %d\n",
                     usiCurrentSubphaseStep,
                     Protocal5F1CStopStep);
              sprintf(msg,
                      "printfMsg [In Dyn] currentStep:%d %d",
                      usiCurrentSubphaseStep,
                      Protocal5F1CStopStep);

              //smem.vWriteMsgToDOM(msg);
              memset(msg, 0, sizeof(msg));

              if (stc.Lock_to_LoadControlStrategy() == STRATEGY_ALLDYNAMIC) {
                timer_gettime(_tMinchun, &_itMinchun);
                if (_itMinchun.it_value.tv_sec <= 0) {
                  printf("Lock_to_Set_Control_Strategy by Timer!!!\n");

                  stc.Lock_to_Set_Control_Strategy(STRATEGY_TOD);

                  sprintf(msg,
                          "[Debug] Change to TOD at CTIMER.cpp, currentStep:%d",
                          usiCurrentSubphaseStep);
                  smem.vWriteMsgToDOM(msg);
                  memset(msg, 0, sizeof(msg));

                  _ControlStrategy.DBit =
                      smem.vGetUCData(TC92_ucControlStrategy);
                  _ControlStrategy.switchBit.b1 = true;
                  _ControlStrategy.switchBit.b5 = false;
                  _ControlStrategy.switchBit.b6 = false;

                  smem.vSetUCData(TC92_ucControlStrategy,
                                  _ControlStrategy.DBit);
                  smem.vSetINTData(TC92_iEffectTime, 0);
                } else {
                  CSTC::Lock_to_Set_Next_Step();
                  CSTC::Dyn_to_TOD_Step_set(usiCurrentSubphaseStep);
                }
              }//if( stc.Lock_to_LoadControlStrategy() == STRATEGY_ALLDYNAMIC )
              printf(
                  "--------------------------------------------------------\n");
              //BT9512260001 START
              //smem.vWriteMsgToDOM(msg);
              //BT9512260001 END
              break;
            case (610):
              CSTC::setMinchunDynFlag(false);
              break;

//OTSS
            case (501):
              sprintf(msg,
                      "Ask W77E58 Version, booting version year is :%d",
                      smem.vGetW77E58FWVer(0));
              smem.vWriteMsgToDOM(msg);
              memset(msg, 0, sizeof(msg));

//OT20110825, move out
//OT20111026, coming back
              if (smem.vGetW77E58FWVer(0) == 0) {
                writeJob.WriteAskW77E58FWVer();                 //ask W77e58 Fw
//                                  _it10.it_value.tv_sec = 0;                      //clear time.
              }
              break;

            case (600):
              _intervalTimer.vSendHeartBeatToLCX405();
              /* OT20111026
              writeJob.WriteAskW77E58FWVer();                 //ask W77e58 Fw, OT20110825
              */

              break;

            default:
              perror("CSTC TIMERID: error!!");
              //OTMARKPRINTF  printf("VDrid: \n", VDrid);
          }
          break;


        case (RTSIGNAL_RECODE_SMEM):
          smem.vSaveShareMemoryDataToDOM();
          break;

        default:
          break;

      }
      smem.vSetTimerMutexCTIMER(0);

    }
  } catch (...) {}
}
//--------------------------------------------------------------------------
bool intervalTimer::GetDoorStatus(void) {
  try {

    unsigned char uc0F04[4];
    bool bShouldSendMsg = false;
    bool bCmpRet;

    DATA_Bit tempDIOByte = smem.Get_DIOByte();
    DATA_Bit tempOldDIOByte = smem.Get_OldDIOByte();

    smem.vSetUCData(TC_GreenConflictDetFromDIO, tempDIOByte.DBit);
    bCmpRet = smem.vCmpGreenConflict();

    if (bCmpRet == false) { usiGreenConflictCount++; }
    else { usiGreenConflictCount = 0; }

    if (usiGreenConflictCount > 30000) usiGreenConflictCount = 0;

    if (SignalConflictVersion) {
      if (usiGreenConflictCount > 2) {
//      bShouldSendMsg = true;
        smem.vSetBOOLData(TC_SignalConflictError, true);

        printf("Lock_to_Set_Control_Strategy by usiGreenConflictCount!!!\n");
        printf("Lock_to_Set_Control_Strategy by usiGreenConflictCount!!!\n");
        printf("Lock_to_Set_Control_Strategy by usiGreenConflictCount!!!\n");


        stc.Lock_to_Set_Control_Strategy(STRATEGY_FLASH);
      }
    }

    if (usiGreenConflictCount == 3) {

      uc0F04[0] = 0x0F;
      uc0F04[1] = 0x04;
      MESSAGEOK _MSG;
      //send HW Status
      uc0F04[2] = smem.vGetHardwareStatus(3);
      uc0F04[3] = smem.vGetHardwareStatus(4);
      _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(uc0F04, 4, true);
      _MSG.InnerOrOutWard = cOutWard;
      writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICECENTER92);

      return true;
    }

    if (smem.vGetBOOLData(TC_SignalConflictError) == true) {
      uc0F04[0] = 0x0F;
      uc0F04[1] = 0x04;
      MESSAGEOK _MSG;
      //send HW Status
      uc0F04[2] = smem.vGetHardwareStatus(3);
      uc0F04[3] = smem.vGetHardwareStatus(4);
      _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(uc0F04, 4, true);
      _MSG.InnerOrOutWard = cOutWard;
      writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICECENTER92);

      return true;

    }

    return false;
  } catch (...) {}
}

//--------------------------------------------------------------------------
bool intervalTimer::vChildChain(void) {
  try {
    static time_t now;

    static struct tm *basetime;

    static struct tm *currenttime;

    static int iChainSignalNotChange;

    sChildChain sCCTMP;
    long lCycleTime;

    bool bChildChainStatus;
    bool bChildChainStatusOld;
    unsigned char ucMBType;

    DATA_Bit tempDIOByte = smem.Get_DIOByte();
//  printf("DIO:%X\n", tempDIOByte.DBit);
    DATA_Bit tempOldDIOByte = smem.Get_OldDIOByte();

    sCCTMP = smem.vGetChildChainStruct();
    now = time(NULL);
//  printf(" now:%d\n", now);

    ucMBType = smem.vGetMotherBoardType();

    if (ucMBType == MB_ICOP6115) {
      bChildChainStatus = tempDIOByte.switchBit.b2;
      bChildChainStatusOld = tempOldDIOByte.switchBit.b2;
    } else {
      bChildChainStatus = tempDIOByte.switchBit.b1;
      bChildChainStatusOld = tempOldDIOByte.switchBit.b1;
    }

    if (bChildChainStatus
        != bChildChainStatusOld)                  //if status have change.
    {
      sCCTMP = smem.vGetChildChainStruct();
      now = time(NULL);
      printf(" now:%d\n", now);
      currenttime = localtime(&now);
      if (bChildChainStatus
          == TC_CHAIN_SIGNAL_IN_START_SUB_PAHSE) {        //get StartLock Signal
        printf("GetChildChain Start Time: %2d:%2d:%2d\n",
               currenttime->tm_hour,
               currenttime->tm_min,
               currenttime->tm_sec);
        printf("GetChildChain Start Time: %2d:%2d:%2d\n",
               currenttime->tm_hour,
               currenttime->tm_min,
               currenttime->tm_sec);
        printf("GetChildChain Start Time: %2d:%2d:%2d\n",
               currenttime->tm_hour,
               currenttime->tm_min,
               currenttime->tm_sec);
        sCCTMP.oldStartTime = sCCTMP.newStartTime;
        sCCTMP.newStartTime = now;

        lCycleTime = sCCTMP.newStartTime - sCCTMP.oldStartTime;
        if (sCCTMP.oldStartTime != 0 && lCycleTime > 0
            && lCycleTime < 300) {      //300 means max cycle for protect
          sCCTMP.iEndKeepTime = sCCTMP.newStartTime - sCCTMP.newEndTime;
          sCCTMP.iChainCycle = lCycleTime;
        } else {                                                                  //error signal cycle
          sCCTMP.iEndKeepTime = 0;
        }
      } else {                                                                    //get EndLock Signal (TC_CHAIN_SIGNAL_IN_END_SUB_PAHSE)
        printf("GetChildChain End Time: %2d:%2d:%2d\n",
               currenttime->tm_hour,
               currenttime->tm_min,
               currenttime->tm_sec);
        printf("GetChildChain End Time: %2d:%2d:%2d\n",
               currenttime->tm_hour,
               currenttime->tm_min,
               currenttime->tm_sec);
        printf("GetChildChain End Time: %2d:%2d:%2d\n",
               currenttime->tm_hour,
               currenttime->tm_min,
               currenttime->tm_sec);
        sCCTMP.oldEndTime = sCCTMP.newEndTime;
        sCCTMP.newEndTime = now;

        lCycleTime = sCCTMP.newEndTime - sCCTMP.oldEndTime;
        if (sCCTMP.oldEndTime != 0 && lCycleTime > 0
            && lCycleTime < 300) {        //300 means max cycle for protect
          sCCTMP.iStartKeepTime = sCCTMP.newEndTime - sCCTMP.newStartTime;
          sCCTMP.iChainCycle = lCycleTime;
        } else {                                                                  //error signal cycle
          sCCTMP.iStartKeepTime = 0;
        }

      }

      if (sCCTMP.iStartKeepTime != 0
          && sCCTMP.iEndKeepTime != 0) {                //Check value reasonable
        sCCTMP.bHaveReasonableChildChainSignal =
            true;                            //get right chain signal
        printf("GetChildChainbHaveReasonableChildChainSignal\n");
        printf("GetChildChainbHaveReasonableChildChainSignal\n");
        printf("GetChildChainbHaveReasonableChildChainSignal\n");
      } else {
        sCCTMP.bHaveReasonableChildChainSignal = false;
      }

      smem.vSetChildChainStruct(sCCTMP);
      iChainSignalNotChange = 0;

    } else {
      iChainSignalNotChange++;
    }

    if (iChainSignalNotChange > 600) {
      smem.vClearChildChainStruct();
    }

    return true;
  } catch (...) {}
}

//--------------------------------------------------------------------------
bool intervalTimer::vCheckSMEM(void) {
  try {
//    if( smem.vGetSimIntervalTimeChangeStatus() ) { //for VD SimDataSend
//        _it4 = smem.vReadSimIntervalTime();

//      //OTMARKPRINTF  printf("[Message] Change Sim Send Interval:%d\n", _it4.it_interval.tv_sec);

//        if ( timer_settime( _t4, 0, & _it4, NULL ) ) exit( 1 );
//        smem.vSetSimIntervalTimeChangeStatus(false);
//    }
    if (smem.vGet0FHardwareCycleChangeStatus()) {
      _it6 = smem.vGet0FHardwareCycle();

      //OTMARKPRINTF  printf("[Message] Change HWStatus Send Interval:%d\n", _it6.it_interval.tv_sec);

      if (timer_settime(_t6, 0, &_it6, NULL)) exit(1);
      smem.vSet0FHardwareCycleChangeStatus(false);
    }
    if (smem.vGet0F11CommuncationResetChangeStatus()) {
      _it7.it_value.tv_sec = 0;
      _it7.it_value.tv_nsec = 500;
      _it7.it_interval.tv_sec = 3;
      _it7.it_interval.tv_nsec = 0;
      //OTMARKPRINTF  printf("[Message] StartCommuncationReset!\n");
      if (timer_settime(_t7, 0, &_it7, NULL)) exit(1);
      smem.vSet0F11CommuncationResetChangeStatus(false);
    }
    if (smem.vGet0F16LockDBRequestStatus()) {
      _it8.it_value.tv_sec = 1;
      _it8.it_value.tv_nsec = 0;
      _it8.it_interval.tv_sec = 30;
      _it8.it_interval.tv_nsec = 0;
      //OTMARKPRINTF  printf("[Message] Start Request LockDB!\n");
      if (timer_settime(_t8, 0, &_it8, NULL)) exit(1);
      smem.vSet0F16LockDBRequestStatus(false);
    }

    return true;
  }
  catch (...) {}
}

//--------------------------------------------------------------------------
//**********************************************************
//      Read and Reset Timers for Resetting CMOS Time
//**********************************************************
//--------------------------------------------------------------------------
bool intervalTimer::TimersRead_BeforeResetCMOSTime(void) {
  try {

    pthread_mutex_lock(&intervalTimer::_ctimer_mutex);
    struct itimerspec _itZero;

    _itZero.it_value.tv_sec = 0;
    _itZero.it_value.tv_nsec = 0;
    _itZero.it_interval.tv_sec = 0;
    _itZero.it_interval.tv_nsec = 0;

//     while(smem.vGetTimerMutexCTIMER() == true) { }
//     smem.vSetTimerMutexCTIMER(true);
    /*
      if(timer_gettime(_t1,&_it1)) return false;
      if(timer_gettime(_t2,&_it2)) return false;
      if(timer_gettime(_t3,&_it3)) return false;
      if(timer_gettime(_t4,&_it4)) return false;
      if(timer_gettime(_t5,&_it5)) return false;
      if(timer_gettime(_t6,&_it6)) return false;
      if(timer_gettime(_t7,&_it7)) return false;
      if(timer_gettime(_t8,&_it8)) return false;
  */
    if (timer_gettime(_t9, &_it9)) return false;
    if (timer_gettime(_tMinchun, &_itMinchun)) return false;
    timer_settime(_t1, 0, &_itZero, NULL);
    timer_settime(_t2, 0, &_itZero, NULL);
    timer_settime(_t3, 0, &_itZero, NULL);
    timer_settime(_t4, 0, &_itZero, NULL);
    timer_settime(_t5, 0, &_itZero, NULL);
    timer_settime(_t6, 0, &_itZero, NULL);
    timer_settime(_t7, 0, &_itZero, NULL);
    timer_settime(_t8, 0, &_itZero, NULL);
    timer_settime(_t9, 0, &_itZero, NULL);
    timer_settime(_tMinchun, 0, &_itZero, NULL);
    timer_settime(_t10, 0, &_itZero, NULL);
    timer_settime(_tTrafficeLight, 0, &_itZero, NULL);

    pthread_mutex_unlock(&intervalTimer::_ctimer_mutex);

    return true;
  } catch (...) {}
}
//--------------------------------------------------------------------------
bool intervalTimer::TimersReset_AfterResetCMOSTime(void) {
  try {

    pthread_mutex_lock(&intervalTimer::_ctimer_mutex);

    for (int i = 0; i < 4; i++) {

      TimersSetting();
      /*
      if(timer_settime(_t1, 0, &_it1, NULL)) return false;
      if(timer_settime(_t2, 0, &_it2, NULL)) return false;
      if(timer_settime(_t3, 0, &_it3, NULL)) return false;
      if(timer_settime(_t4, 0, &_it4, NULL)) return false;
      if(timer_settime(_t5, 0, &_it5, NULL)) return false;
      if(timer_settime(_t6, 0, &_it6, NULL)) return false;
      if(timer_settime(_t7, 0, &_it7, NULL)) return false;
      if(timer_settime(_t8, 0, &_it8, NULL)) return false;
      */
      if (timer_settime(_t9, 0, &_it9, NULL)) return false;
      if (timer_settime(_tMinchun, 0, &_itMinchun, NULL)) return false;

//    smem.vSetTimerMutexCTIMER(false);

    }

    pthread_mutex_unlock(&intervalTimer::_ctimer_mutex);

    return true;
  } catch (...) {}
}

//-------------------------------------------------------------------------
bool intervalTimer::vCheckScreenAndDoSomething(void) {
  try {

    if (smem.GetcFace() == 0) {
      return true;
    }
    time_t NowSec = time(NULL);
//  time_t _TmpLastPress =smem.Get???

//  if( (NowSec - _TmpLastPress > 150)
    {
      //change cfase
    }
    return true;

  } catch (...) {}
}
//-------------------------------------------------------------------------
bool intervalTimer::vCheckVDUpdateDBAndDoSomething(void) {
  try {

//OT Debug 0410
//change for TC

    if (smem.vGetVDUpdateDB(888)) {
      if (smem.GetConnectCenterStatus()) {
        unsigned char data[4];
        data[2] = 0;
        data[2] = data[2] + smem.vGetVDUpdateDB(0);
        data[2] = data[2] + smem.vGetVDUpdateDB(1) * 2;
        data[2] = data[2] + smem.vGetVDUpdateDB(2) * 4;
//OT Debug 951128
        data[2] = data[2] + smem.vGetVDUpdateDB(3) * 8;

        data[0] = 0x5F;
        data[1] = 0x0A;

        data[3] = 0xFF;

        MESSAGEOK _MSG;
        _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4, false);
        _MSG.InnerOrOutWard = cInner;
        writeJob.WriteWorkByMESSAGEOUT(_MSG);

        for (int i = 0; i < 4; i++)
          smem.vSetVDUpdateDB(i, false);
      }
      smem.vSetVDUpdateDB(888,
                          false);                                          // send to center OK.

    }
    return true;
  } catch (...) {}
}
//--------------------------------------------------------------------------

bool intervalTimer::vCommuncationReset(int iTMP) {
  try {
    unsigned char data[2];
    data[0] = 0x0F;
    data[1] = 0x91;

    switch (iTMP) {
      case (0):
        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 2, true);
        _MsgOK.InnerOrOutWard = cOutWard;
        writeJob.WriteWorkByMESSAGEOUT(_MsgOK);

        digitalIO.WriteDigitalIO(0xFF);
        break;
      case (1):
        digitalIO.WriteDigitalIO(0x00);
        smem.vSetTimeInterVal(60, INVAILDVALUE);
//             smem.vSet92VDTransmitType(1, 60);
        break;

      default:
        _it7.it_value.tv_sec = 0;
        _it7.it_value.tv_nsec = 0;
        _it7.it_interval.tv_sec = 0;
        _it7.it_interval.tv_nsec = 0;
        if (timer_settime(_t7, 0, &_it7, NULL)) exit(1);

        break;
    }
  } catch (...) {}
}

//--------------------------------------------------------------------------
bool intervalTimer::vDBLockRequest(int iTMP) {
  try {
    unsigned char data[2];
    data[0] = 0x0F;
    data[1] = 0x06;
    MESSAGEOK _MsgOK;

    switch (iTMP) {
      case (0):
      case (1):
      case (2):
      case (3):
      case (4):
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 2, true);
        _MsgOK.InnerOrOutWard = cOutWard;
        writeJob.WriteWorkByMESSAGEOUT(_MsgOK);

        if (smem.GetDbOperStat() != 1) {
          _it8.it_value.tv_sec = 0;
          _it8.it_value.tv_nsec = 0;
          _it8.it_interval.tv_sec = 0;
          _it8.it_interval.tv_nsec = 0;
          if (timer_settime(_t8, 0, &_it8, NULL)) exit(1);
        }
        break;
      default:
        _it8.it_value.tv_sec = 0;
        _it8.it_value.tv_nsec = 0;
        _it8.it_interval.tv_sec = 0;
        _it8.it_interval.tv_nsec = 0;
        if (timer_settime(_t8, 0, &_it8, NULL)) exit(1);

        break;
    }
  } catch (...) {}
}

//--------------------------------------------------------------------------
bool intervalTimer::vAllDynamicToTODCount(unsigned short int siTMP) {
  try {
    _it9.it_value.tv_sec = siTMP;
    /* ot add 960802 */
    _it9.it_value.tv_nsec = 0;
    _it9.it_interval.tv_sec = 0;
    _it9.it_interval.tv_nsec = 0;

    if (timer_settime(_t9, 0, &_it9, NULL)) exit(1);

  } catch (...) {}
}
void intervalTimer::Lock_to_Set_Next_Dyn_Step(void) {
  try {
    _it9.it_value.tv_sec = 0;
    /* ot add 960802 */
    _it9.it_value.tv_nsec = 500;
    _it9.it_interval.tv_sec = 0;
    _it9.it_interval.tv_nsec = 0;

    if (timer_settime(_t9, 0, &_it9, NULL)) exit(1);

  }
  catch (...) {}
}
bool intervalTimer::vAllDynamicMinchunCount(unsigned short int siTMP) {
  try {
    _itMinchun.it_value.tv_sec = siTMP;
    /* ot add 960802 */
    _itMinchun.it_value.tv_nsec = 0;
    _itMinchun.it_interval.tv_sec = 0;
    _itMinchun.it_interval.tv_nsec = 0;

    if (timer_settime(_tMinchun, 0, &_itMinchun, NULL)) exit(1);
    if (siTMP > 0)CSTC::setMinchunDynFlag(true);
  } catch (...) {}
}

//----------------------------------------------------------
unsigned short int intervalTimer::vGetEffectTime(void) {
  try {

    //Should be mutex

    unsigned short int time_difference = 0;
    timer_gettime(_t9, &_it9);
    time_difference = (_it9.it_value.tv_sec);
    //專為銘將設計的方法，用來計算5f1c即時動態時制已經走過綠燈的時間
    return time_difference;

  } catch (...) {}
}


//--------------------------------------------------------------------------
bool intervalTimer::vRefreshWatchDog(void) {
  try {
//  usiWatchDogCount++;
//  printf("usiWatchDogCount:%d\n", usiWatchDogCount);
//  if(usiWatchDogCount >= 1) {
//    usiWatchDogCount = 0;
    digitalIO.vRefresfWatchDog0x444();
//  }

  } catch (...) {}
}

//OT970214NEWLCX405
//--------------------------------------------------------------------------
bool intervalTimer::vSendHeartBeatToLCX405(void) {
  try {

    unsigned char ucHeartBeat[22];  //OTCombo0713 SayHelloToCard
    char msg[64];

//  ++usiLCX405WatchDogCount;

//  if(usiLCX405WatchDogCount > 1) {
//    usiLCX405WatchDogCount = 0;

    ucHeartBeat[0] = 0xAA;
    ucHeartBeat[1] = 0xBB;
    ucHeartBeat[2] = 0x13;
    ucHeartBeat[3] = 0x00;
    ucHeartBeat[4] = 0x00;
    ucHeartBeat[5] = 0x00;
    ucHeartBeat[6] = 0x00;
    ucHeartBeat[7] = 0x00;
    ucHeartBeat[8] = 0x00;
    ucHeartBeat[9] = 0x00;
    ucHeartBeat[10] = 0x00;
    ucHeartBeat[11] = 0x00;
    ucHeartBeat[12] = 0x00;
    ucHeartBeat[13] = 0x12;
    ucHeartBeat[14] = 0x00;
    ucHeartBeat[15] = 0x00;
    ucHeartBeat[16] = 0x00;
    ucHeartBeat[17] = 0x00;
    ucHeartBeat[18] = 0xAA;
    ucHeartBeat[19] = 0xCC;
    ucHeartBeat[20] = 0x00;
    for (int i = 0; i < 20; i++) {
      ucHeartBeat[20] ^= ucHeartBeat[i];
    }
    writeJob.WritePhysicalOut(ucHeartBeat,
                              21,
                              DEVICETRAFFICLIGHT);  //OTCombo0713

//  sprintf(msg,"SendLightHeartBeat");
//  smem.vWriteMsgToDOM(msg);


//  }

    return 0;

  } catch (...) {}
}

//----------------------------------------------------------
void intervalTimer::vReportCurrentOperationMode_5F08_inCtimer(void) {
  try {
    unsigned short int data_length = 3;
//mallocFuck  unsigned char *data = (unsigned char *)malloc( data_length*sizeof(unsigned char) );
    unsigned char data[4];
    data[0] = 0x5F;
    data[1] = 0x08;
    data[2] = 0x00;
    data[2] = smem.vGetTC5F08Status();

    printf("[MESSAGE] Ctimer Report 5F08 Current Operation Mode to Center: %d\n",
           data[2]);

/*+++++++++++++++++*/

    MESSAGEOK _MsgOK;
    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, data_length, true);
    writeJob.WritePhysicalOut(_MsgOK.packet,
                              _MsgOK.packetLength,
                              DEVICECENTER92);

  } catch (...) {}
}



