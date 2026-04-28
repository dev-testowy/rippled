#pragma once

#include <xrpl/beast/utility/Journal.h>

#include <memory>
#include <mutex>
#include <source_location>
#include <string>
#include <string_view>

namespace xrpl::test {

/** Framework-neutral context for migrated unit test helpers.

    This is intentionally small: it covers the pieces the Beast-based helpers
    commonly need while letting new tests report through gtest.
*/
class TestContext
{
public:
    class TestCaseScope
    {
    public:
        virtual ~TestCaseScope() = default;
    };

    virtual ~TestContext() = default;

    bool
    expect(
        bool condition,
        std::string_view reason = {},
        std::source_location loc = std::source_location::current())
    {
        return doExpect(condition, reason, loc);
    }

    void
    fail(std::string_view reason = {}, std::source_location loc = std::source_location::current())
    {
        doFail(reason, loc);
    }

    void
    pass()
    {
        doPass();
    }

    void
    log(std::string_view message)
    {
        doLog(message);
    }

    std::unique_ptr<TestCaseScope>
    testcase(std::string_view name, std::source_location loc = std::source_location::current())
    {
        return doTestcase(name, loc);
    }

    virtual std::string const&
    arg() const
    {
        static std::string const empty;
        return empty;
    }

protected:
    virtual bool
    doExpect(bool condition, std::string_view reason, std::source_location loc) = 0;

    virtual void
    doFail(std::string_view reason, std::source_location loc) = 0;

    virtual void
    doPass()
    {
    }

    virtual void
    doLog(std::string_view message) = 0;

    virtual std::unique_ptr<TestCaseScope>
    doTestcase(std::string_view name, std::source_location loc) = 0;
};

class TestContextJournalSink : public beast::Journal::Sink
{
    std::string partition_;
    TestContext& context_;

public:
    TestContextJournalSink(
        std::string const& partition,
        beast::severities::Severity threshold,
        TestContext& context)
        : Sink(threshold, false), partition_(partition + " "), context_(context)
    {
    }

    bool
    active(beast::severities::Severity) const override
    {
        return true;
    }

    void
    write(beast::severities::Severity level, std::string const& text) override
    {
        if (level >= threshold())
            writeAlways(level, text);
    }

    void
    writeAlways(beast::severities::Severity level, std::string const& text) override
    {
        using namespace beast::severities;

        char const* const prefix = [level]() {
            switch (level)
            {
                case kTrace:
                    return "TRC:";
                case kDebug:
                    return "DBG:";
                case kInfo:
                    return "INF:";
                case kWarning:
                    return "WRN:";
                case kError:
                    return "ERR:";
                case kFatal:
                default:
                    break;
            }
            return "FTL:";
        }();

        static std::mutex logMutex;
        std::lock_guard const lock(logMutex);
        context_.log(std::string{prefix} + partition_ + text);
    }
};

class TestContextJournal
{
    TestContextJournalSink sink_;
    beast::Journal journal_;

public:
    TestContextJournal(
        std::string const& partition,
        TestContext& context,
        beast::severities::Severity threshold = beast::severities::kFatal)
        : sink_(partition, threshold, context), journal_(sink_)
    {
    }

    operator beast::Journal&()
    {
        return journal_;
    }
};

}  // namespace xrpl::test
