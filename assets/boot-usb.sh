#!/system/bin/sh

#====MVSX: Example of copying OUT the current binary====
#cp /mnt/data/hack/res/bin/fba029743 /mnt/hdisk/

#====MVSX: Example of copying over a hacked one in its place====
#cp /mnt/hdisk/fba029743 /mnt/data/hack/res/bin/fba029743

#====Linux Util Commands====
#list the devices and mountpoints
#cat /proc/mounts > /mnt/hdisk/mount-results-prerun.txt

#copy entire file structure to a file
#ls -R / > /mnt/hdisk/contents.txt

#get hard drive space available across all mounts
#df > /mnt/hdisk/size.txt

#====Launch AES====
mkdir /mnt/aes
/mnt/hdisk/AES > /mnt/hdisk/aes.log 2>&1