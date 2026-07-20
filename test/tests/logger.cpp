#include <sstream>
#include <fcfTest/test.hpp>
#include "helpers.hpp"

FCF_TEST_DECLARE("fcfTest", "Logger", "Log prefix", LogPrefixTester) {
  {
    fcf::NTest::Logger logger;
    std::stringstream ss;
    fcf::NTest::Logger::OutputTarget ot;
    ot.name = "default";
    ot.stream = &ss;
    logger.clearTargets();
    logger.appendTarget(ot);


    logger.log() << "123\n";
    FCF_TEST(ss.str() == "  > 123\n", ss.str());
    ss.str(std::string());

    logger.log() << "456\n";
    FCF_TEST(ss.str() == "  > 456\n", ss.str());
    ss.str(std::string());

    logger.log() << "456";
    logger.log() << " + 789\n";
    FCF_TEST(ss.str() == "  > 456 + 789\n", ss.str());
    ss.str(std::string());

    logger.log() << "456\n123\n";
    FCF_TEST(ss.str() == "  > 456\n  > 123\n", ss.str());
    ss.str(std::string());

    logger.log() << "456\n123";
    logger.log() << "a";
    logger.log() << "b";
    logger.log() << "c\n";

    FCF_TEST(ss.str() == "  > 456\n  > 123abc\n", ss.str());



    logger.clearPrefixes();
    ss.str(std::string());

    fcf::NTest::Logger::PrefixSettings ps;
    ps.name = "custom1";
    ps.multiLine = true;
    ps.messageCategories = fcf::NTest::LMC_ALL;
    logger.appendPrefixStr("[1]", ps);

    ps.name = "custom2";
    ps.multiLine = true;
    ps.messageCategories = fcf::NTest::LMC_ALL;
    logger.appendPrefixStr("[2]", ps);

    logger.log() << "456\n123\n";
    FCF_TEST(ss.str() == "[1][2]456\n[1][2]123\n", ss.str());
    ss.str(std::string());



    logger.clearPrefixes();
    ss.str(std::string());

    ps.name = "custom1";
    ps.multiLine = false;
    ps.messageCategories = fcf::NTest::LMC_ALL;
    logger.appendPrefixStr("[1]", ps);

    ps.name = "custom2";
    ps.multiLine = true;
    ps.messageCategories = fcf::NTest::LMC_ALL;
    logger.appendPrefixStr("[2]", ps);

    logger.log() << "456\n123\n";
    FCF_TEST(ss.str() == "[1][2]456\n[2]123\n", ss.str());
    ss.str(std::string());
  }
  
}
