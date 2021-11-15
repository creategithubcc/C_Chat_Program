#!/bin/bash
mkdir a b         #创建文件夹a和b
chmod 777 /home/linuxcc/桌面/test/a  #赋予权限
chmod 777 /home/linuxcc/桌面/test/b
cd a  #进入文件夹a
echo "hello world!">helloworld.c  #创建文本
echo "你好世界!">nihaoshijie.c
echo "ohayo_sekai!">ohayosekai.c
echo "ohayo_sekai!">test01.txt
echo -n "这里是文件夹a下面的文件"
ls  #显示目录
dir="/home/linuxcc/桌面/test/b"
for i in *.c

do
mv $i $dir  #移动文件
done
echo -n "这里是文件夹a下面的文件,可以发现文件夹只剩下txt文件了。"
ls
echo -n "这里是文件夹b下面的文件,a的.c文件全部移动到了这里。"
ls $dir