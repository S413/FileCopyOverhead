log=time_log.txt
desd=tmpfs_mount/filecopy.txt
soud=file.txt

precondition:
	sudo mount -t tmpfs -o size=3G tmpfs tmpfs_mount/
	sudo swapoff -a && sudo swapon -a
	sudo sh -c "/usr/bin/echo 3 > /proc/sys/vm/drop_caches"

cleanup:
	diff -s $(soud) $(desd)

unmount:
	sudo umount tmpfs_mount

dropC:
	sudo swapoff -a && sudo swapon -a
	sudo sh -c "/usr/bin/echo 3 > /proc/sys/vm/drop_caches"

moveFile:
	cp file.txt tmpfs_mount/file.txt
