#include "hanoi.h"

#include <iostream>
#include <chrono>
#include <fstream>      // для записи в файлы CSV и MD
#include <filesystem>   // для создания папок
#include <iomanip>      // для форматирования вывода

using long_long = long long;
namespace fs = std::filesystem;

// Объявляем функции, реализованные в hanoi.cpp
long_long pow2(int n);
void hanoiRecursive(int n, int from, int to, int aux,
                    long_long &moves, bool print);

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

    // Русифицированный заголовок CSV
    csv << "N,Ходов (программа),Теория (2^N-1),Время, мс\n";

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

        // Запись строки в CSV (значения такие же, как в терминале)
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
