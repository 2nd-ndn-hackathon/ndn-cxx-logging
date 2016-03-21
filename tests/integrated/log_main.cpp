#include "util/logger-factory.hpp"

#include <iostream>

namespace ndn {
namespace tests_integrated_log {

void
logFromModule1();

void
logFromModule2();

int
main(int argc, char** argv)
{
  // log with environ[NDN_CXX_LOG]
  std::cout << "--------" << std::endl;
  logFromModule1();
  logFromModule2();

  if (argc > 1) {
    std::cout << "--------" << std::endl;

    // change log levels to argv[1]
    ndn::util::LoggerFactory::setSeverityLevels(argv[1]);
    logFromModule1();
    logFromModule2();
  }

  std::cout << "--------" << std::endl;

  return 0;
}

} // namespace tests_integrated_log
} // namespace ndn

int
main(int argc, char** argv)
{
  return ndn::tests_integrated_log::main(argc, argv);
}
