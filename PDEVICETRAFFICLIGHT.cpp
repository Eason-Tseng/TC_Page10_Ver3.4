//---------------------------------------------------------------------------
#include "PDEVICETRAFFICLIGHT.h"
#include "SMEM.h"
#include "WRITEJOB.h"

#include "CDataToMessageOK.h"

#include "CSTC.h"

#include "CSTORINI.h"

#include <stdio.h>
//---------------------------------------------------------------------------
PDEVICETRAFFICLIGHT::PDEVICETRAFFICLIGHT(void)
{
  //OT20110930
  iAskW77E58CounterReturn = 0;
}
//---------------------------------------------------------------------------
PDEVICETRAFFICLIGHT::~PDEVICETRAFFICLIGHT(void)
{
}
//---------------------------------------------------------------------------
bool PDEVICETRAFFICLIGHT::DoWorkViaPDevice(MESSAGEOK message)
{
try {
    char cTMP[128];
    unsigned char ucRet;
    switch (message.packet[2]) {
            case 0x46:                                                          //down board(LCX-405) autoruning, last step
              smem.vSetUCData(TC_Redcount_Display_Enable, 1);
              ucRet = smem.vGetUCData(TC_Redcount_Booting_Display_Enable);           // this machine have autorun 8051 IC
              if(ucRet == 1) {
                smem.vSetUCData(TC_Redcount_Booting_Display_Enable, 0);
              }

              smem.vWriteMsgToDOM("From AutoRun To TC");
              stc.vGetLCX405ControlPower();
              smem.vSetBOOLData(TC_SignalConflictError, false);

              smem.vSetStopSend0x22(false);  //means have autorun 51

              oStorINI.vSaveIntToINI("LightCtlBoard", "AutoRun51", 1);

              smem.SetRequestKeypad(1);

              //OT980420, let redcount display open.
//move to stc.vGetLCX405ControlPower()              smem.vSetUCData(TC_Redcount_Display_Enable, 1);
            break;

            case 0x45:                                                          //GreenColfiect  //Eason_Ver3.4
              sprintf(cTMP, "Get GreenColfiect:%02X,%02X,%02X", message.packet[3], message.packet[4], message.packet[5]);
              smem.vWriteMsgToDOM(cTMP);
              char iTMP[128];
              GreenConflictcode(message.packet[3],message.packet[4],message.packet[5]);
              sprintf(iTMP,"%02X%02X%02X",returnnum[0],returnnum[1],returnnum[2]);
              if(message.packet[3]==0x00 && message.packet[4]==0x00 && message.packet[5]==0x00){}
              else
              {
                smem.vWriteGreenConflictToDOM(iTMP);
                smem.vSetBOOLData(TC_SignalConflictError, true);
              }
              smem.vSetSignalConflictErrorVar(message.packet[3], message.packet[4], message.packet[5]);

              //OT980420, let redcount display close.

printf("Lock_to_Set_Control_Strategy by TRAFFICELIGHT!!!\n");
printf("Lock_to_Set_Control_Strategy by TRAFFICELIGHT!!!\n");
printf("Lock_to_Set_Control_Strategy by TRAFFICELIGHT!!!\n");

              stc.Lock_to_Set_Control_Strategy(STRATEGY_FLASH);
              smem.vSetUCData(TC_Redcount_Display_Enable, 0);
            break;

            case 0x48:
              smem.vTrafficLightAckAdd();
            break;

            case 0x44:                                                          //Error Happen, not send WDT.
              smem.vWriteMsgToDOM("From AutoRun To TC, No WDT, do nothing");
            break;

            case 0x61:                                                          //Get W77E58 Ver
              smem.vSetW77E58FWVer(0, message.packet[3]);
              smem.vSetW77E58FWVer(1, message.packet[4]);
              smem.vSetW77E58FWVer(2, message.packet[5]);

              if(iAskW77E58CounterReturn == 2) {
                oStorINI.vSaveIntToINI("LightCtlBoard", "FWVerYear", message.packet[3]);
                oStorINI.vSaveIntToINI("LightCtlBoard", "FWVerMonth", message.packet[4]);
                oStorINI.vSaveIntToINI("LightCtlBoard", "FWVerDay", message.packet[5]);

                bzero(cTMP, 128);
                sprintf(cTMP, "Ask, W77E58 FW Ver:%02d-%02d-%02d", message.packet[3], message.packet[4], message.packet[5]);
                smem.vWriteMsgToDOM(cTMP);
              }
              iAskW77E58CounterReturn++;

            break;

            case 0x62:                                                          //When booting, Get W77E58 Ver
              smem.vSetW77E58FWVer(0, message.packet[3]);
              smem.vSetW77E58FWVer(1, message.packet[4]);
              smem.vSetW77E58FWVer(2, message.packet[5]);

              bzero(cTMP, 128);
              sprintf(cTMP, "TrafficBoard W77E58 Booting, FW Ver:%02d-%02d-%02d", message.packet[3], message.packet[4], message.packet[5]);
              smem.vWriteMsgToDOM(cTMP);
            break;

            case 0x63:                                                          //booting WDT, Get W77E58 Ver
              smem.vSetW77E58FWVer(0, message.packet[3]);
              smem.vSetW77E58FWVer(1, message.packet[4]);
              smem.vSetW77E58FWVer(2, message.packet[5]);
              bzero(cTMP, 128);
              sprintf(cTMP, "TrafficBoard W77E58 WDT Start, FW Ver:%02d-%02d-%02d", message.packet[3], message.packet[4], message.packet[5]);

              smem.vWriteMsgToDOM(cTMP);
            break;

            case 0x15:        //GreenConflict 綠衝突
                BRTGreenConflict(message.packet[3],message.packet[4],message.packet[5],message.packet[2]);
            break;

            default:
//too much              smem.vWriteMsgToDOM("W77E58 Get unknow msg");
            break;
    }
    smem.vLogTrafficLightTime();
    return true;

  } catch (...) {}
}
//---------------------------------------------------------------------------
void PDEVICETRAFFICLIGHT::BRTGreenConflict(BYTE GC0,BYTE GC1, BYTE GC2, BYTE CMD)
{
try{
    char cTMP[128];
    unsigned char cardnumber = 0x00;  // BRT 燈卡編號
    unsigned char LEDnumber = 0x00;   // NO.1 左綠 NO.2 綠  NO.3 直綠

    GC_DATA0.DBit=GC0;      GC_DATA1.DBit=GC1;      GC_DATA2.DBit=GC2;
    sprintf(cTMP, "Get GreenConflict:%02X,%02X,%02X,CMD:%02X", GC0, GC1, GC2, CMD);
    smem.vWriteMsgToDOM(cTMP);
    printf("%s\n", cTMP);
    smem.vSetBOOLData(TC_SignalConflictError, true);

    if(GC_DATA0.switchBit.b1==0 || GC_DATA0.switchBit.b2==0 || GC_DATA0.switchBit.b3==0){
        cardnumber = 0x01;
    }else if(GC_DATA0.switchBit.b4==0 || GC_DATA0.switchBit.b5==0 || GC_DATA0.switchBit.b6==0){
        cardnumber = 0x02;
    }else if(GC_DATA0.switchBit.b7==0 || GC_DATA0.switchBit.b8==0 || GC_DATA1.switchBit.b1==0){
        cardnumber = 0x03;
    }else if(GC_DATA1.switchBit.b2==0 || GC_DATA1.switchBit.b3==0 || GC_DATA1.switchBit.b4==0){
        cardnumber = 0x04;
    }else if(GC_DATA1.switchBit.b5==0 || GC_DATA1.switchBit.b6==0 || GC_DATA1.switchBit.b7==0){
        cardnumber = 0X05;
    }else if(GC_DATA1.switchBit.b8==0 || GC_DATA2.switchBit.b7==0 || GC_DATA2.switchBit.b8==0){
        cardnumber = 0x06;
    }

    if(GC_DATA0.switchBit.b1==0 || GC_DATA0.switchBit.b4==0 || GC_DATA0.switchBit.b7==0 || GC_DATA1.switchBit.b2==0 || GC_DATA1.switchBit.b5==0 || GC_DATA1.switchBit.b8==0){
        LEDnumber = 0x01;
    }else if(GC_DATA0.switchBit.b2==0 || GC_DATA0.switchBit.b5==0 || GC_DATA0.switchBit.b8==0 || GC_DATA1.switchBit.b3==0 || GC_DATA1.switchBit.b6==0 || GC_DATA2.switchBit.b7==0){
        LEDnumber = 0x02;
    }else if(GC_DATA0.switchBit.b3==0 || GC_DATA0.switchBit.b6==0 || GC_DATA1.switchBit.b1==0 || GC_DATA1.switchBit.b4==0 || GC_DATA1.switchBit.b7==0 || GC_DATA2.switchBit.b8==0){
        LEDnumber = 0x03;
    }

    smem.vSetSignalConflictErrorVar(cardnumber, LEDnumber, 0x00);

    stc.Lock_to_Set_Control_Strategy(STRATEGY_FLASH);
    //printf("------------ BRT GreenConflict ------------\n");
    //printf("------------ BRT GreenConflict ------------\n");
    //printf("------------ BRT GreenConflict ------------\n");
    smem.vSetUCData(TC_Redcount_Display_Enable, 0);
} catch (...) {}
}
//-------------------Eason_Ver3.4--------------------------------------------------------
void PDEVICETRAFFICLIGHT::GreenConflictcode(BYTE GC0,BYTE GC1, BYTE GC2) //for 綠衝突歷史紀錄頁面
{
  try
  {
    for (int i=0;i<3;i++) returnnum[i] = 0x00;
    int i = 0;
    bool Switch = true;
    if(GC0-0x80 >= 0 && Switch) {returnnum[i] = 0x32; GC0 = GC0-0x80;   (i>=2) ? (Switch = false) : i++;} //第三燈卡直行
    if(GC0-0x40 >= 0 && Switch) {returnnum[i] = 0x31; GC0 = GC0-0x40;   (i>=2) ? (Switch = false) : i++;} //第三燈卡左轉
    if(GC0-0x20 >= 0 && Switch) {returnnum[i] = 0x23; GC0 = GC0-0x20;   (i>=2) ? (Switch = false) : i++;} //第二燈卡圓綠
    if(GC0-0x10 >= 0 && Switch) {returnnum[i] = 0x22; GC0 = GC0-0x10;   (i>=2) ? (Switch = false) : i++;} //第二燈卡直行
    if(GC0-0x08 >= 0 && Switch) {returnnum[i] = 0x21; GC0 = GC0-0x08;   (i>=2) ? (Switch = false) : i++;} //第二燈卡左轉
    if(GC0-0x04 >= 0 && Switch) {returnnum[i] = 0x13; GC0 = GC0-0x04;   (i>=2) ? (Switch = false) : i++;} //第一燈卡圓綠
    if(GC0-0x02 >= 0 && Switch) {returnnum[i] = 0x12; GC0 = GC0-0x02;   (i>=2) ? (Switch = false) : i++;} //第一燈卡直行
    if(GC0-0x01 == 0 && Switch) {returnnum[i] = 0x11; GC0 = GC0-0x01;   (i>=2) ? (Switch = false) : i++;} //第一燈卡左轉
    if(GC1-0x80 >= 0 && Switch) {returnnum[i] = 0x61; GC1 = GC1-0x80;   (i>=2) ? (Switch = false) : i++;} //第六燈卡左轉
    if(GC1-0x40 >= 0 && Switch) {returnnum[i] = 0x53; GC1 = GC1-0x40;   (i>=2) ? (Switch = false) : i++;} //第五燈卡圓綠
    if(GC1-0x20 >= 0 && Switch) {returnnum[i] = 0x52; GC1 = GC1-0x20;   (i>=2) ? (Switch = false) : i++;} //第五燈卡直行
    if(GC1-0x10 >= 0 && Switch) {returnnum[i] = 0x51; GC1 = GC1-0x10;   (i>=2) ? (Switch = false) : i++;} //第五燈卡左轉
    if(GC1-0x08 >= 0 && Switch) {returnnum[i] = 0x43; GC1 = GC1-0x08;   (i>=2) ? (Switch = false) : i++;} //第四燈卡圓綠
    if(GC1-0x04 >= 0 && Switch) {returnnum[i] = 0x42; GC1 = GC1-0x04;   (i>=2) ? (Switch = false) : i++;} //第四燈卡直行
    if(GC1-0x02 >= 0 && Switch) {returnnum[i] = 0x41; GC1 = GC1-0x02;   (i>=2) ? (Switch = false) : i++;} //第四燈卡左轉
    if(GC1-0x01 == 0 && Switch) {returnnum[i] = 0x33; GC1 = GC1-0x01;   (i>=2) ? (Switch = false) : i++;} //第三燈卡圓綠
    if(GC2-0x02 >= 0 && Switch) {returnnum[i] = 0x63; GC2 = GC2-0x02;   (i>=2) ? (Switch = false) : i++;} //第六燈卡圓綠
    if(GC2-0x01 == 0 && Switch) {returnnum[i] = 0x62; GC2 = GC2-0x01;   (i>=2) ? (Switch = false) : i++;} //第六燈卡直行
    
    for (int i = 0; i < 3; i++)
    {
      printf("point%d = %02X\n",i,returnnum[i]);  
    }
    
  } catch (...) {}
}