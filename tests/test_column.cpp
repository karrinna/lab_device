#include <gtest/gtest.h>
#include <memory>
#include "../main.cpp" // Подключаем твой код с ComplexColumn, Stream и Device

// 1. Юнит-тест для Stream::setMassFlow и Stream::getMassFlow
TEST(StreamTest, SetGetMassFlow) {
    auto s = std::make_shared<Stream>("s1");
    s->setMassFlow(12.5);
    EXPECT_NEAR(s->getMassFlow(), 12.5, 1e-6);
}

// 2. Юнит-тест для Stream::getName
TEST(StreamTest, GetName) {
    auto s = std::make_shared<Stream>("stream_test");
    EXPECT_EQ(s->getName(), "stream_test");
}

// 3. Тест распределения потоков в колонне
TEST(ComplexColumnTest, OutputFlowsCorrect) {
    auto s1 = std::make_shared<Stream>("s1", 10.0);
    auto s2 = std::make_shared<Stream>("s2", 6.0);
    auto s3 = std::make_shared<Stream>("s3");
    auto s4 = std::make_shared<Stream>("s4");

    ComplexColumn col;
    col.addInput(s1);
    col.addInput(s2);
    col.addOutput(s3);
    col.addOutput(s4);
    col.updateOutputs();

    EXPECT_NEAR(s3->getMassFlow(), 8.0, 1e-6);
    EXPECT_NEAR(s4->getMassFlow(), 8.0, 1e-6);
}

// 4. Проверка лимита входов
TEST(ComplexColumnTest, TooManyInputs) {
    auto s1 = std::make_shared<Stream>("s1");
    auto s2 = std::make_shared<Stream>("s2");
    auto s3 = std::make_shared<Stream>("s3");

    ComplexColumn col;
    col.addInput(s1);
    col.addInput(s2);

    EXPECT_THROW(col.addInput(s3), std::runtime_error);
}

// 5. Проверка лимита выходов
TEST(ComplexColumnTest, TooManyOutputs) {
    auto s1 = std::make_shared<Stream>("s1");
    auto s2 = std::make_shared<Stream>("s2");
    auto s3 = std::make_shared<Stream>("s3");
    auto s4 = std::make_shared<Stream>("s4");
    auto s5 = std::make_shared<Stream>("s5");

    ComplexColumn col;
    col.addInput(s1);
    col.addInput(s2);
    col.addOutput(s3);
    col.addOutput(s4);

    EXPECT_THROW(col.addOutput(s5), std::runtime_error);
}

// 6. Проверка суммы потоков вход = выход
TEST(ComplexColumnTest, InputSumEqualsOutputSum) {
    auto s1 = std::make_shared<Stream>("s1", 4.0);
    auto s2 = std::make_shared<Stream>("s2", 6.0);
    auto s3 = std::make_shared<Stream>("s3");
    auto s4 = std::make_shared<Stream>("s4");

    ComplexColumn col;
    col.addInput(s1);
    col.addInput(s2);
    col.addOutput(s3);
    col.addOutput(s4);
    col.updateOutputs();

    double total_input = s1->getMassFlow() + s2->getMassFlow();
    double total_output = s3->getMassFlow() + s4->getMassFlow();
    EXPECT_NEAR(total_input, total_output, 1e-6);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
