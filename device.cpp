

#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <stdexcept>
#include <locale.h>

using namespace std;

/// Допустимая погрешность для проверки значений потоков
const float POSSIBLE_ERROR = 0.01;

// Класс потока вещества
class Stream {
    double mass_flow; ///< Массовый расход
    string name;      ///< Имя потока
public:

    Stream(const string& n, double flow = 0.0) : name(n), mass_flow(flow) {}


    void setMassFlow(double m) { mass_flow = m; }


    double getMassFlow() const { return mass_flow; }


    string getName() const { return name; }

    void print() const { cout << "Поток " << name << " расход = " << mass_flow << endl; }
};


// Базовый класс устройства
class Device {
protected:
    vector<shared_ptr<Stream>> inputs;  ///< Входные потоки
    vector<shared_ptr<Stream>> outputs; ///< Выходные потоки
    int inputAmount = 0;  ///< Максимальное количество входов
    int outputAmount = 0; ///< Максимальное количество выходов
public:
    virtual ~Device() = default;

    void addInput(shared_ptr<Stream> s) {
        if (inputs.size() >= inputAmount) throw runtime_error("ПРЕВЫШЕН ЛИМИТ ВХОДНЫХ ПОТОКОВ!");
        inputs.push_back(s);
    }

    void addOutput(shared_ptr<Stream> s) {
        if (outputs.size() >= outputAmount) throw runtime_error("ПРЕВЫШЕН ЛИМИТ ВЫХОДНЫХ ПОТОКОВ!");
        outputs.push_back(s);
    }

    // Виртуальный метод обновления выходных потоков
    virtual void updateOutputs() = 0;
};


// Сложная колонна с 2 входами и 2 выходами
class ComplexColumn : public Device {
public:

    ComplexColumn() {
        inputAmount = 2;
        outputAmount = 2;
    }

    // Распределяет суммарный поток входов поровну на выходы
    void updateOutputs() override {
        if (inputs.size() != inputAmount || outputs.size() != outputAmount)
            throw runtime_error("ПОТОКИ НЕ ПОДКЛЮЧЕНЫ ПОЛНОСТЬЮ!");

        double total_flow = 0;
        for (auto& s : inputs) total_flow += s->getMassFlow();
        double flow_per_output = total_flow / outputAmount;
        for (auto& s : outputs) s->setMassFlow(flow_per_output);
    }
};

// Запуск тестов
void runTests() {
    int passedTests = 0;
    int totalTests = 0;

    // Тест 1: Проверка создания потока и методов set/get
    {
        totalTests++;
        cout << "\nТест 1: Создание потока и методы set/get" << endl;
        Stream s("test_stream", 5.5);
        s.setMassFlow(10.2);
        
        if (abs(s.getMassFlow() - 10.2) < POSSIBLE_ERROR && s.getName() == "test_stream") {
            cout << "Пройден" << endl;
            passedTests++;
        } else {
            cout << "Не пройден" << endl;
        }
    }

    // Тест 2: Проверка правильности распределения потоков
    {
        totalTests++;
        cout << "\nТест 2: Правильность распределения потоков" << endl;
        auto in1 = make_shared<Stream>("in1", 10.0);
        auto in2 = make_shared<Stream>("in2", 20.0);
        auto out1 = make_shared<Stream>("out1");
        auto out2 = make_shared<Stream>("out2");

        ComplexColumn col;
        col.addInput(in1);
        col.addInput(in2);
        col.addOutput(out1);
        col.addOutput(out2);
        col.updateOutputs();

        double expected = (10.0 + 20.0) / 2;
        if (abs(out1->getMassFlow() - expected) < POSSIBLE_ERROR && 
            abs(out2->getMassFlow() - expected) < POSSIBLE_ERROR) {
            cout << "Пройден" << endl;
            passedTests++;
        } else {
            cout << "Не пройден" << endl;
            cout << "Ожидалось: " << expected << endl;
            cout << "Получено out1: " << out1->getMassFlow() << endl;
            cout << "Получено out2: " << out2->getMassFlow() << endl;
        }
    }

    // Тест 3: Проверка обработки нулевых потоков
    {
        totalTests++;
        cout << "\nТест 3: Обработка нулевых потоков" << endl;
        auto in1 = make_shared<Stream>("in1", 0.0);
        auto in2 = make_shared<Stream>("in2", 0.0);
        auto out1 = make_shared<Stream>("out1", 5.0); // Начальное ненулевое значение
        auto out2 = make_shared<Stream>("out2", 5.0);

        ComplexColumn col;
        col.addInput(in1);
        col.addInput(in2);
        col.addOutput(out1);
        col.addOutput(out2);
        col.updateOutputs();

        if (abs(out1->getMassFlow() - 0.0) < POSSIBLE_ERROR && 
            abs(out2->getMassFlow() - 0.0) < POSSIBLE_ERROR) {
            cout << "Пройден" << endl;
            passedTests++;
        } else {
            cout << "Не пройден" << endl;
            cout << "Получено out1: " << out1->getMassFlow() << endl;
            cout << "Получено out2: " << out2->getMassFlow() << endl;
        }
    }

    // Тест 4: Проверка ограничения количества входов
    {
        totalTests++;
        cout << "\nТест 4: Проверка ограничения входов" << endl;
        ComplexColumn col;
        auto in1 = make_shared<Stream>("in1", 1.0);
        auto in2 = make_shared<Stream>("in2", 2.0);
        auto in3 = make_shared<Stream>("in3", 3.0);

        col.addInput(in1);
        col.addInput(in2);
        
        try {
            col.addInput(in3);
            cout << "Не пройден - Ожидалось исключение" << endl;
        } catch (const runtime_error& e) {
            if (string(e.what()) == "ПРЕВЫШЕН ЛИМИТ ВХОДНЫХ ПОТОКОВ!") {
                cout << "Пройден" << endl;
                passedTests++;
            } else {
                cout << "Не пройден - Неверное сообщение: " << e.what() << endl;
            }
        }
    }

    // Тест 5: Проверка ограничения количества выходов
    {
        totalTests++;
        cout << "\nТест 5: Проверка ограничения выходов" << endl;
        ComplexColumn col;
        auto out1 = make_shared<Stream>("out1");
        auto out2 = make_shared<Stream>("out2");
        auto out3 = make_shared<Stream>("out3");

        col.addOutput(out1);
        col.addOutput(out2);
        
        try {
            col.addOutput(out3);
            cout << "Не пройден - Ожидалось исключение" << endl;
        } catch (const runtime_error& e) {
            if (string(e.what()) == "ПРЕВЫШЕН ЛИМИТ ВЫХОДНЫХ ПОТОКОВ!") {
                cout << "Пройден" << endl;
                passedTests++;
            } else {
                cout << "Не пройден - Неверное сообщение: " << e.what() << endl;
            }
        }
    }

    // Тест 6: Проверка исключения при неполном подключении
    {
        totalTests++;
        cout << "\nТест 6: Проверка неполного подключения" << endl;
        ComplexColumn col;
        auto in1 = make_shared<Stream>("in1", 1.0);
        auto out1 = make_shared<Stream>("out1");

        col.addInput(in1);
        col.addOutput(out1);

        try {
            col.updateOutputs();
            cout << "Не пройден - Ожидалось исключение" << endl;
        } catch (const runtime_error& e) {
            if (string(e.what()) == "ПОТОКИ НЕ ПОДКЛЮЧЕНЫ ПОЛНОСТЬЮ!") {
                cout << "Пройден" << endl;
                passedTests++;
            } else {
                cout << "Не пройден - Неверное сообщение: " << e.what() << endl;
            }
        }
    }

    cout << "\n=== Результаты тестирования ===" << endl;
    cout << "Пройдено тестов: " << passedTests << " из " << totalTests << endl;
    cout << "Успешность: " << (passedTests * 100.0 / totalTests) << "%" << endl;
}

#ifndef UNIT_TEST
int main() {
    setlocale(LC_ALL, "Russian");
    
    // Демонстрация работы
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

    cout << "\nЗапуск тестов...\n";
    runTests();

    return 0;
}
#endif
