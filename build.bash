#!/usr/bin/env bash

bin=$(dirname "${BASH_SOURCE-$0}")
bin=$(cd "$bin" > /dev/null || exit; pwd)
cd "$bin" || exit

declare -r log_path=/tmp/shell.log

function run_cmd() {
    local out_file=${log_path}
    local date_format="+%Y-%m-%d %H:%M:%S"

    echo -e "\033[32m [INFO] [$(date "$date_format")] | $* \033[0m" | tee -a "${out_file}"
    local cmd=$1
    shift

    $cmd "$@"
    local return_code=$?

    if [ ${return_code} -ne 0 ]; then
        echo -e "\033[31m [ERROR][$(date "$date_format")] |「ERRORCODE=${return_code}」| $cmd $* \033[0m" | tee -a "${out_file}"
        exit 1
    fi
}

function build() {
  local build_dir=${bin}/build
  local parallel=8

  local cmd="cmake -S ${bin} -B ${build_dir}"
  run_cmd "${cmd}"

  cmd="cd ${build_dir}"
  run_cmd "${cmd}"

  cmd="make -j ${parallel}"
  run_cmd "${cmd}"
}

"$@"