#!/bin/sh
#set -x 

usage()
{
	echo "usage: `basename $0` make|install|reconf|only_reconf|distclean|test"
	exit 1
}

install_path="install"

export PKG_CONFIG_PATH=$PWD/$install_path/lib/pkgconfig
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD/$install_path/bin
export PATH=$PATH:$PWD/$install_path/bin

active="make"
if [ $# -ne 1 ];
then
	usage
fi
active=$1
log_file=$PWD/autogen.log
echo "Active:$active" > $log_file

echo "Active:$active  Log File:$log_file"

for module in sfpr sfel easygx
do
	cd $module
	echo "Starting $active in $module" | tee -a $log_file 
	case $active in
	make)
		chmod +x configure
		./configure --prefix=$PWD/../$install_path | tee -a $log_file
		make | tee -a $log_file
		make install | tee -a $log_file
		;;
	install)
		make install >> $log_file 2>&1
		;;
	reconf)
 	  autoreconf -i -f -v
		chmod +x configure
		./configure --prefix=$PWD/../$install_path >> $log_file 2>&1
		make >> $log_file 2>&1
		make install >> $log_file 2>&1
		;;
	only_reconf)
 	  autoreconf -i -f -v
 	  ;;
	distclean)
 	  make distclean
 	  ;;
	test)
 	  ;;
	*)usage
		;;
	esac
	cd -
done
if [ $active = "make" ];
then
	cd $PWD/$install_path/bin
	./easygx
	cd -
fi