# Bootloader
TARGET_BOOTLOADER_BOARD_NAME := MSM8992
TARGET_NO_BOOTLOADER := true

# Platform
TARGET_BOARD_PLATFORM := msm8992
TARGET_BOARD_PLATFORM_GPU := qcom-adreno418

# Architecture
TARGET_ARCH := arm64
TARGET_ARCH_VARIANT := armv8-a
TARGET_CPU_ABI := arm64-v8a
TARGET_CPU_ABI2 :=
TARGET_CPU_VARIANT := generic
TARGET_CPU_SMP := true

TARGET_2ND_ARCH := arm
TARGET_2ND_ARCH_VARIANT := armv7-a-neon
TARGET_2ND_CPU_ABI := armeabi-v7a
TARGET_2ND_CPU_ABI2 := armeabi
TARGET_2ND_CPU_VARIANT := generic

BOARD_KERNEL_CMDLINE := console=ttyHSL0,115200,n8 androidboot.console=ttyHSL0 user_debug=31 ehci-hcd.park=3 lpm_levels.sleep_disabled=1 androidboot.hardware=p1 msm_rtb.filter=0x37 androidboot.selinux=permissive

BOARD_KERNEL_BASE := 0x00000000
BOARD_KERNEL_PAGESIZE := 4096
BOARD_MKBOOTIMG_ARGS := --kernel_offset 0x0000000 --ramdisk_offset 0x02200000 --tags_offset 0x00000100 --dt device/lge/h815/dt.img

# prebuilt kernel
TARGET_PREBUILT_KERNEL := device/lge/h815/kernel
# else uncomment below to build from sauce
#TARGET_KERNEL_SOURCE := kernel/lge/h815
#TARGET_KERNEL_CONFIG := h815_defconfig
#TARGET_KERNEL_ARCH := arm64
#TARGET_KERNEL_HEADER_ARCH := arm64
#TARGET_KERNEL_SOURCE := kernel/lge/msm8992
#TARGET_KERNEL_CROSS_COMPILE_PREFIX := aarch64-linux-android-
#TARGET_KERNEL_CONFIG := msm8992_h815_defconfig
#TARGET_KERNEL_CONFIG := cyanogenmod_h815_defconfig

BOARD_BOOTIMAGE_PARTITION_SIZE := 41943040
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 41943040
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 1073741824
BOARD_USERDATAIMAGE_PARTITION_SIZE := 2147483648
BOARD_FLASH_BLOCK_SIZE := 131072 # (BOARD_KERNEL_PAGESIZE * 64)
BOARD_VOLD_EMMC_SHARES_DEV_MAJOR := true

TARGET_USERIMAGES_USE_EXT4 := true
BOARD_HAS_NO_SELECT_BUTTON := true
TARGET_RECOVERY_PIXEL_FORMAT := "RGBX_8888"

TARGET_PLATFORM_DEVICE_BASE := /devices/soc.0/

TW_THEME := portrait_hdpi
RECOVERY_GRAPHICS_USE_LINELENGTH := true
TW_NO_USB_STORAGE := true
TW_INCLUDE_CRYPTO := true
BOARD_SUPPRESS_SECURE_ERASE := true
RECOVERY_SDCARD_ON_DATA := true
BOARD_HAS_NO_REAL_SDCARD := true
TW_SCREEN_BLANK_ON_BOOT := true

# enables the use of f2fs filesystem on sdcard
#TARGET_USERIMAGES_USE_F2FS := true

#### enables hardware accelerated disk decryption
TARGET_HW_DISK_ENCRYPTION := true
# enable the following for aes-xts only!?
#CONFIG_HW_DISK_ENCRYPTION := true
# --> https://nelenkov.blogspot.de/2015/05/hardware-accelerated-disk-encryption-in.html
# --> https://www.bountysource.com/issues/23585711-twrp-2-8-7-0-oneplus-one-fails-to-decrypt-cm12-1-encryption (search for CONFIG_HW_DISK_ENCRYPTION)
