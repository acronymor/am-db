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

################################################

cmake_build_type=Dev
amdb_build_test=OFF
amdb_build_parallel=8
amdb_build_dir=${bin}/build
amdb_release_dir=${bin}/release

################################################

function build() {
  local cmd="cmake -S ${bin} -B ${amdb_build_dir} -DCMAKE_BUILD_TYPE=${cmake_build_type} \
  -DAMDB_BUILD_TEST=${amdb_build_test} \
  -DLEVELDB_BUILD_BENCHMARKS=OFF \
  -DLEVELDB_BUILD_TESTS=OFF \
  -Dprotobuf_BUILD_TESTS=OFF"

  run_cmd "${cmd}"

  cmd="cd ${amdb_build_dir}"
  run_cmd "${cmd}"

  cmd="make -j ${amdb_build_parallel}"
  run_cmd "${cmd}"
}

function release() {
  cmake_build_type=Release
  amdb_build_test=OFF
  amdb_build_parallel=16

  local cmd="rm -rf ${amdb_build_dir}"
  run_cmd "${cmd}"

  build

  cmd="rm -rf ${amdb_release_dir}"
  run_cmd "${cmd}"

  cmd="mkdir -p ${amdb_release_dir}"
  run_cmd "${cmd}"

  cmd="cp ${amdb_build_dir}/amdb/sql/amdb ${amdb_release_dir}"
  run_cmd "${cmd}"
}

function test() {
  cmake_build_type=Test
  amdb_build_test=ON

  build

  cmd="make test"
  run_cmd "${cmd}"
}

"$@"
