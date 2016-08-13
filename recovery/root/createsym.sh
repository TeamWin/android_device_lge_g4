#!/sbin/sh
rm -Rf /dev/block/bootdevice > /tmp/$0.log
ln -vs /dev/block/platform/f9824900.sdhci /dev/block/bootdevice > /tmp/$0.log
ls -la /dev/block/ |grep bootdevice >/tmp/$0.log
