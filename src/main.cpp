#include <iostream>
#include "hanoi.h"

int main() {
    std::cout << "Семинар 9. Перебор. Задача: Ханойские башни.\n";

    int n;
    std::cout << "Введите количество дисков N: ";
    std::cin >> n;

    // Решаем задачу для одного N
    solveSingleN(n);

    std::cout << "\nТеперь проведём эксперименты для разных N.\n";
    std::cout << "Введите максимальное N для экспериментов (например, 15 или 20): ";
    int maxN;
    std::cin >> maxN;

    runExperiments(maxN);

    std::cout << "\nРабота программы завершена.\n";
    return 0;
}
