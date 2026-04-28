#pragma once

#include <xrpl/beast/unit_test.h>

#include <helpers/TestContext.h>

#include <memory>
#include <ostream>
#include <source_location>
#include <string>
#include <string_view>

namespace xrpl::test {

class BeastTestContext final : public TestContext
{
    beast::unit_test::suite& suite_;

public:
    explicit BeastTestContext(beast::unit_test::suite& suite) : suite_(suite)
    {
    }

    std::string const&
    arg() const override
    {
        return suite_.arg();
    }

private:
    bool
    doExpect(bool condition, std::string_view reason, std::source_location loc) override
    {
        return suite_.expect(
            condition, std::string{reason}, loc.file_name(), static_cast<int>(loc.line()));
    }

    void
    doFail(std::string_view reason, std::source_location loc) override
    {
        suite_.fail(std::string{reason}, loc.file_name(), static_cast<int>(loc.line()));
    }

    void
    doPass() override
    {
        suite_.pass();
    }

    void
    doLog(std::string_view message) override
    {
        suite_.log << message << std::endl;
    }

    std::unique_ptr<TestCaseScope>
    doTestcase(std::string_view name, std::source_location) override
    {
        suite_.testcase(std::string{name});
        return {};
    }
};

}  // namespace xrpl::test
