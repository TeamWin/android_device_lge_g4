#ifndef _INIT_G4_COMMON_H
#define _INIT_G4_COMMON_H

#define CMDLINE_MODEL        "model.name="
#define CMDLINE_MODEL_LEN    (strlen(CMDLINE_MODEL))
#define DEVID_MAX 10
#define USU_MAX 10
#define USUOFFSET 3145722       // UsU offset
#define USUCOUNT 6

#define USUDEV		"/dev/block/bootdevice/by-name/raw_resources"
#define USUDEVPF        "/dev/block/platform/f9824900.sdhci/by-name/raw_resources"
#define USUDEVPFSOC     "/dev/block/platform/soc.0/f9824900.sdhci/by-name/raw_resources"

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
