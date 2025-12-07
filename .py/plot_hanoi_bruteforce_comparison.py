#!/usr/bin/env python3
import os
import csv
import math
import matplotlib.pyplot as plt

CSV_PATH = "data/csv/hanoi_results.csv"
PNG_DIR = "data/png"


def ensure_dirs():
    os.makedirs(PNG_DIR, exist_ok=True)


def read_data(path):
    Ns = []
    moves_prog = []
    moves_theory = []
    times = []

    if not os.path.exists(path):
        print(f"Файл {path} не найден.")
        return Ns, moves_prog, moves_theory, times

    with open(path, "r", encoding="utf-8-sig") as f:
        reader = csv.reader(f)
        rows = list(reader)

    if len(rows) <= 1:
        return Ns, moves_prog, moves_theory, times

    data_rows = rows[1:]  # пропускаем заголовок

    for row in data_rows:
        if len(row) < 4:
            continue
        try:
            n = int(row[0])
            m_prog = int(row[1])
            m_theory = int(row[2])
            t = float(row[3])
        except ValueError:
            continue

        Ns.append(n)
        moves_prog.append(m_prog)
        moves_theory.append(m_theory)
        times.append(t)

    return Ns, moves_prog, moves_theory, times


def plot_moves_vs_theory(Ns, moves_prog, moves_theory):
    plt.figure()
    plt.plot(Ns, moves_prog, marker="o", label="Ходы (программа)")
    plt.plot(Ns, moves_theory, marker="s", label="Теория 2^N−1")
    plt.xlabel("N (число дисков)")
    plt.ylabel("Число ходов")
    plt.title("Сравнение полного перебора с теорией для Ханойских башен")
    plt.grid(True)
    plt.legend()

    out_path = os.path.join(PNG_DIR, "hanoi_moves_vs_theory.png")
    plt.savefig(out_path, bbox_inches="tight")
    plt.close()
    print(f"График {out_path} создан.")


def plot_moves_and_time(Ns, moves_prog, times):
    fig, ax1 = plt.subplots()

    ax1.plot(Ns, moves_prog, marker="o", label="Ходы (программа)")
    ax1.set_xlabel("N (число дисков)")
    ax1.set_ylabel("Число ходов")
    ax1.grid(True)

    ax2 = ax1.twinx()
    ax2.plot(Ns, times, marker="s", label="Время, мс")
    ax2.set_ylabel("Время, мс")

    # Общая легенда
    lines1, labels1 = ax1.get_legend_handles_labels()
    lines2, labels2 = ax2.get_legend_handles_labels()
    fig.legend(lines1 + lines2, labels1 + labels2, loc="upper left")

    plt.title("Полный перебор: число ходов и время работы")
    out_path = os.path.join(PNG_DIR, "hanoi_moves_and_time.png")
    plt.savefig(out_path, bbox_inches="tight")
    plt.close(fig)
    print(f"График {out_path} создан.")


def main():
    ensure_dirs()
    Ns, moves_prog, moves_theory, times = read_data(CSV_PATH)

    if not Ns:
        print("Нет данных для построения графиков (CSV пустой).")
        return

    plot_moves_vs_theory(Ns, moves_prog, moves_theory)
    plot_moves_and_time(Ns, moves_prog, times)


if __name__ == "__main__":
    main()
