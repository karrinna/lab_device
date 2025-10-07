/**
 * @file main.cpp
 * @brief Реализация "Сложной колонны" с двумя входами и двумя выходами.
 * 
 * В этом файле реализованы классы Stream, Device и ComplexColumn, 
 * а также простые тесты для проверки работы класса ComplexColumn.
 */

#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <stdexcept>

using namespace std;

/// Допустимая погрешность для проверки значений потоков
const float POSSIBLE_ERROR = 0.01;

/**
 * @class Stream
 * @brief Класс, представляющий поток вещества.
 * 
 * Поток характеризуется именем и массовым расходом.
 */
class Stream {
    double mass_flow; ///< Массовый расход потока
    string name;      ///< Имя потока
public:
    /**
     * @brief Конструктор потока
     * @param n Имя потока
     * @param flow Массовый расход (по умолчанию 0)
     */
    Stream(const string &n, double flow = 0.0) : name(n), mass_flow(flow) {}

    /**
     * @brief Установить массовый расход
     * @param m Новое значение массового расхода
     */
    void setMassFlow(double m) { mass_flow = m; }

    /**
     * @brief Получить массовый расход
     * @return Текущее значение массового расхода
     */
    double getMassFlow() const { return mass_flow; }

    /**
     * @brief Получить имя потока
     * @return Имя потока
     */
    string getName() const { return name; }

    /**
     * @brief Вывести информацию о потоке в консоль
     */
    void print() const { cout << "Stream " << name << " flow = " << mass_flow << endl; }
};

/**
 * @class Device
 * @brief Базовый класс устройства, обрабатывающего потоки.
 * 
 * Устройство имеет входные и выходные потоки и метод обновления выходов.
 */
class Device {
protected:
    vector<shared_ptr<Stream>> inputs;  ///< Входные потоки
    vector<shared_ptr<Stream>> outputs; ///< Выходные потоки
    int inputAmount = 0;  ///< Максимальное количество входов
    int outputAmount = 0; ///< Максимальное количество выходов
public:
    virtual ~Device() = default;

    /**
     * @brief Добавить входной поток
     * @param s Умный указатель на поток
     * @throw runtime_error если превышен лимит входов
     */
    void addInput(shared_ptr<Stream> s) {
        if (inputs.size() >= inputAmount) throw runtime_error("INPUT STREAM LIMIT!");
        inputs.push_back(s);
    }

    /**
     * @brief Добавить выходной поток
     * @param s Умный указатель на поток
     * @throw runtime_error если превышен лимит выходов
     */
    void addOutput(shared_ptr<Stream> s) {
        if (outputs.size() >= outputAmount) throw runtime_error("OUTPUT STREAM LIMIT!");
        outputs.push_back(s);
    }

    /**
     * @brief Виртуальный метод обновления выходных потоков
     * 
     * Реализуется в наследниках устройства.
     */
    virtual void updateOutputs() = 0;
};

/**
 * @class ComplexColumn
 * @brief Сложная колонна с 2 входами и 2 выходами.
 */
class ComplexColumn : public Device {
public:
    /**
     * @brief Конструктор колонны
     */
    ComplexColumn() {
        inputAmount = 2;
        outputAmount = 2;
    }

    /**
     * @brief Распределяет суммарный поток входов поровну на выходы
     * @throw runtime_error если потоки не полностью подключены
     */
    void updateOutputs() override {
        if (inputs.size() != inputAmount || outputs.size() != outputAmount)
            throw runtime_error("Streams not fully connected!");

        double total_flow = 0;
        for (auto &s : inputs) total_flow += s->getMassFlow();
        double flow_per_output = total_flow / outputAmount;
        for (auto &s : outputs) s->setMassFlow(flow_per_output);
    }
};

/**
 * @brief Простейшие тесты для проверки работы ComplexColumn
 */
void runTests() {
    // Тест 1: проверка распределения потоков
    auto s1 = make_shared<Stream>("s1", 10.0);
    auto s2 = make_shared<Stream>("s2", 6.0);
    auto s3 = make_shared<Stream>("s3");
    auto s4 = make_shared<Stream>("s4");
    ComplexColumn col;
    col.addInput(s1);
    col.addInput(s2);
    col.addOutput(s3);
    col.addOutput(s4);
    col.updateOutputs();
    if (abs(s3->getMassFlow() - 8.0) < POSSIBLE_ERROR &&
        abs(s4->getMassFlow() - 8.0) < POSSIBLE_ERROR)
        cout << "Test 1 passed\n";
    else cout << "Test 1 failed\n";

    // Тест 2: слишком много входов
    bool exceptionThrown = false;
    try {
        auto s5 = make_shared<Stream>("s5");
        col.addInput(s5);
    } catch (runtime_error &e) { exceptionThrown = true; }
    cout << "Test 2 " << (exceptionThrown ? "passed" : "failed") << endl;

    // Тест 3: слишком много выходов
    exceptionThrown = false;
    try {
        auto s6 = make_shared<Stream>("s6");
        col.addOutput(s6);
    } catch (runtime_error &e) { exceptionThrown = true; }
    cout << "Test 3 " << (exceptionThrown ? "passed" : "failed") << endl;

    // Тест 4: проверка суммы потоков
    double sum_inputs = s1->getMassFlow() + s2->getMassFlow();
    double sum_outputs = s3->getMassFlow() + s4->getMassFlow();
    if (abs(sum_inputs - sum_outputs) < POSSIBLE_ERROR)
        cout << "Test 4 passed\n";
    else
        cout << "Test 4 failed\n";
}

/**
 * @brief Главная функция программы
 */
int main() {
    auto s1 = make_shared<Stream>("s1", 10.0);
    auto s2 = make_shared<Stream>("s2", 5.0);
    auto s3 = make_shared<Stream>("s3");
    auto s4 = make_shared<Stream>("s4");

    ComplexColumn column;
    column.addInput(s1);
    column.addInput(s2);
    column.addOutput(s3);
    column.addOutput(s4);

    column.updateOutputs();

    s1->print();
    s2->print();
    s3->print();
    s4->print();

    cout << "\nRunning tests...\n";
    runTests();

    return 0;
}
