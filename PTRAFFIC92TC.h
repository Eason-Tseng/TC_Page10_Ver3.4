//---------------------------------------------------------------------------

#ifndef PTRAFFIC92TCH
#define PTRAFFIC92TCH

#include "var.h"
#include "SMEM.h"
//---------------------------------------------------------------------------
class PTRAFFIC92TC
{
    public:

        PTRAFFIC92TC(void);
        ~PTRAFFIC92TC(void);
        bool DoWorkViaPTraffic92(MESSAGEOK);                                    //???PTraffic92????T???@???

        bool vWriteControlStrategy5F10(MESSAGEOK);
        bool vQurryControlStrategy5F40(MESSAGEOK);  //with 5FC0

        bool vWriteReverseTimeData5F11(MESSAGEOK);
        bool vWriteReverseTimeData5F12(MESSAGEOK);
        bool vQueryReverseTimeData5F41(MESSAGEOK);
        bool vQueryReverseTimeData5F42(MESSAGEOK);


        bool vWritePlan5F14(MESSAGEOK);
        bool vQueryPlan5F44(MESSAGEOK);  //with 5FC4

        bool vWritePhase5F13(MESSAGEOK);
        bool vQueryPhase5F43(MESSAGEOK);  //with 5FC3

        bool vWritePlanDBMS5F15(MESSAGEOK);
        bool vQueryPlanDBMS5F45(MESSAGEOK);  //with 5FC5

        bool vWriteSegment5F16(MESSAGEOK);
        bool vQuerySegment5F46(MESSAGEOK);

        bool vWriteSpecialSegment5F17(MESSAGEOK);
        bool vQueryHolidaySegment5F47(MESSAGEOK);

        bool vWritePhaseByStep5F2F(MESSAGEOK);
//OT Debug 4010
        bool vQueryPhase5F5F(MESSAGEOK);
        void vLightBrightnessControl_5F3E(MESSAGEOK);
        void vQueryLightBrightness_5F6E(MESSAGEOK);


        bool vTransmitCycleSetting_5F3F(MESSAGEOK);
        bool vTransmitCycleQuery_5F6F(MESSAGEOK);

        bool vGoToNextPhaseStepControl_5F1C(MESSAGEOK);
        bool vQueryPhaseStepControl_5F4C(MESSAGEOK);

        bool vWriteRunPlan5F18(MESSAGEOK);
        bool vQueryRunPlan5F48(MESSAGEOK);

        bool vSetDynSegParameter(MESSAGEOK);
        bool vQueryDynSegParameter(MESSAGEOK);
        bool vSetDynSegSwitch(MESSAGEOK);
        void vReportLightBrightness_5FEE();


    private:
    int BrightnessStartHour;
    int BrightnessStartMin;
    int BrightnessEndHour;
    int BrightnessEndMin;
    int count;

    int getBrightnessStartHour() const;

    void setBrightnessStartHour(int brightnessStartHour);

    int getBrightnessStartMin() const;

    void setBrightnessStartMin(int brightnessStartMin);

    int getBrightnessEndHour() const;

    void setBrightnessEndHour(int brightnessEndHour);

    int getBrightnessEndMin() const;

    int GetNowPlanOfSegtypeCount(void);

    void setBrightnessEndMin(int brightnessEndMin);

    int vReturnToCenterACK(unsigned char, unsigned char);
        int vReturnToCenterNACK(unsigned char, unsigned char, unsigned char, unsigned char);

//   void Dyn_to_TOD_Step_set(unsigned short currentSubphaseStep,
//                            int i5F10_EffectTime);
};
//---------------------------------------------------------------------------
#endif

