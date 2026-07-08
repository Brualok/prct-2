/*
test_app.c - модуль проверки программы. 

Маткин Илья Александрович
*/

#include "unity.h"

#include "lib_main.h"

void setUp(void) {
    // Вызывается перед каждым тестом (можно оставить пустым)
}

void tearDown(void) {
    // Вызывается после каждого теста (можно оставить пустым)
}

void TestLibAddFunction(void) {
    // Пример проверки: ожидали 3, получили результат функции
    TEST_ASSERT_EQUAL_INT(4, LibAddFunction(2, 2));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(TestLibAddFunction);
    return UNITY_END();
}

