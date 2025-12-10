#ifndef HANOI_H
#define HANOI_H

// Запуск одного эксперимента для N дисков.
// - moves  — количество выполненных ходов
// - ms     — время работы (в миллисекундах)
// Функция НЕ выводит ходы на экран, только считает.
void solveHanoi(int n, long long &moves, double &ms);

// Интерактивный режим: решение задачи для одного N.
// Печатает ходы (для небольших N) и итоговые показатели.
void solveSingleN(int n);

// Серия экспериментов: N = 1..maxN.
// Формирует CSV и Markdown-отчёт.
void runExperiments(int maxN);

#endif // HANOI_H
