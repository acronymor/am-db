#!/usr/bin/env bash

bin=$(dirname "${BASH_SOURCE-$0}")
bin=$(
  cd "$bin" >/dev/null || exit
  pwd
)
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

function env() {
  local pkg=(make cmake flex bison)
  for item in "${pkg[@]}"; do
    cmd="sudo apt install ${item} -y"
    run_cmd "${cmd}"
  done

  local libs=(libgflags-dev libgtest-dev libprotobuf-dev libprotoc-dev protobuf-compiler libleveldb-dev libssl-dev)
  for item in "${libs[@]}"; do
    cmd="sudo apt install ${item} -y"
    run_cmd "${cmd}"
  done

  cmd="cd ${bin}/third-party/incubator-brpc"
  run_cmd "${cmd}"

  cmd="sh config_brpc.sh --headers=/usr/include --libs=/usr/lib"
  run_cmd "${cmd}"

  cmd="make"
  run_cmd "${cmd}"
}

function clone() {
  local cmd="cd ${bin}"
  run_cmd "${cmd}"

  cmd="git submodule update --init --recursive"
  run_cmd "${cmd}"

  cmd="git submodule update --remote --recursive"
  # run_cmd "${cmd}"
}

function init() {
  clone
  env
}

function build() {
  local build_dir=${bin}/build
  local parallel=8

  local cmd="cmake -S ${bin} -B ${build_dir} -DCMAKE_BUILD_TYPE=Release"
  run_cmd "${cmd}"

  cmd="cd ${build_dir}"
  run_cmd "${cmd}"

  cmd="make -j ${parallel}"
  run_cmd "${cmd}"
}

"$@"
