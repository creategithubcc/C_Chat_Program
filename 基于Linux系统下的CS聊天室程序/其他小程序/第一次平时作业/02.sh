#!/bin/bash
i=1
ans=1
for((i=1;i<=20;i++))
do
ans=`expr $ans \* $i` #不断相乘
done

echo $ans