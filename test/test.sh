#!/bin/sh

TEST_RES=../test-html


rm -rf ${TEST_RES}/mmcif_ddl.dic
rm -rf ${TEST_RES}/mmcif_std.dic

mkdir ${TEST_RES}/mmcif_ddl.dic
mkdir ${TEST_RES}/mmcif_ddl.dic/Icons
mkdir ${TEST_RES}/mmcif_ddl.dic/html-includes

cp -f ../icons/*.gif ${TEST_RES}/mmcif_ddl.dic/Icons
cp -f ../html-includes/*.txt ${TEST_RES}/mmcif_ddl.dic/html-includes

mkdir ${TEST_RES}/mmcif_std.dic 
mkdir ${TEST_RES}/mmcif_std.dic/Icons
mkdir ${TEST_RES}/mmcif_std.dic/html-includes

cp -f ../icons/*.gif ${TEST_RES}/mmcif_std.dic/Icons
cp -f ../html-includes/*.txt ${TEST_RES}/mmcif_std.dic/html-includes

echo "Beginning dict2HTML on mmcif_ddl.dic in test-html"
cd ${TEST_RES}; ../bin/dict2HTML -dictSdbFile ../../sdb/mmcif_ddl.sdb -htmlpath mmcif_ddl.dic > mmcif_ddl.log; cd -;

echo "Beginning dict2HTML on mmcif_std.dic in test-html"
cd ${TEST_RES}; ../bin/dict2HTML -dictSdbFile ../../sdb/mmcif_std.sdb -htmlpath mmcif_std.dic > mmcif_std.log;
rm -f ../test/mmcif_ddl.dic-parser.log ../test/mmcif_std.dic-parser.log;
cd -;

