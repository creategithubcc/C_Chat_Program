#!/bin/bash

echo "Input str:" 
read str
grep -rn "$str" *  #输出包含关键字的所有文件