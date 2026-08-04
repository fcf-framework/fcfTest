#include <sstream>
#include <fcfTest/test.hpp>
#include "helpers.hpp"

FCF_TEST_DEFINE("fcfTest", "Logger", "Log prefix", LogPrefixTester) {
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

    fcf::NTest::Logger::Prefix prefix;
    prefix.name = "custom1";
    prefix.prefix = "[1]";
    prefix.multiLine = true;
    prefix.category = fcf::NTest::LMC_ALL;
    logger.appendPrefix(prefix);

    prefix.name = "custom2";
    prefix.prefix = "[2]";
    prefix.multiLine = true;
    prefix.category = fcf::NTest::LMC_ALL;
    logger.appendPrefix(prefix);

    logger.log() << "456\n123\n";
    FCF_TEST(ss.str() == "[1][2]456\n[1][2]123\n", ss.str());
    ss.str(std::string());



    logger.clearPrefixes();
    ss.str(std::string());

    prefix.name = "custom1";
    prefix.prefix = "[1]";
    prefix.multiLine = false;
    prefix.category = fcf::NTest::LMC_ALL;
    logger.appendPrefix(prefix);

    prefix.name = "custom2";
    prefix.prefix = "[2]";
    prefix.multiLine = true;
    prefix.category = fcf::NTest::LMC_ALL;
    logger.appendPrefix(prefix);

    logger.log() << "456\n123\n";
    FCF_TEST(ss.str() == "[1][2]456\n[2]123\n", ss.str());
    ss.str(std::string());
  }

  // double prefix in one settings
  {
    std::stringstream ss;
    fcf::NTest::Logger logger;

    fcf::NTest::Logger::OutputTarget target;
    target.name = "default";
    target.stream = &ss;
    logger.targets({target});

    fcf::NTest::Logger::Prefix prefix;
    prefix.name = "my";
    prefix.prefix = "a1b";
    prefix.handler = [](fcf::NTest::Logger&, fcf::NTest::Logger::MessageContext&){
      return "c2d";
    };
    logger.prefixes({prefix});

    logger.log() << "TEST" << std::endl;

    FCF_TEST(ss.str() == "a1bc2dTEST\n", ss.str());
  }

}
