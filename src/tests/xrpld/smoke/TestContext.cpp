#include <helpers/TestContext.h>

#include <xrpld/consensus/ConsensusTypes.h>
#include <xrpld/overlay/detail/ProtocolVersion.h>
#include <xrpld/peerfinder/detail/Livecache.h>

#include <gtest/gtest.h>
#include <helpers/GTestContext.h>

#include <string>
#include <string_view>
#include <vector>

namespace xrpl::test {

namespace {

class CapturingContext final : public TestContext
{
public:
    std::vector<std::string> logs;
    std::vector<std::string> failures;
    std::string argValue;
    int passes = 0;

    std::string const&
    arg() const override
    {
        return argValue;
    }

private:
    bool
    doExpect(bool condition, std::string_view reason, std::source_location loc) override
    {
        if (condition)
        {
            ++passes;
            return true;
        }

        doFail(reason, loc);
        return false;
    }

    void
    doFail(std::string_view reason, std::source_location loc) override
    {
        failures.push_back(
            std::string{reason} + " @ " + loc.file_name() + ":" + std::to_string(loc.line()));
    }

    void
    doPass() override
    {
        ++passes;
    }

    void
    doLog(std::string_view message) override
    {
        logs.emplace_back(message);
    }

    std::unique_ptr<TestCaseScope>
    doTestcase(std::string_view, std::source_location) override
    {
        return {};
    }
};

}  // namespace

TEST(TestContext, CapturesExpectationsAndLogs)
{
    CapturingContext context;
    context.argValue = "argument";

    EXPECT_TRUE(context.expect(true));
    EXPECT_FALSE(context.expect(false, "expected failure"));
    context.pass();
    context.log("log message");

    EXPECT_EQ(context.arg(), "argument");
    EXPECT_EQ(context.passes, 2);
    ASSERT_EQ(context.failures.size(), 1);
    EXPECT_NE(context.failures.front().find("expected failure"), std::string::npos);
    ASSERT_EQ(context.logs.size(), 1);
    EXPECT_EQ(context.logs.front(), "log message");
}

TEST(TestContext, JournalWritesThroughContext)
{
    CapturingContext context;
    TestContextJournal testJournal("Partition", context, beast::severities::kDebug);
    beast::Journal& journal = testJournal;

    journal.debug() << "debug message";
    journal.trace() << "trace message";
    journal.error() << "error message";

    ASSERT_EQ(context.logs.size(), 2);
    EXPECT_EQ(context.logs[0], "DBG:Partition debug message");
    EXPECT_EQ(context.logs[1], "ERR:Partition error message");
}

TEST(TestContext, GTestContextSmoke)
{
    GTestContext context{"gtest-arg"};

    EXPECT_TRUE(context.expect(true, "true should pass"));
    EXPECT_EQ(context.arg(), "gtest-arg");

    auto scope = context.testcase("scoped trace smoke");
    ASSERT_NE(scope, nullptr);
}

TEST(XrpldGTestTarget, CompilesAgainstXrpldHeaders)
{
    ProtocolVersion const expectedProtocol{2, 0};

    EXPECT_EQ(make_protocol(2, 0), expectedProtocol);
    EXPECT_EQ(to_string(ConsensusMode::proposing), "proposing");

    [[maybe_unused]] PeerFinder::Livecache<>* livecache = nullptr;
    EXPECT_EQ(livecache, nullptr);
}

}  // namespace xrpl::test
