#!/sbin/sh
# workaround script by steadfaster to ensure we have the required symlink on bootdevice

LOG=/tmp/recovery.log
F_LOG(){
   MSG="$1"
   echo "$TAG: $(date +%F_%T) - $MSG" >> $LOG
}

TAG="QSEECOMD"

F_LOG "Starting $0"
F_LOG "bootdevice check before actually doing anything:"
ls -la /dev/block/ |grep bootdevice >> $LOG
rm -Rf /dev/block/bootdevice >> $LOG
ln -vs /dev/block/platform/f9824900.sdhci /dev/block/bootdevice >> $LOG
LNKERR=$?
F_LOG "bootdevice check after fixing the symlink:"
ls -la /dev/block/ |grep bootdevice >> $LOG
F_LOG "$0 finished with <$LNKERR>"
