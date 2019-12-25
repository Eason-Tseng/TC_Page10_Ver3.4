#ifndef CStorINIH
#define CStorINIH

#include "inifile.h"

//--------------------------------------------------------------------------
class CStorINI
{
    public:
      CStorINI(void);
      ~CStorINI(void);

      int vLoadIntFromINI(char *, char *, int);
      bool vSaveIntToINI(char *, char *, int);

      float vLoadFloatFromINI(char *, char *, float);
      bool vSaveFloatToINI(char *, char *, float);

      bool vSaveStringToINI(char *, char *, char *);
      char *vLoadStringFromINI(char *, char *, char *);

    private:
      CIniFile ini;
};
//--------------------------------------------------------------------------
extern CStorINI oStorINI;
#endif
