#!/bin/csh -f
#
# File:  DictToHTML.csh
# Date:  15-August-2005 J. Westbrook
#
# Updated:  3-Aug-2008 to handle overlap of dictionaries common names
#
if ("$1" == "" ) then 
   echo "Usage:  $0 <Dictionary directory (e.g. mmcif_pdbx)>"
   exit 1
endif

set d = "$1"

set log = "$d.log"

cd html
set dic    = "$d.dic"
set dicSdb = "$d.sdb"
set dPath  = $dic

if (! -e ../sdb/$dicSdb ) then 
   echo "Missing dictionary SDB file sdb/$dicSdb"
   exit 1
endif

# 
rm -rf $dPath
mkdir $dPath
mkdir $dPath/Icons
cp -f ./icons/*.gif $dPath/Icons
mkdir $dPath/html-includes
cp -f ./html-includes/*.txt $dPath/html-includes
#

../bin/dict2HTML -dictSdbFile ../sdb/$dicSdb -htmlpath $dic >& $log

chmod -R +x $dPath

#
cd ..

##
##
