#include "hanoi.h"
#include <iostream>

int main() {
    std::cout << "Семинар 9. Полный перебор.\n";
    std::cout << "Задача: Ханойские башни (поиск решения для N дисков).\n\n";

    while (true) {
        std::cout << "Выберите режим:\n";
        std::cout << "  1 — Решить задачу для одного N (интерактивный режим)\n";
        std::cout << "  2 — Серия экспериментов (таблица, отчёт)\n";
        std::cout << "  0 — Выход\n";
        std::cout << "Ваш выбор: ";

        int mode = 0;
        if (!(std::cin >> mode)) {
            std::cout << "Некорректный ввод. Завершаем работу.\n";
            return 0;
        }

        if (mode == 0) {
            std::cout << "Выход.\n";
            break;
        } else if (mode == 1) {
            int n = 0;
            std::cout << "Введите число дисков N: ";
            if (!(std::cin >> n)) {
                std::cout << "Некорректный ввод N.\n";
                return 0;
            }
            solveSingleN(n);
        } else if (mode == 2) {
            int maxN = 0;
            std::cout << "Введите максимальное N для серии экспериментов: ";
            if (!(std::cin >> maxN)) {
                std::cout << "Некорректный ввод.\n";
                return 0;
            }
            runExperiments(maxN);
        } else {
            std::cout << "Неизвестный режим. Повторите ввод.\n\n";
        }

        std::cout << "\n";
    }

    return 0;
}
