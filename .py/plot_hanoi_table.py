# .py/plot_hanoi_table.py
#
# Строит таблицу результатов экспериментов по задаче
# Ханойских башен и сохраняет её как PNG:
#   data/png/hanoi_table.png
#
# Требуется:
#   pip install matplotlib

import csv
import os
import matplotlib.pyplot as plt


def read_data(csv_path):
    """Читает CSV и возвращает заголовок и строки таблицы."""
    header = []
    rows = []

    with open(csv_path, "r", encoding="utf-8", newline="") as f:
        reader = csv.reader(f)
        header = next(reader, [])
        for row in reader:
            if not row:
                continue
            rows.append(row)

    return header, rows


def main():
    csv_path = os.path.join("data", "csv", "hanoi_results.csv")
    png_dir = os.path.join("data", "png")
    out_path = os.path.join(png_dir, "hanoi_table.png")

    if not os.path.exists(csv_path):
        print(f"Файл {csv_path} не найден. Сначала запусти программу C++ для генерации CSV.")
        return

    os.makedirs(png_dir, exist_ok=True)

    header, rows = read_data(csv_path)
    if not rows:
        print("CSV пустой, нечего рисовать.")
        return

    # ---------- Нормализация числа колонок ----------
    # Находим максимальное число столбцов среди всех строк и заголовка
    max_cols = max(len(header), max(len(r) for r in rows))

    def normalize(row):
        """Приводит строку к длине max_cols."""
        if len(row) > max_cols:
            return row[:max_cols]
        if len(row) < max_cols:
            return row + [""] * (max_cols - len(row))
        return row

    header = normalize(header)
    rows = [normalize(r) for r in rows]

    # Ограничим количество строк (на всякий случай)
    max_rows = 25
    rows = rows[:max_rows]

    display_header = header

    # ---------- Рисуем таблицу ----------
    n_rows = len(rows) + 1  # + заголовок
    n_cols = max_cols

    fig, ax = plt.subplots(figsize=(10, 0.4 * n_rows + 1.5))
    ax.axis("off")

    # Цвета: заголовок, чётные/нечётные строки
    header_color = "#e0f0ff"
    row_color_1 = "#ffffff"
    row_color_2 = "#f7fbff"

    cell_text = [display_header] + rows

    cell_colours = []
    for r in range(n_rows):
        if r == 0:
            cell_colours.append([header_color] * n_cols)
        else:
            color = row_color_1 if (r % 2 == 1) else row_color_2
            cell_colours.append([color] * n_cols)

    table = ax.table(
        cellText=cell_text,
        cellColours=cell_colours,
        loc="center",
        cellLoc="center",
    )

    table.auto_set_font_size(False)
    table.set_fontsize(8)

    table.auto_set_column_width(col=list(range(n_cols)))

    for (row_idx, col_idx), cell in table.get_celld().items():
        if row_idx == 0:
            cell.set_text_props(weight="bold")
        cell.set_edgecolor("#cccccc")

    ax.set_title(
        "Таблица результатов экспериментов для задачи Ханойских башен",
        fontsize=12,
        fontweight="bold",
        pad=20,
    )

    plt.tight_layout()
    plt.savefig(out_path, dpi=300)
    plt.close()

    print(f"Таблица сохранена в: {out_path}")


if __name__ == "__main__":
    main()
