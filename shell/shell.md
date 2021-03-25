# 调试shell
执行脚本时，增加 -x 参数
```
sh -x test.sh
```

# head
head -n 1
只打印第一行
```
grep -nr test_word | head -n 1
```

# for循环和if
```
for ((i=1; i<=$nr; i++))
do
    addr=`cat tmp.txt | sed -n "$i"p`
    symbol=`cat symbol.txt | sed -n "$i"p`
    if [ -z `grep $symbol $backtrace` ]; then
        sed -i "s/$addr/$addr  $symbol/g" 1.txt
    fi
done
```