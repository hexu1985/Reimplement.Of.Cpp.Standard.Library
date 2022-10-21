#!/usr/bin/env bash

rm -f result_sample.txt

for cpp_file in $(ls -1 sample*.cpp | sort)
do
    if [ "${cpp_file}" = "sample_max_size.cpp" ]
    then
        continue
    fi
    exe_file=${cpp_file%.cpp}
    echo "run ${exe_file}"
    ./${exe_file} >> result_sample.txt
done
