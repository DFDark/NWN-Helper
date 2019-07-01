#pragma once

#include <cstdio>

namespace Assert {

#if TAR_RELEASE
    #define ASSERT(condition) (void)0
    #define ASSERT_MSG(condition, format, ...) (void)0
    #define ASSERT_FAIL() (void)0
    #define ASSERT_FAIL_MSG(format, ...) (void)0
#else
    #define ASSERT(condition) \
        if (!(condition)) ::Assert::Fail((#condition), __FILE__, __LINE__, nullptr)
    #define ASSERT_MSG(condition, format, ...) \
        if (!(condition)) ::Assert::Fail((#condition), __FILE__, __LINE__, (format), ##__VA_ARGS__)
    #define ASSERT_FAIL() \
        ::Assert::Fail(nullptr, __FILE__, __LINE__, nullptr)
    #define ASSERT_FAIL_MSG(format, ...) \
        ::Assert::Fail(nullptr, __FILE__, __LINE__, (format), ##__VA_ARGS__)
#endif

void Fail(const char* condition, const char* file, int line, const char* message);

template <typename ... Args>
void Fail(const char* condition, const char* file, int line, const char* format, Args ... args);

#include "Utility/Assert.inl"

}
