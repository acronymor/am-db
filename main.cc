#include "main.h"

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <gtest/gtest.h>

int main(int argc, char* argv[]) {
  std::cout << "Hello world" << std::endl;
  google::InitGoogleLogging(argv[0]);
  LOG(ERROR) << "Hello,GLOG!";
  google::ShutdownGoogleLogging();
  return 0;
}