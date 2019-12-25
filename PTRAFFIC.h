//---------------------------------------------------------------------------

#ifndef PTRAFFICH
#define PTRAFFICH

#include "var.h"
#include "PTRAFFIC77.h"
#include "PTRAFFIC87.h"
#include "PTRAFFIC92.h"
#include "PTRAFFICNTCIP.h"

#include "PTRAFFICCCJ.h"

#include "PTRAFFICREVSYNC.h"
#include "PTRAFFICAMEGIDS.h"

//---------------------------------------------------------------------------
class PTRAFFIC
{
    public:

        PTRAFFIC(void);
        ~PTRAFFIC(void);
        bool DoWorkViaProtocol(MESSAGEOK);                                      //根據protocol傳來的Structure作事情


    private:

        PTRAFFIC77 pTraffic77;
        PTRAFFIC87 pTraffic87;
        PTRAFFIC92 pTraffic92;
        PTRAFFICNTCIP pTrafficNtcip;

        PTRAFFICCCJ pTrafficCCJ;

        PTRAFFICREVSYNC pTrafficRevSync;
        PTRAFFICAMEGIDS pTrafficAmegids;

};
//---------------------------------------------------------------------------
#endif


