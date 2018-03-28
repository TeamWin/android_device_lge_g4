## TWRP for LG G4 (ANY model)

Decryption is supported for:

* CM/LOS/AOSP based ROMs (running MM up to O - LP may work but is unsupported)
* STOCK based ROMs (N only)

This tree is unified for **all** known LG G4 models and the detection happens **automatically** when TWRP boots up.

Supported devices by this tree:

### Official unlocked
* H811
* H815 EUR

### UsU'd
_UsU'd_ means: unlocked by [UsU](https://bit.do/unlockg4)

* LS991
* F500
* H810
* H811
* H812
* H815 - any non EUR
* H815 EUR
* H819
* US991
* VS986


Prepare the sources from here: https://github.com/omnirom/android/tree/android-8.0

Add to `.repo/local_manifests/g4.xml`:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<manifest>
  <project name="android_external_busybox" path="external/busybox" remote="omnirom" revision="android-8.0" />
</manifest>
```

Then run `repo sync` to check it out.

To build:

```sh
source build/envsetup.sh
lunch omni_g4-eng
mka recoveryimage
```
(the lunch command may install additional ressources)

### TWRP included kernel

Add  to `.repo/local_manifests/g4_kernel.xml`:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<manifest>
  <remote  name="bitbucket"
           fetch="https://bitbucket.org/" />

  <project name="Suicide-Squirrel/Titan-Kernel-LG-G4" path="kernel/lge/titan" remote="github" revision="refs/heads/oreo-mr1-release_synced" />         
  <project name="steadfasterX/android_buildtools" path="vendor/sedi/prebuilt/bin" remote="github" revision="master" />
  <project name="matthewdalex/aarch64-linux-android-4.9" path="prebuilts/gcc/linux-x86/aarch64-linux-android-4.9-kernel" remote="bitbucket" revision="master" />
  <project name="xiaolu/mkbootimg_tools" path="prebuilts/devtools/mkbootimg_tools" remote="github" revision="master" />
</manifest>
```
Then run `repo sync -jX` to check it out.

To build the kernel run (all in 1 line):

`MAXCPU=13 JAVACBIN=undef NEEDEDJAVA=undef CONFIG_NO_ERROR_ON_MISMATCH=y BUILDID=lge/g4 KCONF=twrp_g4_defconfig UARCH=x64 KDIR=kernel/lge/titan vendor/sedi/prebuilt/bin/universalbuilder.sh kernelonly`

adjust MAXCPU to the amount of cores to use for building

The kernel and DTB will be generated and placed in device/lge/g4

