#pragma once

#include <memory>
#include <spdlog/spdlog.h>
#include <chrono>

class Logger
{
public:
    static void init();
    static std::shared_ptr<spdlog::logger>& get_logger() { return mLogger; }

    // --- PROFILER RAII ---
    class ScopedProfile
    {
    public:
        explicit ScopedProfile(const std::string_view name)
            : mName(name),
            mStart(std::chrono::high_resolution_clock::now())
        {
        }

        ~ScopedProfile()
        {
            const auto end = std::chrono::high_resolution_clock::now();
            const auto dur_us = std::chrono::duration_cast<std::chrono::microseconds>(end - mStart).count();
            Logger::get_logger()->debug("[PROFILE] {}: {:.3f} ms", mName, dur_us / 1000.0);
        }

    private:
        std::string_view mName = {};
        std::chrono::high_resolution_clock::time_point mStart = {};
    };

private:
    static std::shared_ptr<spdlog::logger> mLogger;
};

// --- MACRO PROFILER SEMPLICE & SICURA ---
#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y) CONCAT_IMPL(x, y)
#define PROFILE_SCOPE(name) Logger::ScopedProfile CONCAT(_profile_scope_, __LINE__)(name)

// Logger macros già definite (LOG_DEBUG ecc.)
#define LOG_TRACE(...)		Logger::get_logger()->trace(__VA_ARGS__)
#define LOG_INFO(...)		Logger::get_logger()->info(__VA_ARGS__)
#define LOG_WARN(...)		Logger::get_logger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)		Logger::get_logger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)	Logger::get_logger()->critical(__VA_ARGS__)
#define LOG_DEBUG(...)		Logger::get_logger()->debug(__VA_ARGS__)