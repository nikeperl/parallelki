import os
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Цвета для графиков
colors = ["blue", "red", "green", "orange", "purple", "brown"]
color_map = {}  # Словарь для хранения цветов по sublabel

# Функция для автоматического назначения цвета
def get_color(label):
    if label not in color_map:
        color_map[label] = colors[len(color_map) % len(colors)]  # Цикличный выбор цветов
    return color_map[label]

# Получаем список CSV-файлов в директории
def get_csv_files(directory):
    return [os.path.join(directory, f) for f in os.listdir(directory) if f.endswith(".csv")]

# Читаем CSV в DataFrame
def read_csv(filename):
    return pd.read_csv(filename)

# Функция для построения графиков
def plot_data(directory):
    csv_dir = os.path.join(directory, "csv_files")
    filenames = get_csv_files(csv_dir)

    for filename in filenames:
        df = read_csv(filename)
        label = os.path.basename(filename)

        # Определяем потоки (S1, S2, ...) и соответствующие значения
        thread_columns = [col for col in df.columns if col.startswith("S")]
        threads = [int(col[1:]) for col in thread_columns]  # Числовые значения потоков
        s_values = df[thread_columns].astype(float)  # Значения ускорения
        names = df.iloc[:, 0]  # Первая колонка — имена тестов

        plt.figure(figsize=(8, 6))

        added_labels = set()  # Чтобы не дублировать легенду

        # Рисуем данные из CSV
        for i, sublabel in enumerate(names):
            color = get_color(sublabel)
            y_values = s_values.iloc[i].values  # Преобразуем строку DataFrame в массив

            if len(threads) == len(y_values):  # Проверяем, что размеры совпадают
                if sublabel not in added_labels:
                    plt.plot(threads, y_values, marker='o', linestyle='-', color=color, label=sublabel)
                    added_labels.add(sublabel)
                else:
                    plt.plot(threads, y_values, marker='o', linestyle='-', color=color)
            else:
                print(f"⚠️ Несоответствие размеров в файле {label}: {len(threads)} ≠ {len(y_values)}")

        # Масштабированная линейная функция
        max_y_value = s_values.to_numpy().max()
        ideal_line = np.array(threads) * (max_y_value / max(threads))
        plt.plot(threads, ideal_line, linestyle="--", color="black", label="Идеальное ускорение")

        plt.xlabel("Количество потоков")
        plt.ylabel("Коэффициент ускорения")
        plt.title(f"{label}")
        plt.legend()
        plt.grid()
        
        # Сохранение графика в папку "plots"
        output_path = os.path.join(directory, f"plots/{os.path.splitext(label)[0]}.png")
        plt.savefig(output_path, dpi=300, bbox_inches="tight")
        plt.close()  # Закрываем текущий график, чтобы не накапливать в памяти

        print(f"✅ График сохранён: {output_path}")

# Директория с CSV-файлами
csv_directory = "task3"  # Замените на свою папку
plot_data(csv_directory)
