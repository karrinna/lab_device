/**
 * @file ComplexColumn.hpp
 * @brief Класс Сложная колонна (2 входа, 2 выхода)
 */

#pragma once
#include "Device.hpp"
#include <stdexcept>

/**
 * @class ComplexColumn
 * @brief Представляет колонну с двумя входами и двумя выходами.
 *
 * Логика работы:
 * - Берёт суммарный поток с двух входов.
 * - Делит его между двумя выходами в зависимости от коэффициента splitRatio.
 */
class ComplexColumn : public Device {
private:
    double splitRatio; ///< Доля потока, уходящего в верхний выход

public:
    /**
     * @brief Конструктор задаёт параметры колонны.
     */
    ComplexColumn() {
        inputAmount = 2;
        outputAmount = 2;
        splitRatio = 0.5; // по умолчанию делим пополам
    }

    /**
     * @brief Устанавливает коэффициент распределения потока.
     * @param r Значение от 0 до 1
     * @throws std::invalid_argument если r <0 или r>1
     */
    void setSplitRatio(double r) {
        if (r < 0.0 || r > 1.0)
            throw std::invalid_argument("Неверный коэффициент распределения");
        splitRatio = r;
    }

    /**
     * @brief Возвращает текущий коэффициент распределения потока.
     */
    double getSplitRatio() const { return splitRatio; }

    /**
     * @brief Обновляет выходные потоки в соответствии с входными и splitRatio.
     * @throws std::logic_error если не подключены входы или выходы
     */
    void updateOutputs() override {
        if (inputs.size() != inputAmount || outputs.size() != outputAmount)
            throw std::logic_error("Неправильная конфигурация потоков");

        double totalInput = 0.0;
        for (auto& s : inputs) totalInput += s->getMassFlow();

        double topFlow = totalInput * splitRatio;
        double bottomFlow = totalInput * (1.0 - splitRatio);

        outputs[0]->setMassFlow(topFlow);
        outputs[1]->setMassFlow(bottomFlow);
    }
};
