#!/sbin/sh
# workaround script by steadfaster to ensure we have the required symlink on bootdevice
echo "Starting $0" > /tmp/$0.log
echo "bootdevice check before sleep:">>/tmp/$0.log
ls -la /dev/block/ |grep bootdevice >>/tmp/$0.log
echo "bootdevice check after sleep:">>/tmp/$0.log
ls -la /dev/block/ |grep bootdevice >>/tmp/$0.log
rm -Rf /dev/block/bootdevice >> /tmp/$0.log
ln -vs /dev/block/platform/f9824900.sdhci /dev/block/bootdevice >> /tmp/$0.log
ls -la /dev/block/ |grep bootdevice >>/tmp/$0.log
echo "$0 finished">>/tmp/$0.log
