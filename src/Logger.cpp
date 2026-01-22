#include <Lume/Logger.h>

#include <spdlog/sinks/stdout_color_sinks.h>

std::shared_ptr<spdlog::logger> Logger::mLogger = {};

void Logger::init()
{
    spdlog::set_pattern("%^[%T] %n: %v%$");
    mLogger = spdlog::stdout_color_st("Engine");
    mLogger->set_level(spdlog::level::trace);
}
