#!/bin/sh

target=`basename $PWD`
cd ..
tar zcvf easygx.tar.gz $target
mv easygx.tar.gz $target
cd -