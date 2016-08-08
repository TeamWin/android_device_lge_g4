## TWRP device tree for LG G4 (International)

Add to `.repo/local_manifests/h815.xml`:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<manifest>
	<project path="device/lge/h815" name="android_device_lge_h815" remote="TeamWin" revision="android-6.0" />
</manifest>
```

Then run `repo sync` to check it out.

To build:

```sh
. build/envsetup.sh
lunch omni_h815-eng
make -j5 recoveryimage
```

Kernel sources are available at: 

