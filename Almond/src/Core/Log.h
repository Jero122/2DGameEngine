#pragma once
#include <memory>
#include <glm/detail/qualifier.hpp>
#include <glm/detail/setup.hpp>
#include "glm/gtx/string_cast.hpp"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

class Log
{
public:
	static void Init();
	static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return  s_EngineLogger; }
	static std::shared_ptr<spdlog::logger>& GetAppLogger() { return  s_AppLogger; }
private:
	static std::shared_ptr<spdlog::logger> s_EngineLogger;
	static std::shared_ptr<spdlog::logger> s_AppLogger;
};

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}

// Core log macros
#define AL_ENGINE_TRACE(...)    ::Log::GetEngineLogger()->trace(__VA_ARGS__)
#define AL_ENGINE_INFO(...)     ::Log::GetEngineLogger()->info(__VA_ARGS__)
#define AL_ENGINE_WARN(...)     ::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define AL_ENGINE_ERROR(...)    ::Log::GetEngineLogger()->error(__VA_ARGS__)
#define AL_ENGINE_CRITICAL(...) ::Log::GetEngineLogger()->critical(__VA_ARGS__)

// Client log macros
#define AL_TRACE(...)         ::Log::GetAppLogger()->trace(__VA_ARGS__)
#define AL_INFO(...)          ::Log::GetAppLogger()->info(__VA_ARGS__)
#define AL_WARN(...)          ::Log::GetAppLogger()->warn(__VA_ARGS__)
#define AL_ERROR(...)         ::Log::GetAppLogger()->error(__VA_ARGS__)
#define AL_CRITICAL(...)      ::Log::GetAppLogger()->critical(__VA_ARGS__)