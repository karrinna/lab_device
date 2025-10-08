CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -O2
TARGET = chemical_process
TEST_TARGET = run_tests

# Основная цель
all: $(TARGET)

# Сборка основной программы
$(TARGET): device.cpp
	$(CXX) $(CXXFLAGS) device.cpp -o $(TARGET)

# Сборка тестовой программы
test: $(TEST_TARGET)

$(TEST_TARGET): device.cpp
	$(CXX) $(CXXFLAGS) -DUNIT_TEST device.cpp -o $(TEST_TARGET)
	./$(TEST_TARGET)

# Запуск основной программы
run: $(TARGET)
	./$(TARGET)

# Очистка
clean:
	rm -f $(TARGET) $(TEST_TARGET)

.PHONY: all test run clean
