#/bin/bash

for x in ../test-data/graph/maxflow/*.in
do
	echo $x
	pre=${x%.in}
	ans=$pre.ans
        $* < $x | grep '^f = ' | sed 's/f = //' > out
	if diff $ans out
	then
		echo PASS $x 
		rm out
	else
		echo FAIL $x
		exit 1
	fi
done
