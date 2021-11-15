#!/bin/bash
echo "input number:"
read n                #循环20次操作
for((i=1;i<=20;i++))  
do
time=$(date "+%Y-%m-%d-%H:%M:%S") #显示当前时间
echo $time
sleep $n  #休息n秒
done