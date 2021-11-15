#!/bin/bash
str="I am oldboy teacher welcome to oldboy training class"
for i in ${str}
 do
 if [ ${#i} -le 6 ] #如果小于等于6则输出
 then
  echo ${i}
 fi
done