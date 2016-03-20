#include "util/logger.hpp"

NDN_CXX_LOG_INIT(Module1);

namespace ndn {
namespace tests_integrated_log {

void
logFromModule1()
{
  NDN_CXX_LOG_TRACE("trace1");
  NDN_CXX_LOG_DEBUG("debug1");
  NDN_CXX_LOG_INFO("info1");
  NDN_CXX_LOG_WARN("warn1");
  NDN_CXX_LOG_ERROR("error1");
  NDN_CXX_LOG_FATAL("fatal1");
}

} // namespace tests_integrated_log
} // namespace ndn
