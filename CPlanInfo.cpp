#include "CPlanInfo.h"
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t CPlanInfo::_plan_mutex=PTHREAD_MUTEX_INITIALIZER;
//----------------------------------------------------------
CPlanInfo::CPlanInfo(void):_planid(0x99),_subphase_count(0)
{
}
//----------------------------------------------------------
CPlanInfo &CPlanInfo::operator=(const CPlanInfo &plan_info)
{
try{
  if(this!=&plan_info){

/*
    if(_ptr_subplaninfo){  //if(_ptr_subplaninfo!=NULL), so that this CPlanInfo is not empty
        if(_subphase_count==0) perror("ERROR: CPlanInfo::operator=\n");
      free(_ptr_subplaninfo);
      _planid=0x99; _subphase_count=0;
    }
*/

    _planid         = plan_info._planid;
    _dir            = plan_info._dir;
    _phase_order    = plan_info._phase_order;
    _subphase_count = plan_info._subphase_count;
    _cycle_time     = plan_info._cycle_time;
    _offset         = plan_info._offset;
    _cycle_time     = plan_info._cycle_time;

    _shorten_cycle = plan_info._shorten_cycle;

//mallocFuck    _ptr_subplaninfo = (SSubPlanInfo *)malloc(sizeof(SSubPlanInfo)*_subphase_count);
    for( int j=0; j<_subphase_count; j++ ){
      _ptr_subplaninfo[j]._green          = plan_info._ptr_subplaninfo[j]._green;
      _ptr_subplaninfo[j]._min_green      = plan_info._ptr_subplaninfo[j]._min_green;
      _ptr_subplaninfo[j]._max_green      = plan_info._ptr_subplaninfo[j]._max_green;
      _ptr_subplaninfo[j]._yellow         = plan_info._ptr_subplaninfo[j]._yellow;
      _ptr_subplaninfo[j]._allred         = plan_info._ptr_subplaninfo[j]._allred;
      _ptr_subplaninfo[j]._pedgreen_flash = plan_info._ptr_subplaninfo[j]._pedgreen_flash;
      _ptr_subplaninfo[j]._pedred         = plan_info._ptr_subplaninfo[j]._pedred;

      _ptr_subplaninfo[j]._green_compensation           = 0;
      _ptr_subplaninfo[j]._yellow_compensation          = 0;
      _ptr_subplaninfo[j]._allred_compensation          = 0;
      _ptr_subplaninfo[j]._pedgreen_flash_compensation  = 0;
      _ptr_subplaninfo[j]._pedred_compensation          = 0;
    }

  }
  return *this;
}
catch(...){ perror("ERROR: CPlanInfo::operator=\n");  return *this;}
}
//----------------------------------------------------------

