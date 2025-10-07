/**
 * @file Stream.hpp
 * @brief Класс Stream — описывает поток вещества с именем и массовым расходом.
 */

#pragma once
#include <iostream>
#include <string>

/**
 * @class Stream
 * @brief Класс, представляющий поток вещества.
 *
 * Каждый поток имеет:
 * - имя (например, "s1", "s2")
 * - массовый расход (в условных единицах)
 */
class Stream {
private:
    double mass_flow; ///< Массовый расход потока
    std::string name; ///< Имя потока

public:
    /**
     * @brief Конструктор создаёт поток с уникальным именем.
     * @param s Номер, добавляемый к имени (например, "s1").
     */
    Stream(int s) { setName("s" + std::to_string(s)); }

    /**
     * @brief Устанавливает имя потока.
     * @param s Новое имя.
     */
    void setName(std::string s) { name = s; }

    /**
     * @brief Возвращает имя потока.
     */
    std::string getName() const { return name; }

    /**
     * @brief Устанавливает массовый расход.
     * @param m Значение расхода.
     */
    void setMassFlow(double m) { mass_flow = m; }

    /**
     * @brief Возвращает массовый расход.
     */
    double getMassFlow() const { return mass_flow; }

    /**
     * @brief Выводит информацию о потоке в консоль.
     */
    void print() const {
        std::cout << "Поток " << name << " имеет расход = " << mass_flow << std::endl;
    }
};
