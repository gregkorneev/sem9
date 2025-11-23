# .py/plot_hanoi_dashboard.py
#
# Дашборд по времени работы алгоритма Ханойских башен.
# Читает data/csv/hanoi_results.csv и сохраняет картинку
# data/png/hanoi_dashboard.png

import csv
import os
import matplotlib.pyplot as plt


def read_data(csv_path):
    """Читает CSV и возвращает списки N и time_ms."""
    Ns = []
    times = []

    with open(csv_path, "r", encoding="utf-8", newline="") as f:
        reader = csv.reader(f)
        header = next(reader, None)  # пропускаем заголовок

        for row in reader:
            if len(row) < 4:
                continue
            try:
                n = int(row[0])
                t = float(row[3])
            except ValueError:
                continue

            Ns.append(n)
            times.append(t)

    return Ns, times


def main():
    csv_path = os.path.join("data", "csv", "hanoi_results.csv")
    png_dir = os.path.join("data", "png")
    out_path = os.path.join(png_dir, "hanoi_dashboard.png")

    if not os.path.exists(csv_path):
        print(f"Файл {csv_path} не найден. Сначала запусти программу C++ для генерации CSV.")
        return

    os.makedirs(png_dir, exist_ok=True)

    Ns, times = read_data(csv_path)
    if not Ns:
        print("Не удалось прочитать данные из CSV.")
        return

    # --- Фигура с двумя панелями ---
    fig, (ax_left, ax_right) = plt.subplots(1, 2, figsize=(12, 5))
    fig.suptitle("Время выполнения алгоритма Ханойских башен", fontsize=14, fontweight="bold")

    # ---------- Левая панель: столбчатый график ----------
    ax_left.bar(Ns, times)
    ax_left.set_title("Время полного перебора", fontsize=12, fontweight="bold")
    ax_left.set_xlabel("N (количество дисков)")
    ax_left.set_ylabel("Время, мс")
    ax_left.grid(axis="y", linestyle="--", alpha=0.5)

    # ---------- Правая панель: линия + “опасная зона” ----------
    ax_right.plot(Ns, times, marker="o")
    ax_right.set_title("Рост времени выполнения", fontsize=12, fontweight="bold")
    ax_right.set_xlabel("N (количество дисков)")
    ax_right.set_ylabel("Время, мс")
    ax_right.grid(True, linestyle="--", alpha=0.5)

    # Зона, где время уже "заметное" (выберем порог, например 1 мс)
    threshold = 1.0
    ax_right.fill_between(
        Ns,
        times,
        threshold,
        where=[t >= threshold for t in times],
        alpha=0.2,
        step="mid"
    )

    # Поджимаем поля
    plt.tight_layout(rect=[0, 0, 1, 0.95])
    plt.savefig(out_path, dpi=300)
    plt.close()

    print(f"Дашборд сохранён в: {out_path}")


if __name__ == "__main__":
    main()
