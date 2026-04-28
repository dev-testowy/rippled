#pragma once

#include <gtest/gtest.h>
#include <helpers/TestContext.h>

#include <iostream>
#include <memory>
#include <source_location>
#include <string>
#include <string_view>
#include <utility>

namespace xrpl::test {

class GTestContext final : public TestContext
{
    class ScopedTraceCase final : public TestCaseScope
    {
        ::testing::ScopedTrace trace_;

    public:
        ScopedTraceCase(std::string_view name, std::source_location loc)
            : trace_(loc.file_name(), static_cast<int>(loc.line()), std::string{name})
        {
        }
    };

    std::string arg_;

public:
    explicit GTestContext(std::string arg = {}) : arg_(std::move(arg))
    {
    }

    std::string const&
    arg() const override
    {
        return arg_;
    }

private:
    bool
    doExpect(bool condition, std::string_view reason, std::source_location loc) override
    {
        if (condition)
            return true;

        doFail(reason, loc);
        return false;
    }

    void
    doFail(std::string_view reason, std::source_location loc) override
    {
        ADD_FAILURE_AT(loc.file_name(), static_cast<int>(loc.line())) << std::string{reason};
    }

    void
    doLog(std::string_view message) override
    {
        std::cout << message << std::endl;
    }

    std::unique_ptr<TestCaseScope>
    doTestcase(std::string_view name, std::source_location loc) override
    {
        return std::make_unique<ScopedTraceCase>(name, loc);
    }
};

}  // namespace xrpl::test
