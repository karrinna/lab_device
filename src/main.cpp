/**
 * @file main.cpp
 * @brief Пример использования класса ComplexColumn
 *
 * В этой программе создаются два входных и два выходных потока.
 * Создаётся колонна, задаётся коэффициент распределения потока,
 * и выводятся результаты работы на экран.
 */

#include "Stream.hpp"
#include "Device.hpp"
#include "ComplexColumn.hpp"

int main() {
    // Создаём входные и выходные потоки
    auto s1 = std::make_shared<Stream>(1);
    auto s2 = std::make_shared<Stream>(2);
    auto s3 = std::make_shared<Stream>(3);
    auto s4 = std::make_shared<Stream>(4);

    // Задаём массовый расход входных потоков
    s1->setMassFlow(10.0);
    s2->setMassFlow(20.0);

    // Создаём колонну и подключаем потоки
    ComplexColumn column;
    column.addInput(s1);
    column.addInput(s2);
    column.addOutput(s3);
    column.addOutput(s4);

    // Задаём распределение потока
    column.setSplitRatio(0.6);

    // Обновляем выходные потоки
    column.updateOutputs();

    // Выводим результат
    s3->print(); // Верхний выход
    s4->print(); // Нижний выход

    return 0;
}
