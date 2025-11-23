#include "hanoi.h"

#include <iostream>
#include <chrono>
#include <iomanip>

using long_long = long long;

// -------------------------------------------------------
// Вспомогательная функция: 2^n (степень двойки)
// Простая реализация через цикл.
// -------------------------------------------------------
long_long pow2(int n) {
    long_long result = 1;
    for (int i = 0; i < n; ++i) {
        result *= 2;
    }
    return result;
}

// -------------------------------------------------------
// Рекурсивный алгоритм Ханойских башен.
//
// n      — сколько дисков переносим
// from   — с какого стержня (1, 2 или 3)
// to     — на какой стержень
// aux    — вспомогательный стержень
// moves  — счётчик ходов
// print  — если true, печатаем каждый ход
// -------------------------------------------------------
void hanoiRecursive(int n, int from, int to, int aux,
                    long_long &moves, bool print) {
    if (n == 0) {
        // Базовый случай: нет дисков — нет ходов
        return;
    }

    // 1. Переносим n-1 дисков на вспомогательный стержень
    hanoiRecursive(n - 1, from, aux, to, moves, print);

    // 2. Переносим самый большой диск
    ++moves;
    if (print) {
        std::cout << "Ход " << moves << ": диск " << n
                  << " со стержня " << from
                  << " на стержень " << to << "\n";
    }

    // 3. Переносим n-1 дисков со вспомогательного на целевой
    hanoiRecursive(n - 1, aux, to, from, moves, print);
}

// -------------------------------------------------------
// Решение задачи для одного N.
// Для малых N (<= 5) выводим все ходы,
// для больших N только считаем количество шагов и время.
// -------------------------------------------------------
void solveSingleN(int n) {
    if (n <= 0) {
        std::cout << "N должно быть положительным.\n";
        return;
    }

    bool printMoves = (n <= 5); // ограничиваем вывод ходов

    std::cout << "\n=== Решение Ханойских башен для N = " << n << " ===\n";
    if (printMoves) {
        std::cout << "Выводим все ходы (полный перебор шагов):\n\n";
    } else {
        std::cout << "N слишком велико, ходы не печатаем (только считаем).\n\n";
    }

    long_long moves = 0;

    auto start = std::chrono::steady_clock::now();
    hanoiRecursive(n, 1, 3, 2, moves, printMoves);
    auto end = std::chrono::steady_clock::now();

    // Время в миллисекундах как double
    double ms = std::chrono::duration<double, std::milli>(end - start).count();

    long_long theoreticalMoves = pow2(n) - 1;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "\nИтого ходов (по программе): " << moves << "\n";
    std::cout << "Теоретически 2^N - 1 = " << theoreticalMoves << "\n";
    std::cout << "Разница: " << (moves - theoreticalMoves) << "\n";
    std::cout << "Время работы: " << ms << " мс\n";

    std::cout << "\nКомментарий по сложности:\n";
    std::cout << "• Алгоритм делает примерно 2^N - 1 ходов.\n";
    std::cout << "• Поэтому временная сложность: O(2^N).\n";
    std::cout << "• Это полный перебор всех необходимых ходов для решения задачи.\n";
}
