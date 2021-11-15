#!/bin/bash
cot=0 
ave=0

while read file #读取文件内容
do
let cot++ #总共多少数字
let ans+=$file
if [ $cot -eq 1 ];then #等于1就直接赋值最开始的数字
max=$file
min=$file
fi
[ "$min" -gt "$file" ]&&min=$file #大于的话就重新赋值
[ "$max" -lt "$file" ]&&max=$file #小于的话就重新赋值


done<file
let ave=$ans/$cot
echo "max=$max" 
echo "min=$min"
echo "ave=$ave"