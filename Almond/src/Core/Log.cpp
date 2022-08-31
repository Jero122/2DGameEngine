#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Log::s_EngineLogger;
std::shared_ptr<spdlog::logger> Log::s_AppLogger;

void Log::Init()
{
	spdlog::set_pattern("%^[%T] %n [%l]: %v%$");
	s_EngineLogger = spdlog::stdout_color_mt("ALMOND");
	s_EngineLogger->set_level(spdlog::level::trace);

	s_AppLogger = spdlog::stdout_color_mt("APP");
	s_AppLogger->set_level(spdlog::level::trace);

}
