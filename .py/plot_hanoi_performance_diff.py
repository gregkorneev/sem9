# .py/plot_hanoi_performance_diff.py
#
# Строит визуализацию "Разница в производительности" по данным
# из data/csv/hanoi_results.csv и сохраняет картинку:
#   data/png/hanoi_performance_diff.png
#
# Требуется:
#   pip install matplotlib

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
    out_path = os.path.join(png_dir, "hanoi_performance_diff.png")

    if not os.path.exists(csv_path):
        print(f"Файл {csv_path} не найден. Сначала запусти программу C++ для генерации CSV.")
        return

    os.makedirs(png_dir, exist_ok=True)

    Ns, times = read_data(csv_path)
    if not Ns:
        print("Не удалось прочитать данные из CSV.")
        return

    # Базовое время — при минимальном N (обычно N = 1)
    base_time = times[0]
    if base_time <= 0:
        print("Базовое время некорректно (<= 0).")
        return

    # Во сколько раз время больше, чем при минимальном N
    ratios = [t / base_time for t in times]

    x_labels = [str(n) for n in Ns]

    # Цвета столбцов в зависимости от ratio
    colors = []
    for r in ratios:
        if r < 20:
            colors.append("#2ecc71")   # зелёный
        elif r < 100:
            colors.append("#f39c12")   # оранжевый
        else:
            colors.append("#e74c3c")   # красный

    # --- Рисуем дашборд ---
    fig, ax = plt.subplots(figsize=(12, 4))

    # Фон «карточки»
    fig.patch.set_facecolor("#f5f3ff")
    ax.set_facecolor("#ffffff")

    # Заголовок панели
    fig.suptitle(
        "🚀 Разница в производительности",
        fontsize=16,
        fontweight="bold",
        x=0.03,
        ha="left",
        y=0.98,
    )

    # Подзаголовок
    ax.set_title(
        "Во сколько раз возрастает время выполнения\n"
        "по сравнению с минимальным значением N",
        fontsize=11,
        pad=20,
    )

    # Столбчатый график
    ax.bar(x_labels, ratios, color=colors)

    ax.set_xlabel("N (количество дисков)")
    ax.set_ylabel("Коэффициент увеличения времени")

    # Логарифмическая шкала по Y — так лучше видно рост на малых N
    ax.set_yscale("log")

    # Сетка по Y
    ax.grid(axis="y", linestyle="--", alpha=0.4, which="both")

    # Ось X: подписи с шагом 10 по N
    # (оставляем все столбцы, но подписываем не каждое N)
    min_n = Ns[0]
    max_n = Ns[-1]
    tick_step = 10
    ticks = list(range(min_n, max_n + 1, tick_step))
    ax.set_xticks([Ns.index(t) for t in ticks if t in Ns])
    ax.set_xticklabels([str(t) for t in ticks if t in Ns])

    plt.tight_layout(rect=[0.02, 0.02, 0.98, 0.9])
    plt.savefig(out_path, dpi=300)
    plt.close()

    print(f"График разницы в производительности сохранён в: {out_path}")


if __name__ == "__main__":
    main()
