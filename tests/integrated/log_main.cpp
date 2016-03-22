#include "util/logger-factory.hpp"

#include <iostream>
#include <fstream>

namespace ndn {
namespace tests_integrated_log {

void
logFromModule1();

void
logFromModule2();

shared_ptr<std::ofstream> g_logDestination;

int
main(int argc, char** argv)
{
  // log with environ[NDN_CXX_LOG]
  std::cout << "-------- logging to std::clog with levels from NDN_CXX_LOG environ" << std::endl;
  sleep(1);
  ndn::util::LoggerFactory::setDestination(std::clog);
  logFromModule1();
  logFromModule2();

  if (argc > 1) {
    sleep(1);
    std::cout << "-------- changing LogLevels" << std::endl;
    sleep(1);

    // change log levels to argv[1]
    ndn::util::LoggerFactory::setSeverityLevels(argv[1]);
    logFromModule1();
    logFromModule2();
  }

  if (argc > 2) {
    sleep(1);
    std::cout << "-------- changing destination" << std::endl;
    sleep(1);

    // change destination to argv[2]
    g_logDestination = make_shared<std::ofstream>(argv[2]);
    ndn::util::LoggerFactory::setDestination(*g_logDestination);
    logFromModule1();
    logFromModule2();
  }

  sleep(1);
  std::cout << "-------- done" << std::endl;

  return 0;
}

} // namespace tests_integrated_log
} // namespace ndn

int
main(int argc, char** argv)
{
  return ndn::tests_integrated_log::main(argc, argv);
}
