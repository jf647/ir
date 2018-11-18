#ifndef INIT_LOGGER_H
#define INIT_LOGGER_H

#include "logger.hpp"

#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

void init_logger() {
  // logging::register_simple_formatter_factory<
  //    boost::log::trivial::severity_level, char>("Severity");
  // logging::add_file_log(
  //    keywords::target = "logs/", keywords::file_name = "%y%m%d_%3N.log",
  //    keywords::rotation_size = 10 * 1024 * 1024,
  //    keywords::scan_method = sinks::file::scan_matching
  //      keywords::format = "[%TimeStamp%][%Severity%]: %Message%"
  //);

  // logging::add_console_log(std::cout,
  //                         boost::log::keywords::format = ">> %Message%");
  logging::core::get()->set_filter(logging::trivial::severity >=
                                   logging::trivial::info);
}

#endif /* INIT_LOGGER_H */
