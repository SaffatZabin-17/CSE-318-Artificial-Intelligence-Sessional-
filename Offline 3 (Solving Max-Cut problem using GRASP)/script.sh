#!/bin/bash 

rm "Assignment_output.csv"

touch "Assignment_output.csv"

echo "Problem, , ,Constructive Algorithm, , ,Local Search, ,GRASP, ,Known Best Solution or Upper Bound" >> "Assignment_output.csv"

echo "Name,|V| or n,|E| or m,Simple Randomized or Randomized-1,Simple Greedy or Greedy-1,Semi-Greedy-1,Simple Local or Local-1, ,GRASP-1, " >> "Assignment_output.csv"

echo " , , , , , ,No. of Iterations,Best Value,No. of Iterations,Best Value" >> "Assignment_output.csv"

g++ 1905060.cpp

for file in test_cases/*.rud; do
    ./a.out "$file"
done
