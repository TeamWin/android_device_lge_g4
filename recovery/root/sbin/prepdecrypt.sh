#!/sbin/sh

LOG=/tmp/recovery.log
TAG=PREPDEC

F_LOG(){
   MSG="$1"
   echo -e "I:$TAG: $(date +%F_%T) - $MSG" >> $LOG
}
F_ELOG(){
   MSG="$1"
   echo -e "E:$TAG: $(date +%F_%T) - $MSG" >> $LOG
}
F_LOG "Started $0"

relink()
{
	fname=$(basename "$1")
	target="/sbin/$fname"
	sed 's|/system/bin/linker64|///////sbin/linker64|' "$1" > "$target"
	chmod 755 $target
}

syspath="/dev/block/bootdevice/by-name/system"

mkdir /s >> $LOG 2>&1 
mount -t ext4 -o ro "$syspath" /s  >> $LOG 2>&1 || F_ELOG "mounting /s to $syspath failed"

# directories
F_LOG "$(echo "Preparing directories:"; \
mkdir /vendor 2>&1 ; \ 
mkdir -p /system/etc 2>&1 ; \
mkdir -p /vendor/lib64/hw/ 2>&1 ; \
mkdir /persist-lg 2>&1 ; \ 
mkdir /firmware 2>&1)"

# this relinks (linker64) AND copies qseecomd to /sbin
relink /s/vendor/bin/qseecomd  >> $LOG 2>&1 || F_ELOG "relinking qseecomd failed"

F_LOG "preparing libraries..."

# copy the hws stuff
cp /s/bin/hwservicemanager /sbin/ >> $LOG 2>&1 
cp /s/lib64/libandroid_runtime.so /sbin/ >> $LOG 2>&1 
cp /s/lib64/libhidltransport.so /sbin/ >> $LOG 2>&1 
cp /s/lib64/libhidlbase.so /sbin/ >> $LOG 2>&1 

# copy the decrypt stuff
cp /s/vendor/lib64/libdiag.so /sbin/ >> $LOG 2>&1 
cp /s/vendor/lib64/libdrmfs.so /sbin/ >> $LOG 2>&1 
cp /s/vendor/lib64/libdrmtime.so /sbin/ >> $LOG 2>&1 
cp /s/vendor/lib64/libQSEEComAPI.so /sbin/ >> $LOG 2>&1 
cp /s/vendor/lib64/librpmb.so /sbin/ >> $LOG 2>&1 
cp /s/vendor/lib64/libssd.so /sbin/ >> $LOG 2>&1 
cp /s/vendor/lib64/libtime_genoff.so /sbin/ >> $LOG 2>&1 
#cp /s/vendor/manifest.xml /vendor/ >> $LOG 2>&1 
#cp /s/vendor/compatibility_matrix.xml /vendor/ >> $LOG 2>&1 
cp /s/vendor/lib64/hw/keystore.msm8992.so /sbin/keystore.msm8992.so >> $LOG 2>&1 
cp /s/vendor/lib64/hw/gatekeeper.msm8992.so /sbin/gatekeeper.msm8992.so >> $LOG 2>&1 

cp /s/vendor/lib64/hw/gatekeeper.msm8992.so /vendor/lib64/hw/gatekeeper.msm8992.so >> $LOG 2>&1 
cp /s/vendor/lib64/hw/keystore.msm8992.so /vendor/lib64/hw/keystore.msm8992.so >> $LOG 2>&1 
cp /s/vendor/lib64/libmdtp.so /vendor/lib64/libmdtp.so >> $LOG 2>&1 
cp /s/vendor/lib64/libqmi_common_so.so /vendor/lib64/libqmi_common_so.so >> $LOG 2>&1 
cp /s/vendor/lib64/libsmemlog.so /vendor/lib64/libsmemlog.so >> $LOG 2>&1 
cp /s/vendor/lib64/libqmiservices.so /vendor/lib64/libqmiservices.so >> $LOG 2>&1 
cp /s/vendor/lib64/libqmi_encdec.so /vendor/lib64/libqmi_encdec.so >> $LOG 2>&1 
cp /s/vendor/lib64/libqmi_client_qmux.so /vendor/lib64/libqmi_client_qmux.so >> $LOG 2>&1 
cp /s/vendor/lib64/libqmi_cci.so /vendor/lib64/libqmi_cci.so >> $LOG 2>&1 
cp /s/vendor/lib64/libmdmdetect.so /vendor/lib64/libmdmdetect.so >> $LOG 2>&1 
cp /s/vendor/lib64/libidl.so /vendor/lib64/libidl.so >> $LOG 2>&1 
cp /s/vendor/lib64/libdsutils.so /vendor/lib64/libdsutils.so >> $LOG 2>&1 
cp /s/vendor/lib64/libdiag.so /vendor/lib64/libdiag.so >> $LOG 2>&1 
cp /s/vendor/lib64/libQSEEComAPI.so /vendor/lib64/libQSEEComAPI.so >> $LOG 2>&1 

F_LOG "preparing libraries finished"

umount /s >> $LOG 2>&1 || F_ELOG "unmounting /s failed"

# inform init to start qseecomd
setprop crypto.ready 1  >> $LOG 2>&1 
F_LOG "crypto.ready: $(getprop crypto.ready)"

F_LOG "current mounts: \n$(mount)"

F_LOG "$0 ended"
exit 0
