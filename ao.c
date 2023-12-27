#include "ao.h"

t_enStatusActiveObj    AO__Ctor(t_ActiveObj * const me, evtHandler dispatch);
t_enStatusActiveObj    AO__Post(t_ActiveObj * const me, void *pData);
t_enStatusActiveObj    AO__EvtPump(void *pObj);