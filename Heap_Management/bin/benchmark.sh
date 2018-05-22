#!/bin/bash

test-library() {
    library=$1
    echo "Testing $library ..."

    time $(env LD_PRELOAD=./lib/$library ./bin/test_00 | grep -o -E '[0-9]+' | sed -e 's/^\+//' >> tmp.txt) 
    
    time $(env LD_PRELOAD=./lib/$library ./bin/test_01 | grep -o -E '[0-9]+' | sed -e 's/^\+//' >> tmp.txt)

    time $(env LD_PRELOAD=./lib/$library ./bin/test_02 | grep -o -E '[0-9]+' | sed -e 's/^\+//' >> tmp.txt)

    time $(env LD_PRELOAD=./lib/$library ./bin/test_03 | grep -o -E '[0-9]+' | sed -e 's/^\+//' >> tmp.txt)

    echo "Results: $library..." >> ./benchmark_results.txt
	for index in {1..9}
	do    
		second=$(($index+9))
		third=$(($index+18))
		fourth=$(($index+27))
		#echo "TEST INDEX: $index, $second, $third, $fourth"
	    tmpVar1=$(awk 'NR=='+$index tmp.txt)
	    tmpVar2=$(awk 'NR=='+$second tmp.txt)
	   	tmpVar3=$(awk 'NR=='+$third tmp.txt)
	   	tmpVar4=$(awk 'NR=='+$fourth tmp.txt)
	    #echo "tmpVar = $tmpVar1, $tmpVar2, $tmpVar3, $tmpVar4"
	    tmpVar5=$(($tmpVar1+$tmpVar2+$tmpVar3+$tmpVar4))
	    echo "$index :: $tmpVar5" >> ./benchmark_results.txt
	done

	rm tmp.txt

}


rm ./benchmark_results.txt
echo "-------------------------------------------------------------------------------------------------------------------------" >> ./benchmark_results.txt
echo "1 - Mallocs | 2 - Frees | 3 - Reuses | 4 - Grows | 5 - Splits | 6 - Coalesces | 7 - Blocks | 8 - Requested | 9 - Max Heap" >> ./benchmark_results.txt
echo "-------------------------------------------------------------------------------------------------------------------------" >> ./benchmark_results.txt
test-library libmalloc-ff.so
test-library libmalloc-nf.so
test-library libmalloc-bf.so
test-library libmalloc-wf.so
