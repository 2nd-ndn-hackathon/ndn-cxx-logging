#include "util/logger.hpp"

NDN_CXX_LOG_INIT(Module2);

namespace ndn {
namespace tests_integrated_log {

void
logFromModule2()
{
  NDN_CXX_LOG_TRACE("trace2");
  NDN_CXX_LOG_DEBUG("debug2");
  NDN_CXX_LOG_INFO("info2");
  NDN_CXX_LOG_WARN("warn2");
  NDN_CXX_LOG_ERROR("error2");
  NDN_CXX_LOG_FATAL("fatal2");
}

} // namespace tests_integrated_log
} // namespace ndn
