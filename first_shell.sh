#!/bin/bash



echo "hello world"
echo "任务完成！$(date)"

ping -c 10 10.10.1.1.1
if [ $? -eq 0 ]
then
	echo "通！"
else
	echo "不通！"
fi


for i in {1..50..3}
	do
		echo "hello!$i"
	done


for ((i=1;i<=5;i=i+2))
do
	echo $i
done

sum=0
ssum=0
for ((i=1;i<=100;i++))
do
	let sum=$i+$sum
	ssum=$(($i+$ssum))
done
echo $sum
echo $ssum

for i in {0..100}
do
	let sum=$i+$sum
done
echo $sum

for i in $(seq 2 $[$1-1])
do
	let j=$1%$i
	echo $j
	if [ $j -eq 0 ] 
	then
		echo "是质数"
	else
		echo "不是质数"
	fi
done

i=0
while (( $i < 5 )) 
do
	echo $[$i+1]
	let i+=1
done

let i=0
while [ $i -le 5 ]
do
	echo $(($i+1))
	let i++	
done


sed -n '1p' test
echo "打印结束"
sed -n '1,3p' test
echo "打印1-3行"
sed -n '1p;3p' test
echo "打印1，3行"

sed -n '2chello world' test

sed -n 's/a/ROOT/gp' test

echo "分隔符"

sed -n 's/a/ROOT/p' test

# ///不是必须 #@都可以 前后对应就可以


# awk 选项 '命令部分' 文件名
# sed 选项 '命令部分' 文件名
# awk -F: '命令部分' 文件名
# awk -F: '{print $1,$3}' 文件名       打印1,3列
# awk 'NR==1,NR==5{print $0}' 文件名   打印1-5行
# awk 'NR==1 || NR ==5{print $0}' 文件名 打印1或5行
# awk 是一门语言  $NF为列数
# $NF最后一列 NF列数 NR行号
# OFS可以定义输出分隔符 默认空格 例： awk -F: 'BEGIN{OFS="@"};{print $1,$NF}' 文件名
# 和{print $1"@"$NF}等效果
# RS 输入分隔符 默认\n
# 高级输出 awk '{print "月份： " $2 "\n年份： " $NF}'
# 高级输出 awk '{print "%15s %-10s %-15s\n", $1,$2,$3}' -代表左对齐 默认右对齐
# awk -F: '{print $1,$(NF-1),$NF,NF}' 1.txt    打印1列，倒数第二列，倒数第一列，列数
# BEGIN 在程序开始前执行
# END 在所有文件处理完后执行
# 引用shell变量用双引号
# 打印30-39行以bash结尾的内容：
# awk 'NR>=30 && NR<=39 && $0 ~ /bash$/{print $0}'
# awk 'NR>=30 && NR<=39 && $0 !~ /bash$/{print $0}'
# 打印30-39行不以bash结尾的内容：
# 多个分隔符： awk -F'[:#]' '{[RINT $5}' 正则表达式
# awk -F: '{ if($3>=500 && $3<=60000) {print $1,$3} } 文件名
# awk -F: '{if($(id -u)==0) {print $1,$3}}
# awk -F: '{if($3==0) {printf $1} else {print $2}}'
# awk -F: '{if($3==0) {printf $1} else if($2==0) {print $2} else {print $3}}'  
# 这一块挺像C的
# 值得注意的是 在awk中要输出变量的值不需要$
# awk 'BEGIN{sum=0;for(i=1;i<=10;i+=2) sum=sum+i;{print sum}}' 
# awk 'BEGIN{i=1;while(i<=5) {print i;i++}}'`
# awk 'BEGIN{for(y=1;y<=5;y++) {for(x=1;x<=y;x++) {printf x};print }}
