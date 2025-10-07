/**
 * @file Device.hpp
 * @brief Базовый класс Device для всех типов аппаратов (смесители, колонны и т.д.)
 */

#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Stream.hpp"

/**
 * @class Device
 * @brief Абстрактный класс, представляющий технологический аппарат.
 *
 * Устройство имеет входные и выходные потоки.  
 * Класс определяет базовую логику для добавления потоков и обновления выходных данных.
 */
class Device {
protected:
    std::vector<std::shared_ptr<Stream>> inputs;   ///< Входные потоки
    std::vector<std::shared_ptr<Stream>> outputs;  ///< Выходные потоки
    int inputAmount;   ///< Максимальное количество входов
    int outputAmount;  ///< Максимальное количество выходов

public:
    virtual ~Device() = default;

    /**
     * @brief Добавляет входной поток.
     * @throws const char* если превышен лимит входов.
     */
    void addInput(std::shared_ptr<Stream> s) {
        if (inputs.size() < inputAmount)
            inputs.push_back(s);
        else
            throw "Превышено количество входных потоков!";
    }

    /**
     * @brief Добавляет выходной поток.
     * @throws const char* если превышен лимит выходов.
     */
    void addOutput(std::shared_ptr<Stream> s) {
        if (outputs.size() < outputAmount)
            outputs.push_back(s);
        else
            throw "Превышено количество выходных потоков!";
    }

    /**
     * @brief Чисто виртуальная функция — должна быть реализована в наследниках.
     */
    virtual void updateOutputs() = 0;
};
