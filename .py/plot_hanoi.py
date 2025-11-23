# .py/plot_hanoi.py
#
# Скрипт строит графики по результатам экспериментов
# из файла data/csv/hanoi_results.csv и сохраняет их в
# папку data/png как PNG-изображения.
#
# Требуется:
#   pip install matplotlib

import csv
import os
import matplotlib.pyplot as plt


def read_data(csv_path):
    """Читает CSV и возвращает списки N, moves, time_ms."""
    Ns = []
    moves = []
    times = []

    with open(csv_path, "r", encoding="utf-8", newline="") as f:
        reader = csv.reader(f)
        # первая строка — заголовок, пропускаем
        header = next(reader, None)

        for row in reader:
            if len(row) < 4:
                continue

            try:
                Ns.append(int(row[0]))          # N
                moves.append(int(row[1]))       # Ходов (программа)
                times.append(float(row[3]))     # Время, мс
            except ValueError:
                continue

    return Ns, moves, times


def plot_time(Ns, times, out_path):
    """Строит график времени работы от N и сохраняет в PNG."""
    plt.figure(figsize=(8, 5))
    plt.plot(Ns, times, marker="o")
    plt.xlabel("N (количество дисков)")
    plt.ylabel("Время, мс")
    plt.title("Время работы алгоритма Ханойских башен")
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(out_path, dpi=300)
    plt.close()


def plot_moves(Ns, moves, out_path):
    """Строит график количества ходов от N и сохраняет в PNG."""
    plt.figure(figsize=(8, 5))
    plt.plot(Ns, moves, marker="o")
    plt.xlabel("N (количество дисков)")
    plt.ylabel("Число ходов")
    plt.title("Количество ходов в зависимости от N")
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(out_path, dpi=300)
    plt.close()


def plot_combined(Ns, moves, times, out_path):
    """
    Строит комбинированный график:
    - число ходов (левая ось)
    - время выполнения (правая ось)
    """
    fig, ax1 = plt.subplots(figsize=(8, 5))

    # Левая ось — число ходов
    color1 = "tab:blue"
    ax1.set_xlabel("N (количество дисков)")
    ax1.set_ylabel("Число ходов", color=color1)
    ax1.plot(Ns, moves, marker="o", color=color1, label="Число ходов")
    ax1.tick_params(axis="y", labelcolor=color1)
    ax1.grid(True)

    # Правая ось — время в мс
    ax2 = ax1.twinx()
    color2 = "tab:orange"
    ax2.set_ylabel("Время, мс", color=color2)
    ax2.plot(Ns, times, marker="o", color=color2, label="Время, мс")
    ax2.tick_params(axis="y", labelcolor=color2)

    fig.suptitle("Рост числа ходов и времени выполнения\nдля задачи Ханойских башен")
    fig.tight_layout()
    plt.savefig(out_path, dpi=300)
    plt.close()


def main():
    # Пути
    csv_path = os.path.join("data", "csv", "hanoi_results.csv")
    png_dir = os.path.join("data", "png")

    if not os.path.exists(csv_path):
        print(f"Файл {csv_path} не найден. Сначала запусти программу C++ для генерации CSV.")
        return

    # Создаём папку для картинок
    os.makedirs(png_dir, exist_ok=True)

    # Читаем данные
    Ns, moves, times = read_data(csv_path)

    if not Ns:
        print("Не удалось прочитать данные из CSV (список N пуст).")
        return

    # Пути к картинкам
    time_png = os.path.join(png_dir, "hanoi_time.png")
    moves_png = os.path.join(png_dir, "hanoi_moves.png")
    combined_png = os.path.join(png_dir, "hanoi_combined.png")

    # Генерация визуализаций
    plot_time(Ns, times, time_png)
    plot_moves(Ns, moves, moves_png)
    plot_combined(Ns, moves, times, combined_png)

    print(f"График времени сохранён в: {time_png}")
    print(f"График количества ходов сохранён в: {moves_png}")
    print(f"Комбинированный график сохранён в: {combined_png}")


if __name__ == "__main__":
    main()
