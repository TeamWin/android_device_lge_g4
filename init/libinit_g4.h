#ifndef _INIT_G4_COMMON_H
#define _INIT_G4_COMMON_H


// Rename vendor_load_properties() to real_vendor_load_properties()
// and let init_htcCommon.cpp call the real function as appropriate
/**
#ifdef __cplusplus
    // Allow usage of .c and/or .cpp (this is actually only needed in 6.0 where
    // vendor_init is a c file, but allows for real_vendor_load_properties() to
    // be in a .cpp file due to the #ifdef __cplusplus in vendor_init.h)
    extern "C" { extern void real_vendor_load_properties(void); }
#endif /* __cplusplus */

#define vendor_load_properties real_vendor_load_properties

// 8.0 no longer *needs* these defines but they are needed for backwards
// compatibly and have little/no effect on 8.0+'s no length limitation
#ifndef PROP_NAME_MAX
#define PROP_NAME_MAX   32
#endif

#ifndef PROP_VALUE_MAX
#define PROP_VALUE_MAX  92
#endif

#endif // _INIT_G4_COMMON_H
