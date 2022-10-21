#!/usr/bin/env bash

rm -f result_benchmark.txt

for cpp_file in $(ls -1 benchmark*.cpp | sort)
do
    if [ "${cpp_file}" = "benchmark_max_size.cpp" ]
    then
        continue
    fi
    exe_file=${cpp_file%.cpp}
    echo "run ${exe_file}"
    ./${exe_file} >> result_benchmark.txt
done
