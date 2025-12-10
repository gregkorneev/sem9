#include "hanoi.h"

#include <iostream>
#include <chrono>
#include <iomanip>

using long_long = long long;

// Внутренняя рекурсивная функция.
// from, to, aux — стержни (A, B, C).
// print         — печатать ли ход.
static void hanoiRecursiveInternal(int n,
                                   char from,
                                   char to,
                                   char aux,
                                   long_long &moves,
                                   bool print)
{
    if (n <= 0) return;
    // Переносим n-1 дисков на вспомогательный стержень
    hanoiRecursiveInternal(n - 1, from, aux, to, moves, print);

    // Переносим самый большой диск
    ++moves;
    if (print) {
        std::cout << "Переместить диск " << n
                  << " с " << from
                  << " на " << to << "\n";
    }

    // Переносим n-1 дисков со вспомогательного на конечный
    hanoiRecursiveInternal(n - 1, aux, to, from, moves, print);
}

// Обёртка для экспериментов: только считает и измеряет время, без вывода.
void solveHanoi(int n, long_long &moves, double &ms)
{
    moves = 0;
    ms = 0.0;

    if (n <= 0) {
        return;
    }

    auto start = std::chrono::steady_clock::now();
    hanoiRecursiveInternal(n, 'A', 'C', 'B', moves, false);
    auto end   = std::chrono::steady_clock::now();

    ms = std::chrono::duration<double, std::milli>(end - start).count();
}

// Интерактивный режим: решаем задачу для одного N.
void solveSingleN(int n)
{
    if (n <= 0) {
        std::cout << "N должно быть положительным.\n";
        return;
    }

    bool printMoves = (n <= 8); // чтобы не заспамить терминал при больших N

    long_long moves = 0;
    double ms = 0.0;

    std::cout << "\n=== Задача Ханойских башен для N = " << n << " ===\n\n";

    auto start = std::chrono::steady_clock::now();
    hanoiRecursiveInternal(n, 'A', 'C', 'B', moves, printMoves);
    auto end   = std::chrono::steady_clock::now();

    ms = std::chrono::duration<double, std::milli>(end - start).count();

    long_long theory = (1LL << n) - 1;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "\nИтоги:\n";
    std::cout << "  Количество ходов (перебор): " << moves   << "\n";
    std::cout << "  Теоретическое значение    : " << theory  << "\n";
    std::cout << "  Время выполнения          : " << ms      << " мс\n\n";

    std::cout << "Комментарий:\n";
    std::cout << "• Алгоритм выполняет полный перебор оптимального решения (2^N - 1 ходов).\n";
    std::cout << "• При увеличении N объём работы растёт экспоненциально.\n\n";
}
