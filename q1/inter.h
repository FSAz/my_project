#include "HT66F004.h"

#define	EMI_ENABLE()			(_emi = 1)
#define	EMI_DISABLE()			(_emi = 0)

#define	MF0E_ENABLE()			(_mf0e = 1)
#define	MF0E_DISABLE()			(_mf0e = 0)
#define	MF0F_CLEAR_FLAG()		(_mf0f = 0)