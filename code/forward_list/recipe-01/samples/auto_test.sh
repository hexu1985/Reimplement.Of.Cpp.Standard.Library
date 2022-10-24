#!/usr/bin/env bash

cd `dirname ${BASH_SOURCE[0]}`
make -j4

make_ret=$?
if (( make_ret != 0 ))
then
    err_msg="error: make failed, check source file!"
    echo -e "\033[31m"${err_msg}"\033[0m"
    exit 1
fi

./run_benchmark.sh
./run_sample.sh

diff result_benchmark.txt result_sample.txt
diff_ret=$?
if (( diff_ret != 0 ))
then
    err_msg="error: result of benchmark and sample are different!"
    echo -e "\033[31m"${err_msg}"\033[0m"
    exit 1
fi

info_msg="info: auto test run pass!"
echo -e "\033[32m"${info_msg}"\033[0m"
