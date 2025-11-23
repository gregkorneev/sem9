#include "hanoi.h"

#include <iostream>
#include <chrono>
#include <fstream>      // для записи в файл CSV
#include <filesystem>   // для создания папки csv
#include <iomanip>      // для форматирования вывода

using long_long = long long;
namespace fs = std::filesystem;

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

// -------------------------------------------------------
// Серия экспериментов для N = 1..maxN.
// Результаты выводятся на экран и записываются в
// файл csv/hanoi_results.csv (разделитель — запятая).
// -------------------------------------------------------
void runExperiments(int maxN) {
    if (maxN <= 0) {
        std::cout << "maxN должно быть положительным.\n";
        return;
    }

    // 1. Создаём папку csv (если её ещё нет)
    try {
        fs::create_directories("csv");
    } catch (...) {
        std::cout << "Не удалось создать папку csv.\n";
        return;
    }

    // 2. Открываем CSV-файл
    std::ofstream csv("csv/hanoi_results.csv");
    if (!csv.is_open()) {
        std::cout << "Не удалось открыть файл csv/hanoi_results.csv для записи.\n";
        return;
    }

    // Заголовок CSV: разделитель — запятая
    csv << "N,moves_program,moves_theory,time_ms\n";

    std::cout << "\n=== Эксперименты: рост сложности для разных N ===\n";
    std::cout << "Результаты также сохранены в файле csv/hanoi_results.csv\n\n";
    std::cout << "N\tХодов (программа)\tТеория (2^N-1)\tВремя, мс\n";
    std::cout << "--------------------------------------------------------------\n";

    std::cout << std::fixed << std::setprecision(6);

    for (int n = 1; n <= maxN; ++n) {
        long_long moves = 0;

        auto start = std::chrono::steady_clock::now();
        // В экспериментах ходы не печатаем
        hanoiRecursive(n, 1, 3, 2, moves, false);
        auto end = std::chrono::steady_clock::now();

        double ms = std::chrono::duration<double, std::milli>(end - start).count();
        long_long theoreticalMoves = pow2(n) - 1;

        // Вывод на экран
        std::cout << n << "\t"
                  << moves << "\t\t\t"
                  << theoreticalMoves << "\t\t"
                  << ms << "\n";

        // Запись строки в CSV
        csv << n << ','
            << moves << ','
            << theoreticalMoves << ','
            << std::setprecision(10) << ms << '\n';
    }

    csv.close();

    std::cout << "\nВывод по экспериментам:\n";
    std::cout << "• Число ходов растёт примерно как 2^N.\n";
    std::cout << "• Время работы тоже растёт экспоненциально.\n";
    std::cout << "• Для больших N полный перебор (все ходы) становится практически невозможным.\n";

    std::cout << "\nСравнение алгоритмов:\n";
    std::cout << "1) В программе реализован классический рекурсивный алгоритм.\n";
    std::cout << "   Он строит оптимальное решение и перебирает все ходы.\n";
    std::cout << "2) Любой корректный алгоритм должен сделать минимум 2^N - 1 ходов,\n";
    std::cout << "   поэтому асимптотически более быстрый алгоритм (лучше, чем O(2^N)) невозможен.\n";
    std::cout << "3) \"Лучший\" алгоритм для Ханойских башен отличается только константой,\n";
    std::cout << "   то есть насколько быстро он считает и выводит ходы, но порядок 2^N остаётся.\n";
}
