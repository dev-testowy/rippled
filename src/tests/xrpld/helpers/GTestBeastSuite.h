#pragma once

#include <xrpl/beast/unit_test/suite.h>

#include <gtest/gtest.h>

#include <string>

namespace xrpl::test {

class GTestBeastSuite : public ::testing::Test, public beast::unit_test::suite
{
    class Runner : public beast::unit_test::runner
    {
        void
        on_fail(std::string const& reason) override
        {
            ADD_FAILURE() << reason;
        }

        void
        on_log(std::string const& message) override
        {
            RecordProperty("beast_log", message);
        }
    };

    Runner runner_;

protected:
    GTestBeastSuite()
    {
        beast::unit_test::suite::operator()(runner_);
    }

private:
    void
    run() override
    {
        pass();
    }
};

}  // namespace xrpl::test
