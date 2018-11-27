#ifndef STACKTRACE_H
#define STACKTRACE_H
#include "logger.hpp"
#include <boost/filesystem.hpp>
#include <boost/stacktrace.hpp>
#include <signal.h> // ::signal, ::raise

void server_signal_handler(int signum) {
  ::signal(signum, SIG_DFL);
  boost::stacktrace::safe_dump_to(0, 10, "./backtrace.dump");
  ::raise(SIGABRT);
}

void stack_printer_cleaner() {
  if (boost::filesystem::exists("./backtrace.dump")) {
    // there is a backtrace
    std::ifstream ifs("./backtrace.dump");

    boost::stacktrace::stacktrace st =
        boost::stacktrace::stacktrace::from_dump(ifs);
    BOOST_LOG_TRIVIAL(error) << "Previous run crashed:\n" << st << std::endl;

    // cleaning up
    ifs.close();
    boost::filesystem::remove("./backtrace.dump");
  }
}
#endif /* STACKTRACE_H */
