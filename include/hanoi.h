#ifndef HANOI_H
#define HANOI_H

// Решение задачи для одного N с (возможно) выводом ходов
void solveSingleN(int n);

// Серия экспериментов для N = 1..maxN
// Анализирует количество шагов и время работы
void runExperiments(int maxN);

#endif // HANOI_H
