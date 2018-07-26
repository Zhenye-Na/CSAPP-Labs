#!/usr/bin bash

output_file=tsh.out

echo "" > tsh.out

echo -e "\n" >> tsh.out

for trace_file in ./*.txt
do
    echo -e "`./sdriver.pl -t $trace_file -s ./tsh -a "-p"`" >> tsh.out
done

echo -e "\n" >> tsh.out
