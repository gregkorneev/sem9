#include "hanoi.h"

#include <iostream>
#include <chrono>
#include <fstream>      // для записи в файлы CSV и MD
#include <filesystem>   // для создания папок
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
//
// Структура файлов:
//   data/
//     hanoi_report.md
//     csv/
//       hanoi_results.csv
//
// В терминал выводится таблица и пути к файлам.
// -------------------------------------------------------
void runExperiments(int maxN) {
    if (maxN <= 0) {
        std::cout << "maxN должно быть положительным.\n";
        return;
    }

    // 1. Создаём папку data и внутри неё data/csv
    try {
        fs::create_directories("data/csv");
    } catch (...) {
        std::cout << "Не удалось создать папки data/csv.\n";
        return;
    }

    // Пути к файлам
    const std::string csvPath = "data/csv/hanoi_results.csv";
    const std::string mdPath  = "data/hanoi_report.md";

    // 2. Открываем CSV-файл
    std::ofstream csv(csvPath);
    if (!csv.is_open()) {
        std::cout << "Не удалось открыть файл " << csvPath << " для записи.\n";
        return;
    }

    // Заголовок CSV: разделитель — запятая
    csv << "N,moves_program,moves_theory,time_ms\n";

    std::cout << "\n=== Эксперименты: рост сложности для разных N ===\n";
    std::cout << "Результаты таблицы будут сохранены в файле " << csvPath << "\n\n";
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

    // 3. Формируем Markdown-отчёт (без LaTeX, просто текст)
    std::ofstream md(mdPath);
    if (!md.is_open()) {
        std::cout << "\nНе удалось создать файл " << mdPath << "\n";
        return;
    }

    md << "# Анализ экспериментов по задаче Ханойских башен\n\n";
    md << "Исходные данные измерений находятся в файле "
          "`data/csv/hanoi_results.csv`.\n\n";

    md << "## Вывод по экспериментам\n\n";
    md << "- Число ходов растёт примерно как 2^N.\n";
    md << "- Время работы алгоритма также растёт экспоненциально.\n";
    md << "- Для больших значений N полный перебор (все ходы) "
          "становится практически невозможным с практической точки зрения.\n\n";

    md << "## Сравнение алгоритмов\n\n";
    md << "1. В программе реализован классический рекурсивный алгоритм "
          "Ханойских башен, который строит оптимальное решение и "
          "перебирает все необходимые ходы.\n";
    md << "2. Любой корректный алгоритм решения задачи Ханойских башен "
          "должен выполнить минимум 2^N - 1 перемещений, поэтому "
          "асимптотически более быстрый алгоритм (лучше, чем O(2^N)) "
          "невозможен.\n";
    md << "3. \"Лучший\" алгоритм для Ханойских башен может отличаться только "
          "константным множителем (насколько быстро считаются и выводятся ходы), "
          "но порядок сложности 2^N остаётся неизменным.\n";

    md.close();

    std::cout << "\nТаблица сохранена в: " << csvPath << "\n";
    std::cout << "Текстовый отчёт сохранён в: " << mdPath << "\n";
}
