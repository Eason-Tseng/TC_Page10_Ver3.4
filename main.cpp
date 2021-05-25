//---------------------------------------------------------------------------
#pragma hdrstop

#include "var.h"
#include "SMEM.h"
#include "PARSETRAFFIC.h"
#include "PARSERTMS.h"
#include "PARSENMEA.h"
#include "PARSESS.h"

#include "READJOB.h"
#include "WRITEJOB.h"

#include "LCD240x128.h"
#include "DIGITALIO.h"

#include "CTIMER.h"
#include "CDataToMessageOK.h"

#include "SCREENLogo.h"

#include "STORAGE.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "CSTC.h"
#include "CSTORINI.h"

void SendRequestToKeypad(void) {
    try {

        MESSAGEOK messageOut;
        messageOut.protocol = PROTOCOLKEYPAD;
        messageOut.packetLength = 6;
        messageOut.cksStatus = true;
        messageOut.success = true;
        messageOut.ReadorWrite = cWRITE;

        messageOut.packet[0] = 0xAA;
        messageOut.packet[1] = 0xBB;
        messageOut.packet[2] = 0x03;
        messageOut.packet[3] = 0xAA;
        messageOut.packet[4] = 0xCC;
        messageOut.packet[5] = 0;
        for (int i = 0; i < 5; i++) messageOut.packet[5] ^= messageOut.packet[i];

        writeJob.WriteWorkByMESSAGEOUT(messageOut);

    } catch (...) {}
}

//---------------------------------------------------------------------------
#pragma argsused

int main(int argc, char *argv[]) {
    try {
        bool notexit = true;                                                          //while loop ???????}
        unsigned char ucTmp;

        int iFWYearTmp;
        int iFWMonthTmp;
        int iFWDayTmp;
        char cTMP[128];
        printf("hello hello hello hello\n");
        printf("hello hello hello hello\n");
        printf("hello hello hello hello\n");

        int rec = -1;                                                                 //Select Result
        fd_set readfs;                                                              //File Descriptor Set
        int maxport = 0, tempmax = 0;                                                    //maxnum file desciptor used
        struct timeval timeout;                                                     //?????Y?z??select???S???????i??,?N??timeout?n?@??????
        int readSelectLength = 0;                                                     //?P?_??????????

        PARSETRAFFIC parseAABB;                                                     //???R????AABB?}?YAACC??????????
        PARSERTMS parseRtms;                                                        //???RRTMS??????
        PARSESS parseSS;                                                            //???RSmartSensor??????
        PARSENMEA parseNMEA;
        tsUDPMappingLCN _sUDPMappingTMP;
        tsUDPInfo _sUDPMappingInfoTMP;

        int iCom2Type;
        iCom2Type = smem.vGetINTData(Com2_TYPE);

        //remove tc reset file
        system("rm -rf /tmp/resetTCmyself.txt");
        system("sync");

        int iTmp;
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase0_0", 0);
        smem.vSetVDPhaseMapTable(0, 0, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase0_1", 0);
        smem.vSetVDPhaseMapTable(0, 1, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase0_2", 0);
        smem.vSetVDPhaseMapTable(0, 2, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase0_3", 0);
        smem.vSetVDPhaseMapTable(0, 3, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase0_4", 0);
        smem.vSetVDPhaseMapTable(0, 4, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase0_5", 0);
        smem.vSetVDPhaseMapTable(0, 5, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase0_6", 0);
        smem.vSetVDPhaseMapTable(0, 6, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase0_7", 0);
        smem.vSetVDPhaseMapTable(0, 7, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase1_0", 0);
        smem.vSetVDPhaseMapTable(1, 0, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase1_1", 0);
        smem.vSetVDPhaseMapTable(1, 1, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase1_2", 0);
        smem.vSetVDPhaseMapTable(1, 2, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase1_3", 0);
        smem.vSetVDPhaseMapTable(1, 3, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase1_4", 0);
        smem.vSetVDPhaseMapTable(1, 4, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase1_5", 0);
        smem.vSetVDPhaseMapTable(1, 5, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase1_6", 0);
        smem.vSetVDPhaseMapTable(1, 6, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase1_7", 0);
        smem.vSetVDPhaseMapTable(1, 7, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase2_0", 0);
        smem.vSetVDPhaseMapTable(2, 0, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase2_1", 0);
        smem.vSetVDPhaseMapTable(2, 1, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase2_2", 0);
        smem.vSetVDPhaseMapTable(2, 2, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase2_3", 0);
        smem.vSetVDPhaseMapTable(2, 3, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase2_4", 0);
        smem.vSetVDPhaseMapTable(2, 4, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase2_5", 0);
        smem.vSetVDPhaseMapTable(2, 5, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase2_6", 0);
        smem.vSetVDPhaseMapTable(2, 6, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase2_7", 0);
        smem.vSetVDPhaseMapTable(2, 7, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase3_0", 0);
        smem.vSetVDPhaseMapTable(3, 0, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase3_1", 0);
        smem.vSetVDPhaseMapTable(3, 1, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase3_2", 0);
        smem.vSetVDPhaseMapTable(3, 2, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase3_3", 0);
        smem.vSetVDPhaseMapTable(3, 3, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase3_4", 0);
        smem.vSetVDPhaseMapTable(3, 4, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase3_5", 0);
        smem.vSetVDPhaseMapTable(3, 5, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase3_6", 0);
        smem.vSetVDPhaseMapTable(3, 6, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase3_7", 0);
        smem.vSetVDPhaseMapTable(3, 7, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase4_0", 0);
        smem.vSetVDPhaseMapTable(4, 0, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase4_1", 0);
        smem.vSetVDPhaseMapTable(4, 1, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase4_2", 0);
        smem.vSetVDPhaseMapTable(4, 2, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase4_3", 0);
        smem.vSetVDPhaseMapTable(4, 3, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase4_4", 0);
        smem.vSetVDPhaseMapTable(4, 4, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase4_5", 0);
        smem.vSetVDPhaseMapTable(4, 5, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase4_6", 0);
        smem.vSetVDPhaseMapTable(4, 6, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase4_7", 0);
        smem.vSetVDPhaseMapTable(4, 7, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase5_0", 0);
        smem.vSetVDPhaseMapTable(5, 0, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase5_1", 0);
        smem.vSetVDPhaseMapTable(5, 1, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase5_2", 0);
        smem.vSetVDPhaseMapTable(5, 2, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase5_3", 0);
        smem.vSetVDPhaseMapTable(5, 3, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase5_4", 0);
        smem.vSetVDPhaseMapTable(5, 4, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase5_5", 0);
        smem.vSetVDPhaseMapTable(5, 5, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase5_6", 0);
        smem.vSetVDPhaseMapTable(5, 6, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase5_7", 0);
        smem.vSetVDPhaseMapTable(5, 7, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase6_0", 0);
        smem.vSetVDPhaseMapTable(6, 0, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase6_1", 0);
        smem.vSetVDPhaseMapTable(6, 1, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase6_2", 0);
        smem.vSetVDPhaseMapTable(6, 2, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase6_3", 0);
        smem.vSetVDPhaseMapTable(6, 3, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase6_4", 0);
        smem.vSetVDPhaseMapTable(6, 4, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase6_5", 0);
        smem.vSetVDPhaseMapTable(6, 5, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase6_6", 0);
        smem.vSetVDPhaseMapTable(6, 6, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase6_7", 0);
        smem.vSetVDPhaseMapTable(6, 7, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase7_0", 0);
        smem.vSetVDPhaseMapTable(7, 0, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase7_1", 0);
        smem.vSetVDPhaseMapTable(7, 1, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase7_2", 0);
        smem.vSetVDPhaseMapTable(7, 2, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase7_3", 0);
        smem.vSetVDPhaseMapTable(7, 3, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase7_4", 0);
        smem.vSetVDPhaseMapTable(7, 4, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase7_5", 0);
        smem.vSetVDPhaseMapTable(7, 5, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase7_6", 0);
        smem.vSetVDPhaseMapTable(7, 6, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase7_7", 0);
        smem.vSetVDPhaseMapTable(7, 7, iTmp);

//OT20140211
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan00", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(0, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan01", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(1, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan02", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(2, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan03", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(3, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan04", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(4, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan05", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(5, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan06", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(6, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan07", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(7, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan08", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(8, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan09", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(9, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan10", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(10, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan11", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(11, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan12", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(12, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan13", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(13, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan14", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(14, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan15", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(15, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan16", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(16, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan17", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(17, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan18", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(18, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan19", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(19, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan20", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(20, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan21", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(21, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan22", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(22, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan23", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(23, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan24", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(24, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan25", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(25, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan26", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(26, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan27", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(27, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan28", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(28, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan29", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(29, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan30", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(30, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan31", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(31, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan32", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(32, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan33", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(33, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan34", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(34, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan35", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(35, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan36", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(36, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan37", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(37, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan38", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(38, true); }
        iTmp = oStorINI.vLoadIntFromINI("TODPed", "plan39", 0);
        if (iTmp > 0) { smem.vSetPlanForceTOD(39, true); }


//OT990401 FIX BUG
//OT20110706
        smem.vSetStopSend0x22(true);  //means have autorun 51

        iTmp = oStorINI.vLoadIntFromINI("VDTrigger", "PhaseTriggerSwitch0", 0);
        smem.vSetVDPhaseTriggerSwitch(0, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VDTrigger", "PhaseTriggerSwitch1", 0);
        smem.vSetVDPhaseTriggerSwitch(1, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VDTrigger", "PhaseTriggerSwitch2", 0);
        smem.vSetVDPhaseTriggerSwitch(2, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VDTrigger", "PhaseTriggerSwitch3", 0);
        smem.vSetVDPhaseTriggerSwitch(3, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VDTrigger", "PhaseTriggerSwitch4", 0);
        smem.vSetVDPhaseTriggerSwitch(4, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VDTrigger", "PhaseTriggerSwitch5", 0);
        smem.vSetVDPhaseTriggerSwitch(5, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VDTrigger", "PhaseTriggerSwitch6", 0);
        smem.vSetVDPhaseTriggerSwitch(6, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VDTrigger", "PhaseTriggerSwitch7", 0);
        smem.vSetVDPhaseTriggerSwitch(7, iTmp);

        iTmp = oStorINI.vLoadIntFromINI("TC", "SendCCTActArwenProtocal", 0);
        smem.vSetUCData(TC_CCT_Send_ActuateArwen_Protocal, iTmp);

//OT990419
        iTmp = oStorINI.vLoadIntFromINI("RedCount", "DynShowRedCountForAct", 0);
        smem.vSetUCData(DynShowRedCountForAct, iTmp);

//OT20110607
        iTmp = oStorINI.vLoadIntFromINI("TC", "SendCCTPhaseCycleProtocalForCenter", 1);
        smem.vSetUCData(TC_CCT_SendCCTPhaseCycleProtocalForCenter, iTmp);

//OT20110624
        iTmp = oStorINI.vLoadIntFromINI("RedCount", "RedCountRepeatCounter", 3);
        smem.vSetUCData(CSTC_RedcountRepeatCount, iTmp);

//OT20111128
        iTmp = oStorINI.vLoadIntFromINI("TC", "MachineLocation", 0);
        smem.vSetUCData(TC_CCT_MachineLocation, iTmp);

//OT20111201
        iTmp = oStorINI.vLoadIntFromINI("TC", "railwayChainBanSupphase2", 255);
        if (iTmp == 255) { //copy from railwayChainBanSupphase
            iTmp = smem.vGetUCData(TC_TrainComingBanSubphase);
            smem.vSetUCData(TC_TrainComingBanSubphase2, iTmp);
            oStorINI.vSaveIntToINI("TC", "railwayChainBanSupphase2", iTmp);
        } else {
            smem.vSetUCData(TC_TrainComingBanSubphase2, iTmp);
        }

        ucTmp = smem.vGetUCData(TC_ReverseLane_Manual_Enable_Status);
        printf("smem.vGetUCData(TC_ReverseLane_Manual_Enable_Status):%d\n", ucTmp);
        if (1 == ucTmp) {
            if (smem.revLaneManualPort.SetConnDevice(DEVICEREVMANUALPAD))
                if ((tempmax = smem.revLaneManualPort.OpenRs232Port("/dev/ttyS0", 2400, true)) > 0) {
                    if (tempmax > maxport) { maxport = tempmax; }
                    printf("open revLaneManualPort-/dev/ttyS0 (fdValue:%d) Success!!\n", tempmax);
                } else printf("open revLaneManualPort Fail!!\n");

        } else {
            if (smem.centerPort.SetConnDevice(DEVICECENTER92))
                if ((tempmax = smem.centerPort.OpenRs232Port("/dev/ttyS0", 9600, false)) > 0) {
                    if (tempmax > maxport) { maxport = tempmax; }
                    printf("open CenterPort-/dev/ttyS0 (fdValue:%d) Success!!\n", tempmax);
                } else printf("open CenterPort Fail!!\n");
        }


        if (iCom2Type == Com2IsTesterPort) {
            printf("Com2 is TesterPort Port!\n");
        } else if (iCom2Type == Com2IsGPSPort || iCom2Type == Com2IsHOLUXGPSPort) {     
            printf("Com2 is GPSPort Port!\n");
            if (smem.NMEAPort.SetConnDevice(DEVICEGPS)) {
                if (iCom2Type == Com2IsGPSPort)
                    tempmax = smem.NMEAPort.OpenRs232Port("/dev/ttyS1", 4800, false);
                else if (iCom2Type == Com2IsHOLUXGPSPort)
                    tempmax = smem.NMEAPort.OpenRs232Port("/dev/ttyS1", 9600, false);
                if (tempmax > 0) {
                    if (tempmax > maxport) maxport = tempmax;
                    printf("open NMEAPort-/dev/ttyS1 (fdValue:%d) Success!!\n", tempmax);
                } else printf("open NMEAPort Fail!!\n");
            }
        } else if (iCom2Type == Com2IsPassingPort) {
            printf("Com2 is Passing Port!\n");
            if (smem.passingPort.SetConnDevice(DEVICE87CMS))                          //ForDongKungSF
                if ((tempmax = smem.passingPort.OpenRs232Port("/dev/ttyS1", 9600, false)) > 0) {
                    if (tempmax > maxport) maxport = tempmax;
                    printf("open passingPort-/dev/ttyS1 (fdValue:%d) Success!!\n", tempmax);
                } else printf("open passingPort Fail!!\n");
        } else if (iCom2Type == Com2IsRedCount) {
            if (smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVer94 ||
                smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVer94v2) {
                if (smem.redCountPort.SetConnDevice(DEVICEREDCOUNTVER94))
                    if ((tempmax = smem.redCountPort.OpenRs232Port("/dev/ttyS1", 2400, true)) > 0) {
                        if (tempmax > maxport) maxport = tempmax;
                        printf("open RedCountPortHC-/dev/ttyS1 (fdValue:%d) Success!!\n", tempmax);
                    } else printf("open RedCountPortHC Fail!!\n");
            } else if (smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVerHK) {
                if (smem.redCountPort.SetConnDevice(DEVICEREDCOUNTVERHK))
                    if ((tempmax = smem.redCountPort.OpenRs232Port("/dev/ttyS1", 2400, true)) > 0) {
                        if (tempmax > maxport) maxport = tempmax;
                        printf("open RedCountPortHK-/dev/ttyS1 (fdValue:%d) Success!!\n", tempmax);
                    } else printf("open RedCountPortHK Fail!!\n");
            } else if (smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVerCCT97) {
                if (smem.redCountPort.SetConnDevice(DEVICEREDCOUNTVERCCT97))
//Change        if ((tempmax=smem.redCountPort.OpenRs232Port("/dev/ttyS1", 9600, false))>0)  {
                    if ((tempmax = smem.redCountPort.OpenRs232Port("/dev/ttyS1", 2400, true)) > 0) {
                        if (tempmax > maxport) maxport = tempmax;
                        printf("open RedCountPortCCT97-/dev/ttyS1 (fdValue:%d) Success!!\n", tempmax);
                    } else printf("open RedCountPortCCT97 Fail!!\n");
            } else if (smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVerTaichung) {
                if(smem.TaichungRedCountPort.SetConnDevice(DEVICEREDCOUNTVERTAICHUNG))
                    if((tempmax = smem.TaichungRedCountPort.OpenRs232Port("/dev/ttyS1",9600,false)) > 0) {
                        if(tempmax > maxport) maxport = tempmax;
                        printf("open RedCountPortVerTaichung-/dev/ttyS1 (fdValue:%d) Success!!\n", tempmax);
                    } else printf("open RedCountPortVerTaichung Fail!!\n");
            }

        } else if (iCom2Type == Com2IsTainanPeopleLight) {
            printf("Com2 is TainanPeopleLight Port!\n");
            if (smem.com2Port.SetConnDevice(DEVICETAINANPEOPLELIGHT))                 //ForTainan
                if ((tempmax = smem.com2Port.OpenRs232Port("/dev/ttyS1", 9600, false)) > 0) {
                    if (tempmax > maxport) maxport = tempmax;
                    printf("open TainanPeopleLightPort-/dev/ttyS1 (fdValue:%d) Success!!\n", tempmax);
                } else printf("open TainanPeopleLightPort Fail!!\n");
        }


        //?}??KEYPAD?q?T??
        if (smem.keypadPort.SetConnDevice(DEVICEKEYPAD))
            if ((tempmax = smem.keypadPort.OpenRs232Port("/dev/ttyS2", 9600, false)) > 0) {
                if (tempmax > maxport) maxport = tempmax;
                printf("open KeypadPort-/dev/ttyS2 (fdValue:%d) Success!!\n", tempmax);
            } else printf("open KeypadPort Fail!!\n");

        //?}??Traffic Light
        if (smem.lightPort.SetConnDevice(DEVICETRAFFICLIGHT))
            if ((tempmax = smem.lightPort.OpenRs232Port("/dev/ttyS3", 9600, false)) > 0) {
                if (tempmax > maxport) maxport = tempmax;
                printf("open TrafficLightPort-/dev/ttyS3 (fdValue:%d) Success!!\n", tempmax);
            } else printf("open LightPort Fail!!\n");

//OTSS++
        //Open SmartPort
        //OT1000211 disable
        /*
    if (smem.ssPort.SetConnDevice(DEVICESS))
    if ((tempmax=smem.ssPort.OpenRs232Port("/dev/ttyN1",115200,false))>0)  {
         if (tempmax>maxport)  maxport=tempmax;
         printf("open SmartSensorPort-/dev/ttyN1 (fdValue:%d) Success!!\n",tempmax);
    } else printf("open SSPort Fail!!\n");
    */
//OTSS--


        //???O????

//OT Debug 0523
        if (iCom2Type != Com2IsRedCount) {
            if (smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVer94) {
                if (smem.redCountPort.SetConnDevice(DEVICEREDCOUNTVER94))
                    if ((tempmax = smem.redCountPort.OpenRs232Port("/dev/ttyN0", 2400, true)) > 0) {
                        if (tempmax > maxport) maxport = tempmax;
                        printf("open RedCountPortHC-/dev/ttyN0 (fdValue:%d) Success!!\n", tempmax);
                    } else printf("open RedCountPortHC Fail!!\n");
            } else if (smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVerHK) {
                if (smem.redCountPort.SetConnDevice(DEVICEREDCOUNTVERHK))
                    if ((tempmax = smem.redCountPort.OpenRs232Port("/dev/ttyN0", 2400, true)) > 0) {
                        if (tempmax > maxport) maxport = tempmax;
                        printf("open RedCountPortHK-/dev/ttyN0 (fdValue:%d) Success!!\n", tempmax);
                    } else printf("open RedCountPortHK Fail!!\n");
            }
        }

        int localPort1 = smem.GetLocalIP1(5);
        int distPort0 = smem.GetdistIp0(5);
        int distPort = smem.GetDistIP(5);

        char localIP1[20] = {0}, distIP0[20] = {0}, distIP[20] = {0};
        char tmp[5] = {0};

        for (int ip = 1; ip <= 4; ip++) {
            sprintf(tmp, "%d", smem.GetLocalIP1(ip));
            strcat(localIP1, tmp);
            if (ip < 4) strcat(localIP1, ".");
            sprintf(tmp, "%d", smem.GetdistIp0(ip));
            strcat(distIP0, tmp);
            if (ip < 4) strcat(distIP0, ".");
            sprintf(tmp, "%d", smem.GetDistIP(ip));
            strcat(distIP, tmp);
            if (ip < 4) strcat(distIP, ".");
        }
        // sprintf(distIP0, "192.168.11.173");
        // distPort0 = 7002;

        printf("\nIP1 : %s:%d\n", localIP1, localPort1);
        printf("DISTIP0 : %s:%d\n", distIP0, distPort0);
        printf("DISTIP : %s:%d\n\n", distIP, distPort);


        //?}??UDP SOCKET ?q?T??
        //?}???P???q?????????q?T??UDP SOCKET

// sprintf(localIP1,"192.168.40.247");
// localPort1=7003;
        if (smem.centerSocket.SetConnDevice(DEVICECENTER92))
            if ((tempmax = smem.centerSocket.OpenUdpSocket(localIP1, localPort1, distIP0, distPort0)) > 0) {
                if (tempmax > maxport) maxport = tempmax;
                printf("open CenterSocket-%s:%d (fdValue:%d) Success!!\n", localIP1, localPort1, tempmax);
            } else printf("open CenterSocket-%s:%d Fail!!\n", localIP1, localPort1);


///////////////////////////////////////////////////////////////////////////////////////////////////////////
        /* system("ifconfig eth1 192.168.2.1");//for VSX-6156
     if (smem.tcp_test_socket.SetConnDevice(1000))
    if ((tempmax=smem.tcp_test_socket.OpenTCPSocket("192.168.2.1",7002,"192.168.2.2",7002))>0)  {
         if (tempmax>maxport)  maxport=tempmax;
         printf("open tcp_test_socket-%s:%d (fdValue:%d) Success!!\n","192.168.2.1",7002,tempmax);
    } else printf("open tcp_test_socket-%s:%d Fail!!\n","192.168.2.1",7002);
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////


        if (smem.testerSocket.SetConnDevice(DEVICETESTER92))
            if ((tempmax = smem.testerSocket.OpenUdpSocket(localIP1, localPort1 + 64, distIP, distPort)) > 0) {
                if (tempmax > maxport) maxport = tempmax;
                printf("open TesterSocket-%s:%d (fdValue:%d) Success!!\n", localIP1, localPort1 + 64, tempmax);
            } else printf("open TesterSocket-%s:%d Fail!!\n", localIP1, localPort1 + 64);

        //???f???????????AI/O UDP SOCKET

/* OT ADD 950927 */
        _sUDPMappingTMP = smem.vGetCCJDynCtlIPData();
        if (_sUDPMappingTMP.iListenPort != 0) {
            if (smem.CCJDynCtlSocket.SetConnDevice(DEVICECCJDYNCTL))
                if ((tempmax = smem.CCJDynCtlSocket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                                  _sUDPMappingTMP.cRemoteIP,
                                                                  _sUDPMappingTMP.iSendPort)) > 0) {
                    if (tempmax > maxport) maxport = tempmax;
                    printf("open CCJDynCtlSocket-%s:%d (fdValue:%d) Success!!\n", _sUDPMappingTMP.cLocalIP,
                           _sUDPMappingTMP.iListenPort, tempmax);
                } else
                    printf("open CCJDynCtlSocket-%s:%d %dFail!!\n", _sUDPMappingTMP.cLocalIP,
                           _sUDPMappingTMP.iListenPort, tempmax);
        }

//OT ADD 990326
        _sUDPMappingTMP = smem.vGetCCJDynCtl2IPData();
        if (_sUDPMappingTMP.iListenPort != 0) {
            if (smem.CCJDynCtl2Socket.SetConnDevice(DEVICECCJDYNCTL))
                if ((tempmax = smem.CCJDynCtl2Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP,
                                                                   _sUDPMappingTMP.iListenPort,
                                                                   _sUDPMappingTMP.cRemoteIP,
                                                                   _sUDPMappingTMP.iSendPort)) > 0) {
                    if (tempmax > maxport) maxport = tempmax;
                    printf("open CCJDynCtl2Socket-%s:%d (fdValue:%d) Success!!\n", _sUDPMappingTMP.cLocalIP,
                           _sUDPMappingTMP.iListenPort, tempmax);
                } else
                    printf("open CCJDynCtl2Socket-%s:%d %dFail!!\n", _sUDPMappingTMP.cLocalIP,
                           _sUDPMappingTMP.iListenPort, tempmax);
        }

        //OT990618
        iTmp = oStorINI.vLoadIntFromINI("revSyncSocket", "revSyncEnable", 0);
        smem.vSetUCData(revSyncEnable, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("revSyncSocket", "revSyncInterval", 3);
        smem.vSetUCData(revSyncInterval, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("revSyncSocket", "revSyncTimeDiffTolarence", 10);
        smem.vSetUCData(revSyncTimeDiffTolarence, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("revSyncSocket", "revSyncCountineStatusAdj", 3);
        smem.vSetUCData(revSyncCountineStatusAdj, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("revSyncSocket", "revDefaultVehWay", 0);
        smem.vSetUCData(revDefaultVehWay, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("revSyncSocket", "revLogToFile", 0);
        smem.vSetUCData(revLogToFile, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("revSyncSocket", "revSendProtocol", 0);
        smem.vSetUCData(revSendProtocol, iTmp);
        _sUDPMappingInfoTMP.cLocalIP = oStorINI.vLoadStringFromINI("revSyncSocket", "localIP", "127.0.0.1");
        _sUDPMappingInfoTMP.iListenPort = oStorINI.vLoadIntFromINI("revSyncSocket", "localPort", 0);
        _sUDPMappingInfoTMP.cRemoteIP = oStorINI.vLoadStringFromINI("revSyncSocket", "destIP", "127.0.0.1");
        _sUDPMappingInfoTMP.iSendPort = oStorINI.vLoadIntFromINI("revSyncSocket", "destPort", 0);
        if (_sUDPMappingInfoTMP.iListenPort != 0) {
            if (smem.revSyncSocket.SetConnDevice(DEVICEREVSYNC))
                if ((tempmax = smem.revSyncSocket.OpenUdpSocket(_sUDPMappingInfoTMP.cLocalIP,
                                                                _sUDPMappingInfoTMP.iListenPort,
                                                                _sUDPMappingInfoTMP.cRemoteIP,
                                                                _sUDPMappingInfoTMP.iSendPort)) > 0) {
                    if (tempmax > maxport) maxport = tempmax;
                    printf("open revSyncSocket-%s:%d (fdValue:%d) Success!!\n", _sUDPMappingInfoTMP.cLocalIP,
                           _sUDPMappingInfoTMP.iListenPort, tempmax);
                } else
                    printf("open revSyncSocket-%s:%d %dFail!!\n", _sUDPMappingInfoTMP.cLocalIP,
                           _sUDPMappingInfoTMP.iListenPort, tempmax);
        }

        //OT990707
        iTmp = oStorINI.vLoadIntFromINI("amegidsSocket", "amegidsDynEnable", 0);
        smem.vSetUCData(amegidsDynEnable, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("amegidsSocket", "amegidsDynDevLCN", 0);
        smem.vSetUSIData(amegidsDynDevLCN, iTmp);
        _sUDPMappingInfoTMP.cLocalIP = oStorINI.vLoadStringFromINI("amegidsSocket", "localIP", "127.0.0.1");
        _sUDPMappingInfoTMP.iListenPort = oStorINI.vLoadIntFromINI("amegidsSocket", "localPort", 0);
        _sUDPMappingInfoTMP.cRemoteIP = oStorINI.vLoadStringFromINI("amegidsSocket", "destIP", "127.0.0.1");
        _sUDPMappingInfoTMP.iSendPort = oStorINI.vLoadIntFromINI("amegidsSocket", "destPort", 0);
        if (_sUDPMappingInfoTMP.iListenPort != 0) {
            if (smem.amegidsSocket.SetConnDevice(DEVICEAMEGIDS))
                if ((tempmax = smem.amegidsSocket.OpenUdpSocket(_sUDPMappingInfoTMP.cLocalIP,
                                                                _sUDPMappingInfoTMP.iListenPort,
                                                                _sUDPMappingInfoTMP.cRemoteIP,
                                                                _sUDPMappingInfoTMP.iSendPort)) > 0) {
                    if (tempmax > maxport) maxport = tempmax;
                    printf("open amegidsSocket-%s:%d (fdValue:%d) Success!!\n", _sUDPMappingInfoTMP.cLocalIP,
                           _sUDPMappingInfoTMP.iListenPort, tempmax);
                } else
                    printf("open amegidsSocket-%s:%d %dFail!!\n", _sUDPMappingInfoTMP.cLocalIP,
                           _sUDPMappingInfoTMP.iListenPort, tempmax);
        }


/*-- OT ADD 950927 */

        _sUDPMappingTMP = smem.vGetUDPMappingLCNData(0);
        if (_sUDPMappingTMP.iListenPort != 0) {
            if (smem.DynCalServerInCrossSocket.SetConnDevice(DEVICECENTER92))         //Amegids
//      if (smem.DynCalServerInCrossSocket.SetConnDevice(DEVICE77TC))         //Amegids
                if ((tempmax = smem.DynCalServerInCrossSocket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP,
                                                                            _sUDPMappingTMP.iListenPort,
                                                                            _sUDPMappingTMP.cRemoteIP,
                                                                            _sUDPMappingTMP.iSendPort)) > 0) {
                    if (tempmax > maxport) maxport = tempmax;
                    printf("open DynCalServerInCrossSocket-%s:%d (fdValue:%d) Success!!\n", _sUDPMappingTMP.cLocalIP,
                           _sUDPMappingTMP.iListenPort, tempmax);
                } else
                    printf("open DynCalServerInCrossSocket-%s:%d %dFail!!\n", _sUDPMappingTMP.cLocalIP,
                           _sUDPMappingTMP.iListenPort, tempmax);
        }

        _sUDPMappingTMP = smem.vGetUDPMappingLCNData(1);
        if (_sUDPMappingTMP.iListenPort != 0) {
            if (smem.DynCalServerInCenterSocket.SetConnDevice(DEVICECENTER92))
                if ((tempmax = smem.DynCalServerInCenterSocket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP,
                                                                             _sUDPMappingTMP.iListenPort,
                                                                             _sUDPMappingTMP.cRemoteIP,
                                                                             _sUDPMappingTMP.iSendPort)) > 0) {
                    if (tempmax > maxport) maxport = tempmax;
                    printf("open DynCalServerInCenterSocket-%s:%d (fdValue:%d) Success!!\n", _sUDPMappingTMP.cLocalIP,
                           _sUDPMappingTMP.iListenPort, tempmax);
                } else
                    printf("open DynCalServerInCenterSocket-%s:%d %dFail!!\n", _sUDPMappingTMP.cLocalIP,
                           _sUDPMappingTMP.iListenPort, tempmax);
        }


        _sUDPMappingTMP = smem.vGetUDPMappingLCNData(2);
        if (_sUDPMappingTMP.iListenPort != 0) {
            if (smem.SSVD01Socket.SetConnDevice(DEVICE92TC))                          //Amegids
                if ((tempmax = smem.SSVD01Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                               _sUDPMappingTMP.cRemoteIP, _sUDPMappingTMP.iSendPort)) >
                    0) {
                    if (tempmax > maxport) maxport = tempmax;
                    printf("open SSVD01-%s:%d (fdValue:%d) Success!!\n", _sUDPMappingTMP.cLocalIP,
                           _sUDPMappingTMP.iListenPort, tempmax);
                } else
                    printf("open SSVD01Socket-%s:%d %d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                           tempmax);
        }

        _sUDPMappingTMP = smem.vGetUDPMappingLCNData(3);
        if (_sUDPMappingTMP.iListenPort != 0) {
            if (smem.SSVD02Socket.SetConnDevice(DEVICE92TC))                          //Amegids
                if ((tempmax = smem.SSVD02Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                               _sUDPMappingTMP.cRemoteIP, _sUDPMappingTMP.iSendPort)) >
                    0) {
                    if (tempmax > maxport) maxport = tempmax;
                    printf("open SSVD02-%s:%d (fdValue:%d) Success!!\n", _sUDPMappingTMP.cLocalIP,
                           _sUDPMappingTMP.iListenPort, tempmax);
                } else
                    printf("open SSVD02Socket-%s:%d %d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                           tempmax);
        }

        _sUDPMappingTMP = smem.vGetUDPMappingLCNData(4);
        if (_sUDPMappingTMP.iListenPort != 0) {
            if (smem.SSVD03Socket.SetConnDevice(DEVICE92TC))                          //Amegids
                if ((tempmax = smem.SSVD03Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                               _sUDPMappingTMP.cRemoteIP, _sUDPMappingTMP.iSendPort)) >
                    0) {
                    if (tempmax > maxport) maxport = tempmax;
                } else
                    printf("open SSVD03Socket-%s:%d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort);
        }


        maxport = maxport + 1;                                                          //important


        //?}??IO ?q?T??
        //LPT1(0x378) LCD???O
//ICOPBUG
//    if (lcd240x128.GetAuthority(0x278)) {                                       //?o??kernal?v??,?i?H????LPT1
        if (lcd240x128.GetAuthority(0x378)) {                                       //?o??kernal?v??,?i?H????LPT1
            lcd240x128.GRAPHIC_MODE();                                              //?NLcd?]?w??Graphic?e??
            //OTMARKPRINTF  printf("LCD GetKernalAuthority Successful!!\n");
        } else printf("LCD GetKernalAuthority Fail!!\n");

        //Digital Input Output
        if (digitalIO.GetAuthority(0x200, 0x208)) {
            //OTMARKPRINTF  printf("MotherBoard Digital I/O GetKernalAuthority Successful!!\n");
        } else printf("MotherBoard Digital I/O GetKernalAuthority Failure!!\n");

        screenLogo.DisplayLogo();                                                   //????Logo??

        printf("Main 1 OK! \n");

        _intervalTimer.ThreadsGenerate();                                           //?_Interval Timer Thread

        //pthread_t tidTimerHandle;                                                   //?}?t?@??thread,????timer
        //pthread_create (&tidTimerHandle,NULL,&timerHandle,NULL);

/*Combo++*/

/*
    stc.ReadAllData();
    stc.AllRed5Seconds();
    stc.Lock_to_Set_Control_Strategy(STRATEGY_TOD);
//    stc.Lock_to_Set_Control_Strategy(STRATEGY_ALLRED);
//    usleep(100);
//    printf("OT set to flash OK!\n");

//    SendRequestToKeypad();                                                      //??keypad???e???O?]?w
//    if(!stc.SetDefaultLCNPhaseOrder(47, 0x00)) exit(-1);
    stc.SetDefaultLCNPhaseOrder(1,0);
    //  stc.Lock_to_Set_Control_Strategy(STRATEGY_FLASH);
*/
/*Combo--*/
        //SendStartMsg();
/*OTCombo0714*/
        stc.ThreadsGenerate();

        //OT Fix 950727   LCN0000
        SendRequestToKeypad();                                                      //??keypad???e???O?]?w

        printf("Main 2 OK! \n");

        bzero(cTMP, 128);
        iFWYearTmp = smem.vGetFirmwareInfo_0FC3(0);
        iFWMonthTmp = smem.vGetFirmwareInfo_0FC3(1);
        iFWDayTmp = smem.vGetFirmwareInfo_0FC3(2);
        sprintf(cTMP, "TC Booting, FW Ver:%02d-%02d-%02d", iFWYearTmp, iFWMonthTmp, iFWDayTmp);
        smem.vWriteMsgToDOM(cTMP);

        while (notexit) {

            FD_ZERO(&readfs);

            //join rs232 fd_set
            if (smem.centerPort.GetPortAlreadyOpen()) FD_SET(smem.centerPort.Getfd(), &readfs);

            if (iCom2Type == Com2IsTesterPort) {  //Com2
            } else if (iCom2Type == Com2IsGPSPort || iCom2Type == Com2IsHOLUXGPSPort) {
                if (smem.NMEAPort.GetPortAlreadyOpen()) FD_SET(smem.NMEAPort.Getfd(), &readfs);
            } else if (iCom2Type == Com2IsPassingPort) {
                if (smem.passingPort.GetPortAlreadyOpen()) FD_SET(smem.passingPort.Getfd(), &readfs);
            }

//OTSS++
            //OT1000211 disable        if (smem.ssPort.GetPortAlreadyOpen()) FD_SET(smem.ssPort.Getfd(),&readfs);
//OTSS--
            if (smem.keypadPort.GetPortAlreadyOpen()) FD_SET(smem.keypadPort.Getfd(), &readfs);
            if (smem.revLaneManualPort.GetPortAlreadyOpen()) FD_SET(smem.revLaneManualPort.Getfd(), &readfs);

//        if (smem.rtmsPort.GetPortAlreadyOpen()) FD_SET(smem.rtmsPort.Getfd(),&readfs);
            if (smem.lightPort.GetPortAlreadyOpen()) FD_SET(smem.lightPort.Getfd(), &readfs);
            if (smem.redCountPort.GetPortAlreadyOpen()) FD_SET(smem.redCountPort.Getfd(), &readfs);
            //join udp fd_set

            if (smem.centerSocket.GetPortAlreadyOpen()) FD_SET(smem.centerSocket.Getfd(), &readfs);
//        if (smem.tempSocket.GetPortAlreadyOpen()) FD_SET(smem.tempSocket.Getfd(),&readfs);
            if (smem.testerSocket.GetPortAlreadyOpen()) FD_SET(smem.testerSocket.Getfd(), &readfs);

/* OT ADD 950927 */
            if (smem.CCJDynCtlSocket.GetPortAlreadyOpen()) FD_SET(smem.CCJDynCtlSocket.Getfd(), &readfs);
            if (smem.CCJDynCtl2Socket.GetPortAlreadyOpen()) FD_SET(smem.CCJDynCtl2Socket.Getfd(), &readfs);
/*-- OT ADD 950927 */

            if (smem.DynCalServerInCrossSocket.GetPortAlreadyOpen())
                    FD_SET(smem.DynCalServerInCrossSocket.Getfd(), &readfs);
            if (smem.DynCalServerInCenterSocket.GetPortAlreadyOpen())
                    FD_SET(smem.DynCalServerInCenterSocket.Getfd(), &readfs);

//OT990618
            if (smem.revSyncSocket.GetPortAlreadyOpen()) FD_SET(smem.revSyncSocket.Getfd(), &readfs);

            if (smem.amegidsSocket.GetPortAlreadyOpen()) FD_SET(smem.amegidsSocket.Getfd(), &readfs);

            if (smem.SSVD01Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD01Socket.Getfd(), &readfs);
            if (smem.SSVD02Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD02Socket.Getfd(), &readfs);
            if (smem.SSVD03Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD03Socket.Getfd(), &readfs);
            if (smem.SSVD04Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD04Socket.Getfd(), &readfs);
//OT Pass
            if (smem.SSVD05Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD05Socket.Getfd(), &readfs);
            if (smem.SSVD06Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD06Socket.Getfd(), &readfs);
            if (smem.SSVD07Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD07Socket.Getfd(), &readfs);
            if (smem.SSVD08Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD08Socket.Getfd(), &readfs);
            if (smem.SSVD09Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD09Socket.Getfd(), &readfs);
            if (smem.SSVD10Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD10Socket.Getfd(), &readfs);
            if (smem.SSVD11Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD11Socket.Getfd(), &readfs);
            if (smem.SSVD12Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD12Socket.Getfd(), &readfs);
            if (smem.SSVD13Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD13Socket.Getfd(), &readfs);
            if (smem.SSVD14Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD14Socket.Getfd(), &readfs);
            if (smem.SSVD15Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD15Socket.Getfd(), &readfs);
            if (smem.SSVD16Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD16Socket.Getfd(), &readfs);
            if (smem.SSVD17Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD17Socket.Getfd(), &readfs);
            if (smem.SSVD18Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD18Socket.Getfd(), &readfs);
            if (smem.SSVD19Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD19Socket.Getfd(), &readfs);
            if (smem.SSVD20Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD20Socket.Getfd(), &readfs);
            if (smem.SSVD21Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD21Socket.Getfd(), &readfs);
            if (smem.SSVD22Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD22Socket.Getfd(), &readfs);
            if (smem.SSVD23Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD23Socket.Getfd(), &readfs);
            if (smem.SSVD24Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD24Socket.Getfd(), &readfs);
            if (smem.SSVD25Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD25Socket.Getfd(), &readfs);
            if (smem.SSVD26Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD26Socket.Getfd(), &readfs);
            if (smem.SSVD27Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD27Socket.Getfd(), &readfs);
            if (smem.SSVD28Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD28Socket.Getfd(), &readfs);
            if (smem.SSVD29Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD29Socket.Getfd(), &readfs);
            if (smem.SSVD30Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD30Socket.Getfd(), &readfs);
/*
        if (smem.SSVD31Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD31Socket.Getfd(),&readfs);
        if (smem.SSVD32Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD32Socket.Getfd(),&readfs);
*/

            timeout.tv_sec = 60;                                                      //timeout????
            timeout.tv_usec = 0;                                                      //?o???O?@??,????????

            rec = select(maxport, &readfs, NULL, NULL, &timeout);                         //wait block for read

            if (rec < 0) {                                                            //Select Error

            } else if (rec == 0) {                                                    //Select Time Out
                //OTMARKPRINTF  printf("Select Time Out!!\n");
            } else {

                if (smem.centerPort.GetPortAlreadyOpen()) {                         //port?O?_?w?}??
                    if (FD_ISSET(smem.centerPort.Getfd(), &readfs)) {                //port?O?_??????
                        readSelectLength = smem.centerPort.Rs232Read();               //?Nrs232????q1`???ibuffer??
                        printf("receive Center Message !!!!!\n");
                        for (int i = 0; i < readSelectLength; i++) {
                            printf("%x ", smem.centerPort.block[i]);
                        }
                        printf("\n");
                        if (readSelectLength > 0) {
                            if (smem.vGetCommEnable() == true) {
                                //OT20110526
                                smem.vSetLastGetProtocolTime();
                                parseAABB.ParseBlock(readSelectLength, smem.centerPort.block, smem.centerPort.messageIn,
                                                     &smem.centerPort.lastPacketIndex,
                                                     &smem.centerPort.maxMessageIndex);
                                parseAABB.CheckSum(&smem.centerPort.maxMessageIndex, smem.centerPort.messageIn);
                                parseAABB.DecideProtocol(&smem.centerPort.maxMessageIndex, smem.centerPort.messageIn,
                                                         smem.centerPort.GetConnDevice());
                                parseAABB.vJudgeProtocolAndCheckLength(&smem.centerPort.maxMessageIndex,
                                                                       smem.centerPort.messageIn);
                                parseAABB.CheckReasonable(&smem.centerPort.maxMessageIndex, smem.centerPort.messageIn);
                                parseAABB.AssignLcn(&smem.centerPort.maxMessageIndex, smem.centerPort.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.centerPort.maxMessageIndex,
                                                                smem.centerPort.messageIn, cRS232);
                                readJob.vCheckIfBcastingForwardToUDP(&smem.centerPort.maxMessageIndex,
                                                                     smem.centerPort.messageIn);
                                readJob.CheckLcn(&smem.centerPort.maxMessageIndex, smem.centerPort.messageIn);
                                readJob.SetCenterComm(&smem.centerPort.maxMessageIndex, smem.centerPort.messageIn);
                                readJob.DoWorkByMESSAGEIN(&smem.centerPort.maxMessageIndex, smem.centerPort.messageIn);
                                parseAABB.EchoToGUI(&smem.centerPort.maxMessageIndex, smem.centerPort.messageIn,
                                                    "/dev/ttyS0");
                                parseAABB.MoveLastData(&smem.centerPort.maxMessageIndex,
                                                       &smem.centerPort.lastPacketIndex, smem.centerPort.messageIn);
                            }
                        }
                    }
                }

                if (smem.keypadPort.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.keypadPort.Getfd(), &readfs)) {
                        readSelectLength = smem.keypadPort.Rs232Read();
                        if (readSelectLength > 0) {

//OT20111116
                            if (smem.vGetCommEnable() == true) {
                                parseAABB.ParseBlock(readSelectLength, smem.keypadPort.block, smem.keypadPort.messageIn,
                                                     &smem.keypadPort.lastPacketIndex,
                                                     &smem.keypadPort.maxMessageIndex);
                                parseAABB.CheckSum(&smem.keypadPort.maxMessageIndex, smem.keypadPort.messageIn);
                                parseAABB.DecideProtocol(&smem.keypadPort.maxMessageIndex, smem.keypadPort.messageIn,
                                                         smem.keypadPort.GetConnDevice());
                                parseAABB.CheckReasonable(&smem.keypadPort.maxMessageIndex, smem.keypadPort.messageIn);
                                parseAABB.AssignLcn(&smem.keypadPort.maxMessageIndex, smem.keypadPort.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.keypadPort.maxMessageIndex,
                                                                smem.keypadPort.messageIn, cRS232);
                                readJob.SetKeypadComm(&smem.keypadPort.maxMessageIndex, smem.keypadPort.messageIn);
                                readJob.DoWorkByMESSAGEIN(&smem.keypadPort.maxMessageIndex, smem.keypadPort.messageIn);
                                parseAABB.EchoToGUI(&smem.keypadPort.maxMessageIndex, smem.keypadPort.messageIn,
                                                    "/dev/ttyS2");
                                parseAABB.MoveLastData(&smem.keypadPort.maxMessageIndex,
                                                       &smem.keypadPort.lastPacketIndex, smem.keypadPort.messageIn);
                            }
                        }
                    }
                }

                if (smem.revLaneManualPort.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.revLaneManualPort.Getfd(), &readfs)) {
                        readSelectLength = smem.revLaneManualPort.Rs232Read();
                        if (readSelectLength > 0) {

                            printf("Get Rev:");

                            for (int ii = 0; ii < readSelectLength; ii++) {
                                printf("%X ", smem.revLaneManualPort.block[ii]);
                            }
                            printf("\n");

//OT20111116
                            if (smem.vGetCommEnable() == true) {

                                parseAABB.ParseBlock(readSelectLength, smem.revLaneManualPort.block,
                                                     smem.revLaneManualPort.messageIn,
                                                     &smem.revLaneManualPort.lastPacketIndex,
                                                     &smem.revLaneManualPort.maxMessageIndex);
                                parseAABB.CheckSum(&smem.revLaneManualPort.maxMessageIndex,
                                                   smem.revLaneManualPort.messageIn);
                                parseAABB.DecideProtocol(&smem.revLaneManualPort.maxMessageIndex,
                                                         smem.revLaneManualPort.messageIn,
                                                         smem.revLaneManualPort.GetConnDevice());
                                parseAABB.CheckReasonable(&smem.revLaneManualPort.maxMessageIndex,
                                                          smem.revLaneManualPort.messageIn);
                                parseAABB.AssignLcn(&smem.revLaneManualPort.maxMessageIndex,
                                                    smem.revLaneManualPort.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.revLaneManualPort.maxMessageIndex,
                                                                smem.revLaneManualPort.messageIn, cRS232);
                                readJob.CheckLcn(&smem.revLaneManualPort.maxMessageIndex,
                                                 smem.revLaneManualPort.messageIn);
                                readJob.DoWorkByMESSAGEIN(&smem.revLaneManualPort.maxMessageIndex,
                                                          smem.revLaneManualPort.messageIn);
                                parseAABB.EchoToGUI(&smem.revLaneManualPort.maxMessageIndex,
                                                    smem.revLaneManualPort.messageIn, "/dev/ttyS0");
                                parseAABB.MoveLastData(&smem.revLaneManualPort.maxMessageIndex,
                                                       &smem.revLaneManualPort.lastPacketIndex,
                                                       smem.revLaneManualPort.messageIn);
                            }

                        }
                    }
                }

                if (iCom2Type == Com2IsTesterPort) {                                     // selsec 1

                } else if (iCom2Type == Com2IsGPSPort ||
                           iCom2Type == Com2IsHOLUXGPSPort) {                              // selsec 2
                    if (smem.NMEAPort.GetPortAlreadyOpen()) {
                        if (FD_ISSET(smem.NMEAPort.Getfd(), &readfs)) {
                            readSelectLength = smem.NMEAPort.Rs232Read();
                            if (readSelectLength > 0) {
                                if (smem.vGetCommEnable() == true) {  //OT20110728
                                    parseNMEA.ParseNMEABlock(readSelectLength, smem.NMEAPort.block,
                                                             smem.NMEAPort.messageIn, &smem.NMEAPort.lastPacketIndex,
                                                             &smem.NMEAPort.maxMessageIndex);
                                    parseNMEA.vFindGPRMC(&smem.NMEAPort.maxMessageIndex, smem.NMEAPort.messageIn);
                                    parseNMEA.vAdjTimeByGPS(&smem.NMEAPort.maxMessageIndex, smem.NMEAPort.messageIn);
                                    parseNMEA.EchoToGUI(&smem.NMEAPort.maxMessageIndex, smem.NMEAPort.messageIn,
                                                        "/dev/ttyS1");
                                    parseNMEA.MoveLastData(&smem.NMEAPort.maxMessageIndex,
                                                           &smem.NMEAPort.lastPacketIndex, smem.NMEAPort.messageIn);
                                }
                            }
                        }
                    }
                } else if (iCom2Type == Com2IsPassingPort) {                            // selsec 3
                    if (smem.passingPort.GetPortAlreadyOpen()) {
                        if (FD_ISSET(smem.passingPort.Getfd(), &readfs)) {
                            readSelectLength = smem.passingPort.Rs232Read();
                            if (readSelectLength > 0) {
                                parseAABB.ParseBlock(readSelectLength, smem.passingPort.block,
                                                     smem.passingPort.messageIn, &smem.passingPort.lastPacketIndex,
                                                     &smem.passingPort.maxMessageIndex);
                                parseAABB.CheckSum(&smem.passingPort.maxMessageIndex, smem.passingPort.messageIn);
                                parseAABB.DecideProtocol(&smem.passingPort.maxMessageIndex, smem.passingPort.messageIn,
                                                         smem.passingPort.GetConnDevice());
                                parseAABB.vJudgeProtocolAndCheckLength(&smem.passingPort.maxMessageIndex,
                                                                       smem.passingPort.messageIn);
                                parseAABB.CheckReasonable(&smem.passingPort.maxMessageIndex,
                                                          smem.passingPort.messageIn);
                                parseAABB.AssignLcn(&smem.passingPort.maxMessageIndex, smem.passingPort.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.passingPort.maxMessageIndex,
                                                                smem.passingPort.messageIn, cUDP);
                                readJob.DoSendUDP_MESSAGEIN(&smem.passingPort.maxMessageIndex,
                                                            smem.passingPort.messageIn);
                                parseAABB.EchoToGUI(&smem.passingPort.maxMessageIndex, smem.passingPort.messageIn,
                                                    "CMS Passing");
                                parseAABB.MoveLastData(&smem.passingPort.maxMessageIndex,
                                                       &smem.passingPort.lastPacketIndex, smem.passingPort.messageIn);
                            }
                        }
                    }

                }

                if (smem.lightPort.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.lightPort.Getfd(), &readfs)) {
                        readSelectLength = smem.lightPort.Rs232Read();
                        if (readSelectLength > 0) {
                            if (smem.vGetCommEnable() == true) {  //OT20110728
                                parseAABB.ParseBlock(readSelectLength, smem.lightPort.block, smem.lightPort.messageIn,
                                                     &smem.lightPort.lastPacketIndex, &smem.lightPort.maxMessageIndex);
                                parseAABB.CheckSum(&smem.lightPort.maxMessageIndex, smem.lightPort.messageIn);
                                parseAABB.DecideProtocol(&smem.lightPort.maxMessageIndex, smem.lightPort.messageIn,
                                                         smem.lightPort.GetConnDevice());
                                parseAABB.CheckReasonable(&smem.lightPort.maxMessageIndex, smem.lightPort.messageIn);
                                parseAABB.AssignLcn(&smem.lightPort.maxMessageIndex, smem.lightPort.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.lightPort.maxMessageIndex,
                                                                smem.lightPort.messageIn, cRS232);
                                readJob.DoWorkByMESSAGEIN(&smem.lightPort.maxMessageIndex, smem.lightPort.messageIn);
                                parseAABB.EchoToGUI(&smem.lightPort.maxMessageIndex, smem.lightPort.messageIn,
                                                    "/dev/ttyS3");
                                parseAABB.MoveLastData(&smem.lightPort.maxMessageIndex, &smem.lightPort.lastPacketIndex,
                                                       smem.lightPort.messageIn);

/*
                        printf("lightPort: ");
                        for(int ii = 0; ii < readSelectLength; ii++) {
                          printf("%02X ", smem.lightPort.block[ii]);
                        }
                        printf("\n");
*/

                                smem.vSetBOOLData(TC_SIGNAL_DRIVER_UNIT, true);
                            }
                        }
                    }
                }


//OTSS++
                //OT1000211 disable
                /*
            if (smem.ssPort.GetPortAlreadyOpen()) {
                if (FD_ISSET(smem.ssPort.Getfd(),&readfs)) {
                    readSelectLength=smem.ssPort.Rs232Read();
                    if (readSelectLength>0) {
                        parseSS.ParseBlock(readSelectLength,smem.ssPort.block,smem.ssPort.messageIn,&smem.ssPort.lastPacketIndex,&smem.ssPort.maxMessageIndex);
                        parseSS.vMultiDropProcess(&smem.ssPort.maxMessageIndex,smem.ssPort.messageIn);
                        parseSS.CheckSum(&smem.ssPort.maxMessageIndex,smem.ssPort.messageIn);
                        readJob.SetInterfaceAndReadFlag(&smem.ssPort.maxMessageIndex,smem.ssPort.messageIn,cRS232);
                        readJob.DoWorkByMESSAGEIN(&smem.ssPort.maxMessageIndex,smem.ssPort.messageIn);
                        parseSS.EchoToGUI(&smem.ssPort.maxMessageIndex,smem.ssPort.messageIn,"/dev/ttyS1");
                        parseSS.MoveLastData(&smem.ssPort.maxMessageIndex,&smem.ssPort.lastPacketIndex,smem.ssPort.messageIn);
                    }
                }
            }
*/

//OTSS--

                if (smem.centerSocket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.centerSocket.Getfd(), &readfs)) {
                        smem.iSetVer34CommLog(0);
                        readSelectLength = smem.centerSocket.UdpRead();

                        if (readSelectLength > 0) {
                            printf("\n\n\n\n\n");
                            printf("aloooo~~~~\n");
                            for (int i = 0; i < readSelectLength; i++) {
                                printf("%x ", smem.centerSocket.block[i]);
                            }
                            printf("\n\n\n\n\n");

                            if (smem.vGetCommEnable() == true) {
                                //OT20110526
                                smem.vSetLastGetProtocolTime();
                                printf("aloooo~~~~2222\n");
                                parseAABB.ParseBlock(readSelectLength, smem.centerSocket.block,
                                                     smem.centerSocket.messageIn, &smem.centerSocket.lastPacketIndex,
                                                     &smem.centerSocket.maxMessageIndex);
                                parseAABB.CheckSum(&smem.centerSocket.maxMessageIndex, smem.centerSocket.messageIn);
                                //enable vJudgeProtocolAndCheckLength
                                parseAABB.vJudgeProtocolAndCheckLength(&smem.centerSocket.maxMessageIndex,
                                                                       smem.centerSocket.messageIn);
                                parseAABB.DecideProtocol(&smem.centerSocket.maxMessageIndex,
                                                         smem.centerSocket.messageIn,
                                                         smem.centerSocket.GetConnDevice());
                                parseAABB.CheckReasonable(&smem.centerSocket.maxMessageIndex,
                                                          smem.centerSocket.messageIn);
                                parseAABB.AssignLcn(&smem.centerSocket.maxMessageIndex, smem.centerSocket.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.centerSocket.maxMessageIndex,
                                                                smem.centerSocket.messageIn, cUDP);
                                readJob.vCheckIfBcastingForwardToUDP(&smem.centerSocket.maxMessageIndex,
                                                                     smem.centerSocket.messageIn);
                                readJob.CheckLcn(&smem.centerSocket.maxMessageIndex, smem.centerSocket.messageIn);
                                readJob.SetCenterComm(&smem.centerSocket.maxMessageIndex, smem.centerSocket.messageIn);
                                readJob.DoWorkByMESSAGEIN(&smem.centerSocket.maxMessageIndex,
                                                          smem.centerSocket.messageIn);
                                parseAABB.EchoToGUI(&smem.centerSocket.maxMessageIndex, smem.centerSocket.messageIn,
                                                    "192.168.1.101:6003");
                                parseAABB.MoveLastData(&smem.centerSocket.maxMessageIndex,
                                                       &smem.centerSocket.lastPacketIndex, smem.centerSocket.messageIn);
                            }
                        }
                    }
                }

                if (smem.CCJDynCtlSocket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.CCJDynCtlSocket.Getfd(), &readfs)) {
                        readSelectLength = smem.CCJDynCtlSocket.UdpRead();
                        if (readSelectLength > 0) {
                            if (smem.vGetCommEnable() == true) {
                                parseAABB.ParseBlock(readSelectLength, smem.CCJDynCtlSocket.block,
                                                     smem.CCJDynCtlSocket.messageIn,
                                                     &smem.CCJDynCtlSocket.lastPacketIndex,
                                                     &smem.CCJDynCtlSocket.maxMessageIndex);
                                parseAABB.CheckSum(&smem.CCJDynCtlSocket.maxMessageIndex,
                                                   smem.CCJDynCtlSocket.messageIn);
//enable vJudgeProtocolAndCheckLength
//                        parseAABB.vJudgeProtocolAndCheckLength(&smem.CCJDynCtlSocket.maxMessageIndex,smem.CCJDynCtlSocket.messageIn);
                                parseAABB.DecideProtocol(&smem.CCJDynCtlSocket.maxMessageIndex,
                                                         smem.CCJDynCtlSocket.messageIn,
                                                         smem.CCJDynCtlSocket.GetConnDevice());
                                parseAABB.CheckReasonable(&smem.CCJDynCtlSocket.maxMessageIndex,
                                                          smem.CCJDynCtlSocket.messageIn);
                                parseAABB.AssignLcn(&smem.CCJDynCtlSocket.maxMessageIndex,
                                                    smem.CCJDynCtlSocket.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.CCJDynCtlSocket.maxMessageIndex,
                                                                smem.CCJDynCtlSocket.messageIn, cUDP);
//                        readJob.vCheckIfBcastingForwardToUDP(&smem.CCJDynCtlSocket.maxMessageIndex,smem.CCJDynCtlSocket.messageIn);
                                readJob.CheckLcn(&smem.CCJDynCtlSocket.maxMessageIndex, smem.CCJDynCtlSocket.messageIn);
//enable SetCenterComm
//OT20110526                        readJob.SetCenterComm(&smem.CCJDynCtlSocket.maxMessageIndex,smem.CCJDynCtlSocket.messageIn);
                                readJob.DoWorkByMESSAGEIN(&smem.CCJDynCtlSocket.maxMessageIndex,
                                                          smem.CCJDynCtlSocket.messageIn);
                                parseAABB.EchoToGUI(&smem.CCJDynCtlSocket.maxMessageIndex,
                                                    smem.CCJDynCtlSocket.messageIn, "CCJDynCtlSocket");
                                parseAABB.MoveLastData(&smem.CCJDynCtlSocket.maxMessageIndex,
                                                       &smem.CCJDynCtlSocket.lastPacketIndex,
                                                       smem.CCJDynCtlSocket.messageIn);
                            }
                        }
                    }
                }

                if (smem.CCJDynCtl2Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.CCJDynCtl2Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.CCJDynCtl2Socket.UdpRead();
                        if (readSelectLength > 0) {
                            if (smem.vGetCommEnable() == true) {
                                parseAABB.ParseBlock(readSelectLength, smem.CCJDynCtl2Socket.block,
                                                     smem.CCJDynCtl2Socket.messageIn,
                                                     &smem.CCJDynCtl2Socket.lastPacketIndex,
                                                     &smem.CCJDynCtl2Socket.maxMessageIndex);
                                parseAABB.CheckSum(&smem.CCJDynCtl2Socket.maxMessageIndex,
                                                   smem.CCJDynCtl2Socket.messageIn);
//enable vJudgeProtocolAndCheckLength
//                        parseAABB.vJudgeProtocolAndCheckLength(&smem.CCJDynCtl2Socket.maxMessageIndex,smem.CCJDynCtl2Socket.messageIn);
                                parseAABB.DecideProtocol(&smem.CCJDynCtl2Socket.maxMessageIndex,
                                                         smem.CCJDynCtl2Socket.messageIn,
                                                         smem.CCJDynCtl2Socket.GetConnDevice());
                                parseAABB.CheckReasonable(&smem.CCJDynCtl2Socket.maxMessageIndex,
                                                          smem.CCJDynCtl2Socket.messageIn);
                                parseAABB.AssignLcn(&smem.CCJDynCtl2Socket.maxMessageIndex,
                                                    smem.CCJDynCtl2Socket.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.CCJDynCtl2Socket.maxMessageIndex,
                                                                smem.CCJDynCtl2Socket.messageIn, cUDP);
//                        readJob.vCheckIfBcastingForwardToUDP(&smem.CCJDynCtl2Socket.maxMessageIndex,smem.CCJDynCtl2Socket.messageIn);
                                readJob.CheckLcn(&smem.CCJDynCtl2Socket.maxMessageIndex,
                                                 smem.CCJDynCtl2Socket.messageIn);
//enable SetCenterComm
//OT20110526                        readJob.SetCenterComm(&smem.CCJDynCtl2Socket.maxMessageIndex,smem.CCJDynCtl2Socket.messageIn);
                                readJob.DoWorkByMESSAGEIN(&smem.CCJDynCtl2Socket.maxMessageIndex,
                                                          smem.CCJDynCtl2Socket.messageIn);
                                parseAABB.EchoToGUI(&smem.CCJDynCtl2Socket.maxMessageIndex,
                                                    smem.CCJDynCtl2Socket.messageIn, "CCJDynCtl2Socket");
                                parseAABB.MoveLastData(&smem.CCJDynCtl2Socket.maxMessageIndex,
                                                       &smem.CCJDynCtl2Socket.lastPacketIndex,
                                                       smem.CCJDynCtl2Socket.messageIn);
                            }
                        }
                    }
                }



/*
            if (smem.tempSocket.GetPortAlreadyOpen()) {
                if (FD_ISSET(smem.tempSocket.Getfd(),&readfs)) {
                    readSelectLength=smem.tempSocket.UdpRead();
                    if (readSelectLength>0) {
                        parseAABB.ParseBlock(readSelectLength,smem.tempSocket.block,smem.tempSocket.messageIn,&smem.tempSocket.lastPacketIndex,&smem.tempSocket.maxMessageIndex);
                        parseAABB.CheckSum(&smem.tempSocket.maxMessageIndex,smem.tempSocket.messageIn);
                        parseAABB.DecideProtocol(&smem.tempSocket.maxMessageIndex,smem.tempSocket.messageIn,smem.tempSocket.GetConnDevice());
                        parseAABB.CheckReasonable(&smem.tempSocket.maxMessageIndex,smem.tempSocket.messageIn);
                        parseAABB.AssignLcn(&smem.tempSocket.maxMessageIndex,smem.tempSocket.messageIn);
                        readJob.SetInterfaceAndReadFlag(&smem.tempSocket.maxMessageIndex,smem.tempSocket.messageIn,cUDP);
                        readJob.CheckLcn(&smem.tempSocket.maxMessageIndex,smem.tempSocket.messageIn);
                        readJob.SetCenterComm(&smem.tempSocket.maxMessageIndex,smem.tempSocket.messageIn);
                        readJob.DoWorkByMESSAGEIN(&smem.tempSocket.maxMessageIndex,smem.tempSocket.messageIn);
                        parseAABB.EchoToGUI(&smem.tempSocket.maxMessageIndex,smem.tempSocket.messageIn,"192.168.1.101:6789");
                        parseAABB.MoveLastData(&smem.tempSocket.maxMessageIndex,&smem.tempSocket.lastPacketIndex,smem.tempSocket.messageIn);
                    }
                }
            }
*/

                if (smem.testerSocket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.testerSocket.Getfd(), &readfs)) {
                        smem.iSetVer34CommLog(1);
                        readSelectLength = smem.testerSocket.UdpRead();
                        if (readSelectLength > 0) {
                            if (smem.vGetCommEnable() == true) {
                                //OT20110526
                                smem.vSetLastGetProtocolTime();

                                parseAABB.ParseBlock(readSelectLength, smem.testerSocket.block,
                                                     smem.testerSocket.messageIn, &smem.testerSocket.lastPacketIndex,
                                                     &smem.testerSocket.maxMessageIndex);
                                parseAABB.CheckSum(&smem.testerSocket.maxMessageIndex, smem.testerSocket.messageIn);
                                //enable vJudgeProtocolAndCheckLength
                                parseAABB.vJudgeProtocolAndCheckLength(&smem.testerSocket.maxMessageIndex,
                                                                       smem.testerSocket.messageIn);
                                parseAABB.DecideProtocol(&smem.testerSocket.maxMessageIndex,
                                                         smem.testerSocket.messageIn,
                                                         smem.testerSocket.GetConnDevice());
                                parseAABB.CheckReasonable(&smem.testerSocket.maxMessageIndex,
                                                          smem.testerSocket.messageIn);
                                parseAABB.AssignLcn(&smem.testerSocket.maxMessageIndex, smem.testerSocket.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.testerSocket.maxMessageIndex,
                                                                smem.testerSocket.messageIn, cUDP);
                                readJob.vCheckIfBcastingForwardToUDP(&smem.testerSocket.maxMessageIndex,
                                                                     smem.testerSocket.messageIn);
                                readJob.CheckLcn(&smem.testerSocket.maxMessageIndex, smem.testerSocket.messageIn);
                                readJob.SetCenterComm(&smem.testerSocket.maxMessageIndex, smem.testerSocket.messageIn);
                                readJob.DoWorkByMESSAGEIN(&smem.testerSocket.maxMessageIndex,
                                                          smem.testerSocket.messageIn);
                                parseAABB.EchoToGUI(&smem.testerSocket.maxMessageIndex, smem.testerSocket.messageIn,
                                                    "192.168.1.102:6003");
                                parseAABB.MoveLastData(&smem.testerSocket.maxMessageIndex,
                                                       &smem.testerSocket.lastPacketIndex, smem.testerSocket.messageIn);
                            }
                        }
                    }
                }


                if (smem.DynCalServerInCenterSocket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.DynCalServerInCenterSocket.Getfd(), &readfs)) {
                        readSelectLength = smem.DynCalServerInCenterSocket.UdpRead();
                        if (readSelectLength > 0) {
                            if (smem.vGetCommEnable() == true) {
                                //OT20110526

                                smem.vSetLastGetProtocolTime();
//                      printf("rev packet\n");
                                parseAABB.ParseBlock(readSelectLength, smem.DynCalServerInCenterSocket.block,
                                                     smem.DynCalServerInCenterSocket.messageIn,
                                                     &smem.DynCalServerInCenterSocket.lastPacketIndex,
                                                     &smem.DynCalServerInCenterSocket.maxMessageIndex);
                                parseAABB.CheckSum(&smem.DynCalServerInCenterSocket.maxMessageIndex,
                                                   smem.DynCalServerInCenterSocket.messageIn);
//enable vJudgeProtocolAndCheckLength
                                parseAABB.vJudgeProtocolAndCheckLength(&smem.DynCalServerInCenterSocket.maxMessageIndex,
                                                                       smem.DynCalServerInCenterSocket.messageIn);
                                parseAABB.DecideProtocol(&smem.DynCalServerInCenterSocket.maxMessageIndex,
                                                         smem.DynCalServerInCenterSocket.messageIn,
                                                         smem.DynCalServerInCenterSocket.GetConnDevice());
                                parseAABB.CheckReasonable(&smem.DynCalServerInCenterSocket.maxMessageIndex,
                                                          smem.DynCalServerInCenterSocket.messageIn);
                                parseAABB.AssignLcn(&smem.DynCalServerInCenterSocket.maxMessageIndex,
                                                    smem.DynCalServerInCenterSocket.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.DynCalServerInCenterSocket.maxMessageIndex,
                                                                smem.DynCalServerInCenterSocket.messageIn, cUDP);
                                readJob.vCheckIfBcastingForwardToUDP(&smem.DynCalServerInCenterSocket.maxMessageIndex,
                                                                     smem.DynCalServerInCenterSocket.messageIn);
                                readJob.CheckLcn(&smem.DynCalServerInCenterSocket.maxMessageIndex,
                                                 smem.DynCalServerInCenterSocket.messageIn);
//enable SetCenterComm
                                readJob.SetCenterComm(&smem.DynCalServerInCenterSocket.maxMessageIndex,
                                                      smem.DynCalServerInCenterSocket.messageIn);
                                readJob.DoWorkByMESSAGEIN(&smem.DynCalServerInCenterSocket.maxMessageIndex,
                                                          smem.DynCalServerInCenterSocket.messageIn);
                                parseAABB.EchoToGUI(&smem.DynCalServerInCenterSocket.maxMessageIndex,
                                                    smem.DynCalServerInCenterSocket.messageIn,
                                                    "DynCalServerInCenterSocket");
                                parseAABB.MoveLastData(&smem.DynCalServerInCenterSocket.maxMessageIndex,
                                                       &smem.DynCalServerInCenterSocket.lastPacketIndex,
                                                       smem.DynCalServerInCenterSocket.messageIn);
                            }
                        }
                    }
                }

                if (smem.DynCalServerInCrossSocket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.DynCalServerInCrossSocket.Getfd(), &readfs)) {
                        readSelectLength = smem.DynCalServerInCrossSocket.UdpRead();
                        if (readSelectLength > 0) {
                            if (smem.vGetCommEnable() == true) {
                                //OT20110526
                                smem.vSetLastGetProtocolTime();

                                parseAABB.ParseBlock(readSelectLength, smem.DynCalServerInCrossSocket.block,
                                                     smem.DynCalServerInCrossSocket.messageIn,
                                                     &smem.DynCalServerInCrossSocket.lastPacketIndex,
                                                     &smem.DynCalServerInCrossSocket.maxMessageIndex);
                                parseAABB.CheckSum(&smem.DynCalServerInCrossSocket.maxMessageIndex,
                                                   smem.DynCalServerInCrossSocket.messageIn);
//enable vJudgeProtocolAndCheckLength
                                parseAABB.vJudgeProtocolAndCheckLength(&smem.DynCalServerInCrossSocket.maxMessageIndex,
                                                                       smem.DynCalServerInCrossSocket.messageIn);
                                parseAABB.DecideProtocol(&smem.DynCalServerInCrossSocket.maxMessageIndex,
                                                         smem.DynCalServerInCrossSocket.messageIn,
                                                         smem.DynCalServerInCrossSocket.GetConnDevice());
                                parseAABB.CheckReasonable(&smem.DynCalServerInCrossSocket.maxMessageIndex,
                                                          smem.DynCalServerInCrossSocket.messageIn);
                                parseAABB.AssignLcn(&smem.DynCalServerInCrossSocket.maxMessageIndex,
                                                    smem.DynCalServerInCrossSocket.messageIn);
//                        readJob.SetInterfaceAndReadFlag(&smem.DynCalServerInCrossSocket.maxMessageIndex,smem.DynCalServerInCrossSocket.messageIn,cUDP);
                                readJob.SetInterfaceAndReadFlag(&smem.DynCalServerInCrossSocket.maxMessageIndex,
                                                                smem.DynCalServerInCrossSocket.messageIn,
                                                                cComingFromAmegidsCrossServer);
                                readJob.vCheckIfBcastingForwardToUDP(&smem.DynCalServerInCrossSocket.maxMessageIndex,
                                                                     smem.DynCalServerInCrossSocket.messageIn);
                                readJob.CheckLcn(&smem.DynCalServerInCrossSocket.maxMessageIndex,
                                                 smem.DynCalServerInCrossSocket.messageIn);
//enable SetCenterComm
                                readJob.SetCenterComm(&smem.DynCalServerInCrossSocket.maxMessageIndex,
                                                      smem.DynCalServerInCrossSocket.messageIn);
                                readJob.DoWorkByMESSAGEIN(&smem.DynCalServerInCrossSocket.maxMessageIndex,
                                                          smem.DynCalServerInCrossSocket.messageIn);
                                parseAABB.EchoToGUI(&smem.DynCalServerInCrossSocket.maxMessageIndex,
                                                    smem.DynCalServerInCrossSocket.messageIn,
                                                    "DynCalServerInCrossSocket");
                                parseAABB.MoveLastData(&smem.DynCalServerInCrossSocket.maxMessageIndex,
                                                       &smem.DynCalServerInCrossSocket.lastPacketIndex,
                                                       smem.DynCalServerInCrossSocket.messageIn);
                            }
                        }
                    }
                }

                if (smem.revSyncSocket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.revSyncSocket.Getfd(), &readfs)) {
                        readSelectLength = smem.revSyncSocket.UdpRead();
                        if (readSelectLength > 0) {

                            printf("Get Rev:");

                            for (int ii = 0; ii < readSelectLength; ii++) {
                                printf("%X ", smem.revSyncSocket.block[ii]);
                            }
                            printf("\n");

                            if (smem.vGetCommEnable() == true) {

                                parseAABB.ParseBlock(readSelectLength, smem.revSyncSocket.block,
                                                     smem.revSyncSocket.messageIn, &smem.revSyncSocket.lastPacketIndex,
                                                     &smem.revSyncSocket.maxMessageIndex);
                                parseAABB.CheckSum(&smem.revSyncSocket.maxMessageIndex, smem.revSyncSocket.messageIn);
                                parseAABB.DecideProtocol(&smem.revSyncSocket.maxMessageIndex,
                                                         smem.revSyncSocket.messageIn,
                                                         smem.revSyncSocket.GetConnDevice());
                                parseAABB.CheckReasonable(&smem.revSyncSocket.maxMessageIndex,
                                                          smem.revSyncSocket.messageIn);
                                parseAABB.AssignLcn(&smem.revSyncSocket.maxMessageIndex, smem.revSyncSocket.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.revSyncSocket.maxMessageIndex,
                                                                smem.revSyncSocket.messageIn, cUDP);
//don't do this        readJob.CheckLcn(&smem.revSyncSocket.maxMessageIndex,smem.revSyncSocket.messageIn);
                                readJob.DoWorkByMESSAGEIN(&smem.revSyncSocket.maxMessageIndex,
                                                          smem.revSyncSocket.messageIn);
                                parseAABB.EchoToGUI(&smem.revSyncSocket.maxMessageIndex, smem.revSyncSocket.messageIn,
                                                    "revSyncSocket");
                                parseAABB.MoveLastData(&smem.revSyncSocket.maxMessageIndex,
                                                       &smem.revSyncSocket.lastPacketIndex,
                                                       smem.revSyncSocket.messageIn);
                            }
                        }
                    }
                }

                if (smem.amegidsSocket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.amegidsSocket.Getfd(), &readfs)) {
                        readSelectLength = smem.amegidsSocket.UdpRead();
                        if (readSelectLength > 0) {

                            printf("Get Amegids:");

                            for (int ii = 0; ii < readSelectLength; ii++) {
                                printf("%X ", smem.amegidsSocket.block[ii]);
                            }
                            printf("\n");

                            if (smem.vGetCommEnable() == true) {

                                parseAABB.ParseBlock(readSelectLength, smem.amegidsSocket.block,
                                                     smem.amegidsSocket.messageIn, &smem.amegidsSocket.lastPacketIndex,
                                                     &smem.amegidsSocket.maxMessageIndex);
                                parseAABB.CheckSum(&smem.amegidsSocket.maxMessageIndex, smem.amegidsSocket.messageIn);
                                //enable vJudgeProtocolAndCheckLength
                                parseAABB.vJudgeProtocolAndCheckLength(&smem.amegidsSocket.maxMessageIndex,
                                                                       smem.amegidsSocket.messageIn);
                                parseAABB.DecideProtocol(&smem.amegidsSocket.maxMessageIndex,
                                                         smem.amegidsSocket.messageIn,
                                                         smem.amegidsSocket.GetConnDevice());
                                parseAABB.CheckReasonable(&smem.amegidsSocket.maxMessageIndex,
                                                          smem.amegidsSocket.messageIn);
                                parseAABB.AssignLcn(&smem.amegidsSocket.maxMessageIndex, smem.amegidsSocket.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.amegidsSocket.maxMessageIndex,
                                                                smem.amegidsSocket.messageIn, cUDP);
//                      readJob.vCheckIfBcastingForwardToUDP(&smem.amegidsSocket.maxMessageIndex,smem.amegidsSocket.messageIn);
                                readJob.CheckLcn(&smem.amegidsSocket.maxMessageIndex, smem.amegidsSocket.messageIn);
//                      readJob.SetCenterComm(&smem.amegidsSocket.maxMessageIndex,smem.amegidsSocket.messageIn);
                                readJob.DoWorkByMESSAGEIN(&smem.amegidsSocket.maxMessageIndex,
                                                          smem.amegidsSocket.messageIn);
                                parseAABB.EchoToGUI(&smem.amegidsSocket.maxMessageIndex, smem.amegidsSocket.messageIn,
                                                    "192.168.1.101:6003");
                                parseAABB.MoveLastData(&smem.amegidsSocket.maxMessageIndex,
                                                       &smem.amegidsSocket.lastPacketIndex,
                                                       smem.amegidsSocket.messageIn);

                            }


                        }

                    }
                }


                if (smem.SSVD01Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.SSVD01Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.SSVD01Socket.UdpRead();
                        if (readSelectLength > 0) {
                            parseAABB.ParseBlock(readSelectLength, smem.SSVD01Socket.block, smem.SSVD01Socket.messageIn,
                                                 &smem.SSVD01Socket.lastPacketIndex,
                                                 &smem.SSVD01Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD01Socket.maxMessageIndex, smem.SSVD01Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD01Socket.maxMessageIndex, smem.SSVD01Socket.messageIn,
                                                     smem.SSVD01Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD01Socket.maxMessageIndex,
                                                                   smem.SSVD01Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD01Socket.maxMessageIndex, smem.SSVD01Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD01Socket.maxMessageIndex, smem.SSVD01Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD01Socket.maxMessageIndex,
                                                            smem.SSVD01Socket.messageIn, cUDP);

//OT Pass
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD01Socket.maxMessageIndex,
                                                               smem.SSVD01Socket.messageIn, cVD01ToCenter);
//                      readJob.DoSendUDP_MESSAGEIN(&smem.SSVD01Socket.maxMessageIndex,smem.SSVD01Socket.messageIn);
//                      readJob.DoWorkByMESSAGEIN(&smem.SSVD01Socket.maxMessageIndex,smem.centerSocket.messageIn);

                            parseAABB.EchoToGUI(&smem.SSVD01Socket.maxMessageIndex, smem.SSVD01Socket.messageIn,
                                                "SSVD01");
                            parseAABB.MoveLastData(&smem.SSVD01Socket.maxMessageIndex,
                                                   &smem.SSVD01Socket.lastPacketIndex, smem.SSVD01Socket.messageIn);

//                    //OTMARKPRINTF  printf("[MESSAGE (Should not receive this port)] Forwarding SSVD01 to Center: %3X\n", smem.SSVD01Socket.block);
//                      writeJob.WritePhysicalOut(smem.SSVD01Socket.block, readSelectLength, DEVICECENTER92);
                        }
                    }
                }

                if (smem.SSVD02Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.SSVD02Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.SSVD02Socket.UdpRead();
                        if (readSelectLength > 0) {
                            parseAABB.ParseBlock(readSelectLength, smem.SSVD02Socket.block, smem.SSVD02Socket.messageIn,
                                                 &smem.SSVD02Socket.lastPacketIndex,
                                                 &smem.SSVD02Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD02Socket.maxMessageIndex, smem.SSVD02Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD02Socket.maxMessageIndex, smem.SSVD02Socket.messageIn,
                                                     smem.SSVD02Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD02Socket.maxMessageIndex,
                                                                   smem.SSVD02Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD02Socket.maxMessageIndex, smem.SSVD02Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD02Socket.maxMessageIndex, smem.SSVD02Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD02Socket.maxMessageIndex,
                                                            smem.SSVD02Socket.messageIn, cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD02Socket.maxMessageIndex,
                                                               smem.SSVD02Socket.messageIn, cVD02ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD02Socket.maxMessageIndex, smem.SSVD02Socket.messageIn,
                                                "SSVD02");
                            parseAABB.MoveLastData(&smem.SSVD02Socket.maxMessageIndex,
                                                   &smem.SSVD02Socket.lastPacketIndex, smem.SSVD02Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD03Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.SSVD03Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.SSVD03Socket.UdpRead();
                        if (readSelectLength > 0) {
                            parseAABB.ParseBlock(readSelectLength, smem.SSVD03Socket.block, smem.SSVD03Socket.messageIn,
                                                 &smem.SSVD03Socket.lastPacketIndex,
                                                 &smem.SSVD03Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD03Socket.maxMessageIndex, smem.SSVD03Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD03Socket.maxMessageIndex, smem.SSVD03Socket.messageIn,
                                                     smem.SSVD03Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD03Socket.maxMessageIndex,
                                                                   smem.SSVD03Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD03Socket.maxMessageIndex, smem.SSVD03Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD03Socket.maxMessageIndex, smem.SSVD03Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD03Socket.maxMessageIndex,
                                                            smem.SSVD03Socket.messageIn, cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD03Socket.maxMessageIndex,
                                                               smem.SSVD03Socket.messageIn, cVD03ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD03Socket.maxMessageIndex, smem.SSVD03Socket.messageIn,
                                                "SSVD03");
                            parseAABB.MoveLastData(&smem.SSVD03Socket.maxMessageIndex,
                                                   &smem.SSVD03Socket.lastPacketIndex, smem.SSVD03Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD04Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.SSVD04Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.SSVD04Socket.UdpRead();
                        if (readSelectLength > 0) {
                            parseAABB.ParseBlock(readSelectLength, smem.SSVD04Socket.block, smem.SSVD04Socket.messageIn,
                                                 &smem.SSVD04Socket.lastPacketIndex,
                                                 &smem.SSVD04Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD04Socket.maxMessageIndex, smem.SSVD04Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD04Socket.maxMessageIndex, smem.SSVD04Socket.messageIn,
                                                     smem.SSVD04Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD04Socket.maxMessageIndex,
                                                                   smem.SSVD04Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD04Socket.maxMessageIndex, smem.SSVD04Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD04Socket.maxMessageIndex, smem.SSVD04Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD04Socket.maxMessageIndex,
                                                            smem.SSVD04Socket.messageIn, cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD04Socket.maxMessageIndex,
                                                               smem.SSVD04Socket.messageIn, cVD04ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD04Socket.maxMessageIndex, smem.SSVD04Socket.messageIn,
                                                "SSVD04");
                            parseAABB.MoveLastData(&smem.SSVD04Socket.maxMessageIndex,
                                                   &smem.SSVD04Socket.lastPacketIndex, smem.SSVD04Socket.messageIn);
                        }
                    }
                }
//OT Pass
                if (smem.SSVD05Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.SSVD05Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.SSVD05Socket.UdpRead();
                        if (readSelectLength > 0) {
                            parseAABB.ParseBlock(readSelectLength, smem.SSVD05Socket.block, smem.SSVD05Socket.messageIn,
                                                 &smem.SSVD05Socket.lastPacketIndex,
                                                 &smem.SSVD05Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD05Socket.maxMessageIndex, smem.SSVD05Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD05Socket.maxMessageIndex, smem.SSVD05Socket.messageIn,
                                                     smem.SSVD05Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD05Socket.maxMessageIndex,
                                                                   smem.SSVD05Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD05Socket.maxMessageIndex, smem.SSVD05Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD05Socket.maxMessageIndex, smem.SSVD05Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD05Socket.maxMessageIndex,
                                                            smem.SSVD05Socket.messageIn, cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD05Socket.maxMessageIndex,
                                                               smem.SSVD05Socket.messageIn, cVD05ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD05Socket.maxMessageIndex, smem.SSVD05Socket.messageIn,
                                                "SSVD05");
                            parseAABB.MoveLastData(&smem.SSVD05Socket.maxMessageIndex,
                                                   &smem.SSVD05Socket.lastPacketIndex, smem.SSVD05Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD06Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.SSVD06Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.SSVD06Socket.UdpRead();
                        if (readSelectLength > 0) {
                            parseAABB.ParseBlock(readSelectLength, smem.SSVD06Socket.block, smem.SSVD06Socket.messageIn,
                                                 &smem.SSVD06Socket.lastPacketIndex,
                                                 &smem.SSVD06Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD06Socket.maxMessageIndex, smem.SSVD06Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD06Socket.maxMessageIndex, smem.SSVD06Socket.messageIn,
                                                     smem.SSVD06Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD06Socket.maxMessageIndex,
                                                                   smem.SSVD06Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD06Socket.maxMessageIndex, smem.SSVD06Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD06Socket.maxMessageIndex, smem.SSVD06Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD06Socket.maxMessageIndex,
                                                            smem.SSVD06Socket.messageIn, cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD06Socket.maxMessageIndex,
                                                               smem.SSVD06Socket.messageIn, cVD06ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD06Socket.maxMessageIndex, smem.SSVD06Socket.messageIn,
                                                "SSVD06");
                            parseAABB.MoveLastData(&smem.SSVD06Socket.maxMessageIndex,
                                                   &smem.SSVD06Socket.lastPacketIndex, smem.SSVD06Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD07Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.SSVD07Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.SSVD07Socket.UdpRead();
                        if (readSelectLength > 0) {
                            parseAABB.ParseBlock(readSelectLength, smem.SSVD07Socket.block, smem.SSVD07Socket.messageIn,
                                                 &smem.SSVD07Socket.lastPacketIndex,
                                                 &smem.SSVD07Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD07Socket.maxMessageIndex, smem.SSVD07Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD07Socket.maxMessageIndex, smem.SSVD07Socket.messageIn,
                                                     smem.SSVD07Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD07Socket.maxMessageIndex,
                                                                   smem.SSVD07Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD07Socket.maxMessageIndex, smem.SSVD07Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD07Socket.maxMessageIndex, smem.SSVD07Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD07Socket.maxMessageIndex,
                                                            smem.SSVD07Socket.messageIn, cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD07Socket.maxMessageIndex,
                                                               smem.SSVD07Socket.messageIn, cVD07ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD07Socket.maxMessageIndex, smem.SSVD07Socket.messageIn,
                                                "SSVD07");
                            parseAABB.MoveLastData(&smem.SSVD07Socket.maxMessageIndex,
                                                   &smem.SSVD07Socket.lastPacketIndex, smem.SSVD07Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD08Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.SSVD08Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.SSVD08Socket.UdpRead();
                        if (readSelectLength > 0) {
                            parseAABB.ParseBlock(readSelectLength, smem.SSVD08Socket.block, smem.SSVD08Socket.messageIn,
                                                 &smem.SSVD08Socket.lastPacketIndex,
                                                 &smem.SSVD08Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD08Socket.maxMessageIndex, smem.SSVD08Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD08Socket.maxMessageIndex, smem.SSVD08Socket.messageIn,
                                                     smem.SSVD08Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD08Socket.maxMessageIndex,
                                                                   smem.SSVD08Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD08Socket.maxMessageIndex, smem.SSVD08Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD08Socket.maxMessageIndex, smem.SSVD08Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD08Socket.maxMessageIndex,
                                                            smem.SSVD08Socket.messageIn, cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD08Socket.maxMessageIndex,
                                                               smem.SSVD08Socket.messageIn, cVD08ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD08Socket.maxMessageIndex, smem.SSVD08Socket.messageIn,
                                                "SSVD08");
                            parseAABB.MoveLastData(&smem.SSVD08Socket.maxMessageIndex,
                                                   &smem.SSVD08Socket.lastPacketIndex, smem.SSVD08Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD09Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.SSVD09Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.SSVD09Socket.UdpRead();
                        if (readSelectLength > 0) {
                            parseAABB.ParseBlock(readSelectLength, smem.SSVD09Socket.block, smem.SSVD09Socket.messageIn,
                                                 &smem.SSVD09Socket.lastPacketIndex,
                                                 &smem.SSVD09Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD09Socket.maxMessageIndex, smem.SSVD09Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD09Socket.maxMessageIndex, smem.SSVD09Socket.messageIn,
                                                     smem.SSVD09Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD09Socket.maxMessageIndex,
                                                                   smem.SSVD09Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD09Socket.maxMessageIndex, smem.SSVD09Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD09Socket.maxMessageIndex, smem.SSVD09Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD09Socket.maxMessageIndex,
                                                            smem.SSVD09Socket.messageIn, cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD09Socket.maxMessageIndex,
                                                               smem.SSVD09Socket.messageIn, cVD09ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD09Socket.maxMessageIndex, smem.SSVD09Socket.messageIn,
                                                "SSVD09");
                            parseAABB.MoveLastData(&smem.SSVD09Socket.maxMessageIndex,
                                                   &smem.SSVD09Socket.lastPacketIndex, smem.SSVD09Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD10Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.SSVD10Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.SSVD10Socket.UdpRead();
                        if (readSelectLength > 0) {
                            parseAABB.ParseBlock(readSelectLength, smem.SSVD10Socket.block, smem.SSVD10Socket.messageIn,
                                                 &smem.SSVD10Socket.lastPacketIndex,
                                                 &smem.SSVD10Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD10Socket.maxMessageIndex, smem.SSVD10Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD10Socket.maxMessageIndex, smem.SSVD10Socket.messageIn,
                                                     smem.SSVD10Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD10Socket.maxMessageIndex,
                                                                   smem.SSVD10Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD10Socket.maxMessageIndex, smem.SSVD10Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD10Socket.maxMessageIndex, smem.SSVD10Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD10Socket.maxMessageIndex,
                                                            smem.SSVD10Socket.messageIn, cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD10Socket.maxMessageIndex,
                                                               smem.SSVD10Socket.messageIn, cVD10ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD10Socket.maxMessageIndex, smem.SSVD10Socket.messageIn,
                                                "SSVD10");
                            parseAABB.MoveLastData(&smem.SSVD10Socket.maxMessageIndex,
                                                   &smem.SSVD10Socket.lastPacketIndex, smem.SSVD10Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD11Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.SSVD11Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.SSVD11Socket.UdpRead();
                        if (readSelectLength > 0) {
                            parseAABB.ParseBlock(readSelectLength, smem.SSVD11Socket.block, smem.SSVD11Socket.messageIn,
                                                 &smem.SSVD11Socket.lastPacketIndex,
                                                 &smem.SSVD11Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD11Socket.maxMessageIndex, smem.SSVD11Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD11Socket.maxMessageIndex, smem.SSVD11Socket.messageIn,
                                                     smem.SSVD11Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD11Socket.maxMessageIndex,
                                                                   smem.SSVD11Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD11Socket.maxMessageIndex, smem.SSVD11Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD11Socket.maxMessageIndex, smem.SSVD11Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD11Socket.maxMessageIndex,
                                                            smem.SSVD11Socket.messageIn, cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD11Socket.maxMessageIndex,
                                                               smem.SSVD11Socket.messageIn, cVD11ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD11Socket.maxMessageIndex, smem.SSVD11Socket.messageIn,
                                                "SSVD11");
                            parseAABB.MoveLastData(&smem.SSVD11Socket.maxMessageIndex,
                                                   &smem.SSVD11Socket.lastPacketIndex, smem.SSVD11Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD12Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.SSVD12Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.SSVD12Socket.UdpRead();
                        if (readSelectLength > 0) {
                            parseAABB.ParseBlock(readSelectLength, smem.SSVD12Socket.block, smem.SSVD12Socket.messageIn,
                                                 &smem.SSVD12Socket.lastPacketIndex,
                                                 &smem.SSVD12Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD12Socket.maxMessageIndex, smem.SSVD12Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD12Socket.maxMessageIndex, smem.SSVD12Socket.messageIn,
                                                     smem.SSVD12Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD12Socket.maxMessageIndex,
                                                                   smem.SSVD12Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD12Socket.maxMessageIndex, smem.SSVD12Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD12Socket.maxMessageIndex, smem.SSVD12Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD12Socket.maxMessageIndex,
                                                            smem.SSVD12Socket.messageIn, cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD12Socket.maxMessageIndex,
                                                               smem.SSVD12Socket.messageIn, cVD12ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD12Socket.maxMessageIndex, smem.SSVD12Socket.messageIn,
                                                "SSVD12");
                            parseAABB.MoveLastData(&smem.SSVD12Socket.maxMessageIndex,
                                                   &smem.SSVD12Socket.lastPacketIndex, smem.SSVD12Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD13Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.SSVD13Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.SSVD13Socket.UdpRead();
                        if (readSelectLength > 0) {
                            parseAABB.ParseBlock(readSelectLength, smem.SSVD13Socket.block, smem.SSVD13Socket.messageIn,
                                                 &smem.SSVD13Socket.lastPacketIndex,
                                                 &smem.SSVD13Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD13Socket.maxMessageIndex, smem.SSVD13Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD13Socket.maxMessageIndex, smem.SSVD13Socket.messageIn,
                                                     smem.SSVD13Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD13Socket.maxMessageIndex,
                                                                   smem.SSVD13Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD13Socket.maxMessageIndex, smem.SSVD13Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD13Socket.maxMessageIndex, smem.SSVD13Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD13Socket.maxMessageIndex,
                                                            smem.SSVD13Socket.messageIn, cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD13Socket.maxMessageIndex,
                                                               smem.SSVD13Socket.messageIn, cVD13ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD13Socket.maxMessageIndex, smem.SSVD13Socket.messageIn,
                                                "SSVD13");
                            parseAABB.MoveLastData(&smem.SSVD13Socket.maxMessageIndex,
                                                   &smem.SSVD13Socket.lastPacketIndex, smem.SSVD13Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD14Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.SSVD14Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.SSVD14Socket.UdpRead();
                        if (readSelectLength > 0) {
                            parseAABB.ParseBlock(readSelectLength, smem.SSVD14Socket.block, smem.SSVD14Socket.messageIn,
                                                 &smem.SSVD14Socket.lastPacketIndex,
                                                 &smem.SSVD14Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD14Socket.maxMessageIndex, smem.SSVD14Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD14Socket.maxMessageIndex, smem.SSVD14Socket.messageIn,
                                                     smem.SSVD14Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD14Socket.maxMessageIndex,
                                                                   smem.SSVD14Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD14Socket.maxMessageIndex, smem.SSVD14Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD14Socket.maxMessageIndex, smem.SSVD14Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD14Socket.maxMessageIndex,
                                                            smem.SSVD14Socket.messageIn, cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD14Socket.maxMessageIndex,
                                                               smem.SSVD14Socket.messageIn, cVD14ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD14Socket.maxMessageIndex, smem.SSVD14Socket.messageIn,
                                                "SSVD14");
                            parseAABB.MoveLastData(&smem.SSVD14Socket.maxMessageIndex,
                                                   &smem.SSVD14Socket.lastPacketIndex, smem.SSVD14Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD15Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.SSVD15Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.SSVD15Socket.UdpRead();
                        if (readSelectLength > 0) {
                            parseAABB.ParseBlock(readSelectLength, smem.SSVD15Socket.block, smem.SSVD15Socket.messageIn,
                                                 &smem.SSVD15Socket.lastPacketIndex,
                                                 &smem.SSVD15Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD15Socket.maxMessageIndex, smem.SSVD15Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD15Socket.maxMessageIndex, smem.SSVD15Socket.messageIn,
                                                     smem.SSVD15Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD15Socket.maxMessageIndex,
                                                                   smem.SSVD15Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD15Socket.maxMessageIndex, smem.SSVD15Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD15Socket.maxMessageIndex, smem.SSVD15Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD15Socket.maxMessageIndex,
                                                            smem.SSVD15Socket.messageIn, cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD15Socket.maxMessageIndex,
                                                               smem.SSVD15Socket.messageIn, cVD15ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD15Socket.maxMessageIndex, smem.SSVD15Socket.messageIn,
                                                "SSVD15");
                            parseAABB.MoveLastData(&smem.SSVD15Socket.maxMessageIndex,
                                                   &smem.SSVD15Socket.lastPacketIndex, smem.SSVD15Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD16Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.SSVD16Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.SSVD16Socket.UdpRead();
                        if (readSelectLength > 0) {
                            parseAABB.ParseBlock(readSelectLength, smem.SSVD16Socket.block, smem.SSVD16Socket.messageIn,
                                                 &smem.SSVD16Socket.lastPacketIndex,
                                                 &smem.SSVD16Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD16Socket.maxMessageIndex, smem.SSVD16Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD16Socket.maxMessageIndex, smem.SSVD16Socket.messageIn,
                                                     smem.SSVD16Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD16Socket.maxMessageIndex,
                                                                   smem.SSVD16Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD16Socket.maxMessageIndex, smem.SSVD16Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD16Socket.maxMessageIndex, smem.SSVD16Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD16Socket.maxMessageIndex,
                                                            smem.SSVD16Socket.messageIn, cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD16Socket.maxMessageIndex,
                                                               smem.SSVD16Socket.messageIn, cVD16ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD16Socket.maxMessageIndex, smem.SSVD16Socket.messageIn,
                                                "SSVD16");
                            parseAABB.MoveLastData(&smem.SSVD16Socket.maxMessageIndex,
                                                   &smem.SSVD16Socket.lastPacketIndex, smem.SSVD16Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD17Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.SSVD17Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.SSVD17Socket.UdpRead();
                        if (readSelectLength > 0) {
                            parseAABB.ParseBlock(readSelectLength, smem.SSVD17Socket.block, smem.SSVD17Socket.messageIn,
                                                 &smem.SSVD17Socket.lastPacketIndex,
                                                 &smem.SSVD17Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD17Socket.maxMessageIndex, smem.SSVD17Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD17Socket.maxMessageIndex, smem.SSVD17Socket.messageIn,
                                                     smem.SSVD17Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD17Socket.maxMessageIndex,
                                                                   smem.SSVD17Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD17Socket.maxMessageIndex, smem.SSVD17Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD17Socket.maxMessageIndex, smem.SSVD17Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD17Socket.maxMessageIndex,
                                                            smem.SSVD17Socket.messageIn, cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD17Socket.maxMessageIndex,
                                                               smem.SSVD17Socket.messageIn, cVD17ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD17Socket.maxMessageIndex, smem.SSVD17Socket.messageIn,
                                                "SSVD17");
                            parseAABB.MoveLastData(&smem.SSVD17Socket.maxMessageIndex,
                                                   &smem.SSVD17Socket.lastPacketIndex, smem.SSVD17Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD18Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.SSVD18Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.SSVD18Socket.UdpRead();
                        if (readSelectLength > 0) {
                            parseAABB.ParseBlock(readSelectLength, smem.SSVD18Socket.block, smem.SSVD18Socket.messageIn,
                                                 &smem.SSVD18Socket.lastPacketIndex,
                                                 &smem.SSVD18Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD18Socket.maxMessageIndex, smem.SSVD18Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD18Socket.maxMessageIndex, smem.SSVD18Socket.messageIn,
                                                     smem.SSVD18Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD18Socket.maxMessageIndex,
                                                                   smem.SSVD18Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD18Socket.maxMessageIndex, smem.SSVD18Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD18Socket.maxMessageIndex, smem.SSVD18Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD18Socket.maxMessageIndex,
                                                            smem.SSVD18Socket.messageIn, cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD18Socket.maxMessageIndex,
                                                               smem.SSVD18Socket.messageIn, cVD18ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD18Socket.maxMessageIndex, smem.SSVD18Socket.messageIn,
                                                "SSVD18");
                            parseAABB.MoveLastData(&smem.SSVD18Socket.maxMessageIndex,
                                                   &smem.SSVD18Socket.lastPacketIndex, smem.SSVD18Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD19Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.SSVD19Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.SSVD19Socket.UdpRead();
                        if (readSelectLength > 0) {
                            parseAABB.ParseBlock(readSelectLength, smem.SSVD19Socket.block, smem.SSVD19Socket.messageIn,
                                                 &smem.SSVD19Socket.lastPacketIndex,
                                                 &smem.SSVD19Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD19Socket.maxMessageIndex, smem.SSVD19Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD19Socket.maxMessageIndex, smem.SSVD19Socket.messageIn,
                                                     smem.SSVD19Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD19Socket.maxMessageIndex,
                                                                   smem.SSVD19Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD19Socket.maxMessageIndex, smem.SSVD19Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD19Socket.maxMessageIndex, smem.SSVD19Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD19Socket.maxMessageIndex,
                                                            smem.SSVD19Socket.messageIn, cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD19Socket.maxMessageIndex,
                                                               smem.SSVD19Socket.messageIn, cVD19ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD19Socket.maxMessageIndex, smem.SSVD19Socket.messageIn,
                                                "SSVD19");
                            parseAABB.MoveLastData(&smem.SSVD19Socket.maxMessageIndex,
                                                   &smem.SSVD19Socket.lastPacketIndex, smem.SSVD19Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD20Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.SSVD20Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.SSVD20Socket.UdpRead();
                        if (readSelectLength > 0) {
                            parseAABB.ParseBlock(readSelectLength, smem.SSVD20Socket.block, smem.SSVD20Socket.messageIn,
                                                 &smem.SSVD20Socket.lastPacketIndex,
                                                 &smem.SSVD20Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD20Socket.maxMessageIndex, smem.SSVD20Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD20Socket.maxMessageIndex, smem.SSVD20Socket.messageIn,
                                                     smem.SSVD20Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD20Socket.maxMessageIndex,
                                                                   smem.SSVD20Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD20Socket.maxMessageIndex, smem.SSVD20Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD20Socket.maxMessageIndex, smem.SSVD20Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD20Socket.maxMessageIndex,
                                                            smem.SSVD20Socket.messageIn, cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD20Socket.maxMessageIndex,
                                                               smem.SSVD20Socket.messageIn, cVD20ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD20Socket.maxMessageIndex, smem.SSVD20Socket.messageIn,
                                                "SSVD20");
                            parseAABB.MoveLastData(&smem.SSVD20Socket.maxMessageIndex,
                                                   &smem.SSVD20Socket.lastPacketIndex, smem.SSVD20Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD21Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.SSVD21Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.SSVD21Socket.UdpRead();
                        if (readSelectLength > 0) {
                            parseAABB.ParseBlock(readSelectLength, smem.SSVD21Socket.block, smem.SSVD21Socket.messageIn,
                                                 &smem.SSVD21Socket.lastPacketIndex,
                                                 &smem.SSVD21Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD21Socket.maxMessageIndex, smem.SSVD21Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD21Socket.maxMessageIndex, smem.SSVD21Socket.messageIn,
                                                     smem.SSVD21Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD21Socket.maxMessageIndex,
                                                                   smem.SSVD21Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD21Socket.maxMessageIndex, smem.SSVD21Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD21Socket.maxMessageIndex, smem.SSVD21Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD21Socket.maxMessageIndex,
                                                            smem.SSVD21Socket.messageIn, cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD21Socket.maxMessageIndex,
                                                               smem.SSVD21Socket.messageIn, cVD21ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD21Socket.maxMessageIndex, smem.SSVD21Socket.messageIn,
                                                "SSVD21");
                            parseAABB.MoveLastData(&smem.SSVD21Socket.maxMessageIndex,
                                                   &smem.SSVD21Socket.lastPacketIndex, smem.SSVD21Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD22Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.SSVD22Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.SSVD22Socket.UdpRead();
                        if (readSelectLength > 0) {
                            parseAABB.ParseBlock(readSelectLength, smem.SSVD22Socket.block, smem.SSVD22Socket.messageIn,
                                                 &smem.SSVD22Socket.lastPacketIndex,
                                                 &smem.SSVD22Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD22Socket.maxMessageIndex, smem.SSVD22Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD22Socket.maxMessageIndex, smem.SSVD22Socket.messageIn,
                                                     smem.SSVD22Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD22Socket.maxMessageIndex,
                                                                   smem.SSVD22Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD22Socket.maxMessageIndex, smem.SSVD22Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD22Socket.maxMessageIndex, smem.SSVD22Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD22Socket.maxMessageIndex,
                                                            smem.SSVD22Socket.messageIn, cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD22Socket.maxMessageIndex,
                                                               smem.SSVD22Socket.messageIn, cVD22ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD22Socket.maxMessageIndex, smem.SSVD22Socket.messageIn,
                                                "SSVD22");
                            parseAABB.MoveLastData(&smem.SSVD22Socket.maxMessageIndex,
                                                   &smem.SSVD22Socket.lastPacketIndex, smem.SSVD22Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD23Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.SSVD23Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.SSVD23Socket.UdpRead();
                        if (readSelectLength > 0) {
                            parseAABB.ParseBlock(readSelectLength, smem.SSVD23Socket.block, smem.SSVD23Socket.messageIn,
                                                 &smem.SSVD23Socket.lastPacketIndex,
                                                 &smem.SSVD23Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD23Socket.maxMessageIndex, smem.SSVD23Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD23Socket.maxMessageIndex, smem.SSVD23Socket.messageIn,
                                                     smem.SSVD23Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD23Socket.maxMessageIndex,
                                                                   smem.SSVD23Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD23Socket.maxMessageIndex, smem.SSVD23Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD23Socket.maxMessageIndex, smem.SSVD23Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD23Socket.maxMessageIndex,
                                                            smem.SSVD23Socket.messageIn, cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD23Socket.maxMessageIndex,
                                                               smem.SSVD23Socket.messageIn, cVD23ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD23Socket.maxMessageIndex, smem.SSVD23Socket.messageIn,
                                                "SSVD23");
                            parseAABB.MoveLastData(&smem.SSVD23Socket.maxMessageIndex,
                                                   &smem.SSVD23Socket.lastPacketIndex, smem.SSVD23Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD24Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.SSVD24Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.SSVD24Socket.UdpRead();
                        if (readSelectLength > 0) {
                            parseAABB.ParseBlock(readSelectLength, smem.SSVD24Socket.block, smem.SSVD24Socket.messageIn,
                                                 &smem.SSVD24Socket.lastPacketIndex,
                                                 &smem.SSVD24Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD24Socket.maxMessageIndex, smem.SSVD24Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD24Socket.maxMessageIndex, smem.SSVD24Socket.messageIn,
                                                     smem.SSVD24Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD24Socket.maxMessageIndex,
                                                                   smem.SSVD24Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD24Socket.maxMessageIndex, smem.SSVD24Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD24Socket.maxMessageIndex, smem.SSVD24Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD24Socket.maxMessageIndex,
                                                            smem.SSVD24Socket.messageIn, cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD24Socket.maxMessageIndex,
                                                               smem.SSVD24Socket.messageIn, cVD24ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD24Socket.maxMessageIndex, smem.SSVD24Socket.messageIn,
                                                "SSVD24");
                            parseAABB.MoveLastData(&smem.SSVD24Socket.maxMessageIndex,
                                                   &smem.SSVD24Socket.lastPacketIndex, smem.SSVD24Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD25Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.SSVD25Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.SSVD25Socket.UdpRead();
                        if (readSelectLength > 0) {
                            parseAABB.ParseBlock(readSelectLength, smem.SSVD25Socket.block, smem.SSVD25Socket.messageIn,
                                                 &smem.SSVD25Socket.lastPacketIndex,
                                                 &smem.SSVD25Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD25Socket.maxMessageIndex, smem.SSVD25Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD25Socket.maxMessageIndex, smem.SSVD25Socket.messageIn,
                                                     smem.SSVD25Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD25Socket.maxMessageIndex,
                                                                   smem.SSVD25Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD25Socket.maxMessageIndex, smem.SSVD25Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD25Socket.maxMessageIndex, smem.SSVD25Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD25Socket.maxMessageIndex,
                                                            smem.SSVD25Socket.messageIn, cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD25Socket.maxMessageIndex,
                                                               smem.SSVD25Socket.messageIn, cVD25ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD25Socket.maxMessageIndex, smem.SSVD25Socket.messageIn,
                                                "SSVD25");
                            parseAABB.MoveLastData(&smem.SSVD25Socket.maxMessageIndex,
                                                   &smem.SSVD25Socket.lastPacketIndex, smem.SSVD25Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD26Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.SSVD26Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.SSVD26Socket.UdpRead();
                        if (readSelectLength > 0) {
                            parseAABB.ParseBlock(readSelectLength, smem.SSVD26Socket.block, smem.SSVD26Socket.messageIn,
                                                 &smem.SSVD26Socket.lastPacketIndex,
                                                 &smem.SSVD26Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD26Socket.maxMessageIndex, smem.SSVD26Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD26Socket.maxMessageIndex, smem.SSVD26Socket.messageIn,
                                                     smem.SSVD26Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD26Socket.maxMessageIndex,
                                                                   smem.SSVD26Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD26Socket.maxMessageIndex, smem.SSVD26Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD26Socket.maxMessageIndex, smem.SSVD26Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD26Socket.maxMessageIndex,
                                                            smem.SSVD26Socket.messageIn, cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD26Socket.maxMessageIndex,
                                                               smem.SSVD26Socket.messageIn, cVD26ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD26Socket.maxMessageIndex, smem.SSVD26Socket.messageIn,
                                                "SSVD26");
                            parseAABB.MoveLastData(&smem.SSVD26Socket.maxMessageIndex,
                                                   &smem.SSVD26Socket.lastPacketIndex, smem.SSVD26Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD27Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.SSVD27Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.SSVD27Socket.UdpRead();
                        if (readSelectLength > 0) {
                            parseAABB.ParseBlock(readSelectLength, smem.SSVD27Socket.block, smem.SSVD27Socket.messageIn,
                                                 &smem.SSVD27Socket.lastPacketIndex,
                                                 &smem.SSVD27Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD27Socket.maxMessageIndex, smem.SSVD27Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD27Socket.maxMessageIndex, smem.SSVD27Socket.messageIn,
                                                     smem.SSVD27Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD27Socket.maxMessageIndex,
                                                                   smem.SSVD27Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD27Socket.maxMessageIndex, smem.SSVD27Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD27Socket.maxMessageIndex, smem.SSVD27Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD27Socket.maxMessageIndex,
                                                            smem.SSVD27Socket.messageIn, cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD27Socket.maxMessageIndex,
                                                               smem.SSVD27Socket.messageIn, cVD27ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD27Socket.maxMessageIndex, smem.SSVD27Socket.messageIn,
                                                "SSVD27");
                            parseAABB.MoveLastData(&smem.SSVD27Socket.maxMessageIndex,
                                                   &smem.SSVD27Socket.lastPacketIndex, smem.SSVD27Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD28Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.SSVD28Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.SSVD28Socket.UdpRead();
                        if (readSelectLength > 0) {
                            parseAABB.ParseBlock(readSelectLength, smem.SSVD28Socket.block, smem.SSVD28Socket.messageIn,
                                                 &smem.SSVD28Socket.lastPacketIndex,
                                                 &smem.SSVD28Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD28Socket.maxMessageIndex, smem.SSVD28Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD28Socket.maxMessageIndex, smem.SSVD28Socket.messageIn,
                                                     smem.SSVD28Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD28Socket.maxMessageIndex,
                                                                   smem.SSVD28Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD28Socket.maxMessageIndex, smem.SSVD28Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD28Socket.maxMessageIndex, smem.SSVD28Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD28Socket.maxMessageIndex,
                                                            smem.SSVD28Socket.messageIn, cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD28Socket.maxMessageIndex,
                                                               smem.SSVD28Socket.messageIn, cVD28ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD28Socket.maxMessageIndex, smem.SSVD28Socket.messageIn,
                                                "SSVD28");
                            parseAABB.MoveLastData(&smem.SSVD28Socket.maxMessageIndex,
                                                   &smem.SSVD28Socket.lastPacketIndex, smem.SSVD28Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD29Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.SSVD29Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.SSVD29Socket.UdpRead();
                        if (readSelectLength > 0) {
                            parseAABB.ParseBlock(readSelectLength, smem.SSVD29Socket.block, smem.SSVD29Socket.messageIn,
                                                 &smem.SSVD29Socket.lastPacketIndex,
                                                 &smem.SSVD29Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD29Socket.maxMessageIndex, smem.SSVD29Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD29Socket.maxMessageIndex, smem.SSVD29Socket.messageIn,
                                                     smem.SSVD29Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD29Socket.maxMessageIndex,
                                                                   smem.SSVD29Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD29Socket.maxMessageIndex, smem.SSVD29Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD29Socket.maxMessageIndex, smem.SSVD29Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD29Socket.maxMessageIndex,
                                                            smem.SSVD29Socket.messageIn, cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD29Socket.maxMessageIndex,
                                                               smem.SSVD29Socket.messageIn, cVD29ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD29Socket.maxMessageIndex, smem.SSVD29Socket.messageIn,
                                                "SSVD29");
                            parseAABB.MoveLastData(&smem.SSVD29Socket.maxMessageIndex,
                                                   &smem.SSVD29Socket.lastPacketIndex, smem.SSVD29Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD30Socket.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.SSVD30Socket.Getfd(), &readfs)) {
                        readSelectLength = smem.SSVD30Socket.UdpRead();
                        if (readSelectLength > 0) {
                            parseAABB.ParseBlock(readSelectLength, smem.SSVD30Socket.block, smem.SSVD30Socket.messageIn,
                                                 &smem.SSVD30Socket.lastPacketIndex,
                                                 &smem.SSVD30Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD30Socket.maxMessageIndex, smem.SSVD30Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD30Socket.maxMessageIndex, smem.SSVD30Socket.messageIn,
                                                     smem.SSVD30Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD30Socket.maxMessageIndex,
                                                                   smem.SSVD30Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD30Socket.maxMessageIndex, smem.SSVD30Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD30Socket.maxMessageIndex, smem.SSVD30Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD30Socket.maxMessageIndex,
                                                            smem.SSVD30Socket.messageIn, cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD30Socket.maxMessageIndex,
                                                               smem.SSVD30Socket.messageIn, cVD30ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD30Socket.maxMessageIndex, smem.SSVD30Socket.messageIn,
                                                "SSVD30");
                            parseAABB.MoveLastData(&smem.SSVD30Socket.maxMessageIndex,
                                                   &smem.SSVD30Socket.lastPacketIndex, smem.SSVD30Socket.messageIn);
                        }
                    }
                }
/*
            if (smem.SSVD31Socket.GetPortAlreadyOpen()) {
                if (FD_ISSET(smem.SSVD31Socket.Getfd(),&readfs)) {
                    readSelectLength=smem.SSVD31Socket.UdpRead();
                    if (readSelectLength>0) {
                      parseAABB.ParseBlock(readSelectLength,smem.SSVD31Socket.block,smem.SSVD31Socket.messageIn,&smem.SSVD31Socket.lastPacketIndex,&smem.SSVD31Socket.maxMessageIndex);
                           parseAABB.CheckSum(&smem.SSVD31Socket.maxMessageIndex,smem.SSVD31Socket.messageIn);
                      parseAABB.DecideProtocol(&smem.SSVD31Socket.maxMessageIndex,smem.SSVD31Socket.messageIn,smem.SSVD31Socket.GetConnDevice());
                      parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD31Socket.maxMessageIndex,smem.SSVD31Socket.messageIn);
                      parseAABB.CheckReasonable(&smem.SSVD31Socket.maxMessageIndex,smem.SSVD31Socket.messageIn);
                      parseAABB.AssignLcn(&smem.SSVD31Socket.maxMessageIndex,smem.SSVD31Socket.messageIn);
                      readJob.SetInterfaceAndReadFlag(&smem.SSVD31Socket.maxMessageIndex,smem.SSVD31Socket.messageIn,cUDP);
                      readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD31Socket.maxMessageIndex, smem.SSVD31Socket.messageIn, cVD31ToCenter);
                      parseAABB.EchoToGUI(&smem.SSVD31Socket.maxMessageIndex,smem.SSVD31Socket.messageIn,"SSVD31");
                      parseAABB.MoveLastData(&smem.SSVD31Socket.maxMessageIndex,&smem.SSVD31Socket.lastPacketIndex,smem.SSVD31Socket.messageIn);
                    }
                }
            }
            if (smem.SSVD32Socket.GetPortAlreadyOpen()) {
                if (FD_ISSET(smem.SSVD32Socket.Getfd(),&readfs)) {
                    readSelectLength=smem.SSVD32Socket.UdpRead();
                    if (readSelectLength>0) {
                      parseAABB.ParseBlock(readSelectLength,smem.SSVD32Socket.block,smem.SSVD32Socket.messageIn,&smem.SSVD32Socket.lastPacketIndex,&smem.SSVD32Socket.maxMessageIndex);
                           parseAABB.CheckSum(&smem.SSVD32Socket.maxMessageIndex,smem.SSVD32Socket.messageIn);
                      parseAABB.DecideProtocol(&smem.SSVD32Socket.maxMessageIndex,smem.SSVD32Socket.messageIn,smem.SSVD32Socket.GetConnDevice());
                      parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD32Socket.maxMessageIndex,smem.SSVD32Socket.messageIn);
                      parseAABB.CheckReasonable(&smem.SSVD32Socket.maxMessageIndex,smem.SSVD32Socket.messageIn);
                      parseAABB.AssignLcn(&smem.SSVD32Socket.maxMessageIndex,smem.SSVD32Socket.messageIn);
                      readJob.SetInterfaceAndReadFlag(&smem.SSVD32Socket.maxMessageIndex,smem.SSVD32Socket.messageIn,cUDP);
                      readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD32Socket.maxMessageIndex, smem.SSVD32Socket.messageIn, cVD32ToCenter);
                      parseAABB.EchoToGUI(&smem.SSVD32Socket.maxMessageIndex,smem.SSVD32Socket.messageIn,"SSVD32");
                      parseAABB.MoveLastData(&smem.SSVD32Socket.maxMessageIndex,&smem.SSVD32Socket.lastPacketIndex,smem.SSVD32Socket.messageIn);
                    }
                }
            }
*/

                if (smem.redCountPort.GetPortAlreadyOpen()) {
                    if (FD_ISSET(smem.redCountPort.Getfd(), &readfs)) {
                        readSelectLength = smem.redCountPort.Rs232Read();
                        if (readSelectLength > 0) {
                            if (smem.vGetCommEnable() == true) {  //OT20110728
//                        printf("Get RedCount Return Msg!\n");
                                parseAABB.ParseBlock(readSelectLength, smem.redCountPort.block,
                                                     smem.redCountPort.messageIn, &smem.redCountPort.lastPacketIndex,
                                                     &smem.redCountPort.maxMessageIndex);
                                parseAABB.CheckSum(&smem.redCountPort.maxMessageIndex, smem.redCountPort.messageIn);
                                parseAABB.DecideProtocol(&smem.redCountPort.maxMessageIndex,
                                                         smem.redCountPort.messageIn,
                                                         smem.redCountPort.GetConnDevice());
                                parseAABB.CheckReasonable(&smem.redCountPort.maxMessageIndex,
                                                          smem.redCountPort.messageIn);
                                readJob.vDoDisplay(&smem.redCountPort.maxMessageIndex, smem.redCountPort.messageIn);
                                parseAABB.EchoToGUI(&smem.redCountPort.maxMessageIndex, smem.redCountPort.messageIn,
                                                    "RedCount");
                                parseAABB.MoveLastData(&smem.redCountPort.maxMessageIndex,
                                                       &smem.redCountPort.lastPacketIndex, smem.redCountPort.messageIn);
                            }
                        }
                    }
                }

            }

        }

        //pthread_join(tidTimerHandle,NULL);                                          //?????t?@??thread


        //????RS232,422,485 ?q?T??
        if (smem.centerPort.CloseRs232Port()) printf("Close CenterPort Successful!!\n");
//    if (smem.ssPort.CloseRs232Port()) printf("Close SSPort Successful!!\n");
        if (smem.keypadPort.CloseRs232Port()) printf("Close KeypadPort Successful!!\n");
        if (smem.revLaneManualPort.CloseRs232Port()) printf("Close revLaneManualPort Successful!!\n");


        if (iCom2Type == Com2IsTesterPort) {      //?}??test??
        } else if (iCom2Type == Com2IsGPSPort || iCom2Type == Com2IsHOLUXGPSPort) {       //?}??GPS Port
            if (smem.NMEAPort.CloseRs232Port()) printf("Close NMEAPort Successful!!\n");
        } else if (iCom2Type == Com2IsPassingPort) {
            if (smem.passingPort.CloseRs232Port()) printf("Close passingPort Successful!!\n");
        }

//    if (smem.rtmsPort.CloseRs232Port()) printf("Close RtmsPort Successful!!\n");
        if (smem.redCountPort.CloseRs232Port()) printf("Close redCountPort Successful!!\n");


        //????UDP SOCKET ?q?T??
        if (smem.centerSocket.CloseUdpSocket()) printf("Close CenterSocket Successful!!\n");
//    if (smem.tempSocket.CloseUdpSocket()) printf("Close TempSocket Successful!!\n");
        if (smem.testerSocket.CloseUdpSocket()) printf("Close TesterSocket Successful!!\n");

/* OT ADD 950927 */
        if (smem.CCJDynCtlSocket.CloseUdpSocket()) printf("Close CCJDynCtlSocket Successful!!\n");
        if (smem.CCJDynCtl2Socket.CloseUdpSocket()) printf("Close CCJDynCtl2Socket Successful!!\n");
/*-- OT ADD 950927 */
        if (smem.DynCalServerInCrossSocket.CloseUdpSocket()) printf("Close DynCalServerInCrossSocket Successful!!\n");
        if (smem.DynCalServerInCenterSocket.CloseUdpSocket()) printf("Close DynCalServerInCenterSocket Successful!!\n");

//OT990618
        if (smem.revSyncSocket.CloseUdpSocket()) printf("Close revSyncSocket Successful!!\n");

        if (smem.amegidsSocket.CloseUdpSocket()) printf("Close amegidsSocket Successful!!\n");

        if (smem.SSVD01Socket.CloseUdpSocket()) printf("Close SSVD01Socket Successful!!\n");
        if (smem.SSVD02Socket.CloseUdpSocket()) printf("Close SSVD02Socket Successful!!\n");
        if (smem.SSVD03Socket.CloseUdpSocket()) printf("Close SSVD03Socket Successful!!\n");
        if (smem.SSVD04Socket.CloseUdpSocket()) printf("Close SSVD04Socket Successful!!\n");
//OT Pass
        smem.SSVD05Socket.CloseUdpSocket();
        smem.SSVD06Socket.CloseUdpSocket();
        smem.SSVD07Socket.CloseUdpSocket();
        smem.SSVD08Socket.CloseUdpSocket();
        smem.SSVD09Socket.CloseUdpSocket();
        smem.SSVD10Socket.CloseUdpSocket();
        smem.SSVD11Socket.CloseUdpSocket();
        smem.SSVD12Socket.CloseUdpSocket();
        smem.SSVD13Socket.CloseUdpSocket();
        smem.SSVD14Socket.CloseUdpSocket();
        smem.SSVD15Socket.CloseUdpSocket();
        smem.SSVD16Socket.CloseUdpSocket();
        smem.SSVD17Socket.CloseUdpSocket();
        smem.SSVD18Socket.CloseUdpSocket();
        smem.SSVD19Socket.CloseUdpSocket();
        smem.SSVD20Socket.CloseUdpSocket();
        smem.SSVD21Socket.CloseUdpSocket();
        smem.SSVD22Socket.CloseUdpSocket();
        smem.SSVD23Socket.CloseUdpSocket();
        smem.SSVD24Socket.CloseUdpSocket();
        smem.SSVD25Socket.CloseUdpSocket();
        smem.SSVD26Socket.CloseUdpSocket();
        smem.SSVD27Socket.CloseUdpSocket();
        smem.SSVD28Socket.CloseUdpSocket();
        smem.SSVD29Socket.CloseUdpSocket();
        smem.SSVD30Socket.CloseUdpSocket();
/*
    smem.SSVD31Socket.CloseUdpSocket();
    smem.SSVD32Socket.CloseUdpSocket();
*/

        //????IO ?q?T??
        lcd240x128.ReleaseAuthority();
        digitalIO.ReleaseAuthority();


        return 0;

    } catch (...) {}
}

//---------------------------------------------------------------------------
void *timerHandle(void *) {
}
//---------------------------------------------------------------------------


/*
            aaaaaaaaaa();
            if (smem.SSVD06Socket.GetPortAlreadyOpen()) {
                if (FD_ISSET(smem.SSVD06Socket.Getfd(),&readfs)) {
                    readSelectLength=smem.SSVD06Socket.UdpRead();
                    if (readSelectLength>0) {
                      parseAABB.ParseBlock(readSelectLength,smem.SSVD06Socket.block,smem.SSVD06Socket.messageIn,&smem.SSVD06Socket.lastPacketIndex,&smem.SSVD06Socket.maxMessageIndex);
                           parseAABB.CheckSum(&smem.SSVD06Socket.maxMessageIndex,smem.SSVD06Socket.messageIn);
                      parseAABB.DecideProtocol(&smem.SSVD06Socket.maxMessageIndex,smem.SSVD06Socket.messageIn,smem.SSVD06Socket.GetConnDevice());
                      parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD06Socket.maxMessageIndex,smem.SSVD06Socket.messageIn);
                      parseAABB.CheckReasonable(&smem.SSVD06Socket.maxMessageIndex,smem.SSVD06Socket.messageIn);
                      parseAABB.AssignLcn(&smem.SSVD06Socket.maxMessageIndex,smem.SSVD06Socket.messageIn);
                      readJob.SetInterfaceAndReadFlag(&smem.SSVD06Socket.maxMessageIndex,smem.SSVD06Socket.messageIn,cUDP);
                      readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD06Socket.maxMessageIndex, smem.SSVD06Socket.messageIn, cVD06ToCenter);
                      parseAABB.EchoToGUI(&smem.SSVD06Socket.maxMessageIndex,smem.SSVD06Socket.messageIn,"SSVD06");
                      parseAABB.MoveLastData(&smem.SSVD06Socket.maxMessageIndex,&smem.SSVD06Socket.lastPacketIndex,smem.SSVD06Socket.messageIn);
                    }
                }
            }
*/


