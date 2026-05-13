# -*- coding: utf-8 -*-
import os
import glob
import matplotlib.pyplot as plt
plt.switch_backend('Agg')

# 强制把图片保存到 tiaobian_try 文件夹
folder = r"C:\Users\liu\Desktop\TAUAUTO\TAUAUTO\SESAN1\SESAN\tiaobian_try"

print("正在读取文件夹：", os.path.abspath(folder))

txt_list = glob.glob(os.path.join(folder, "sweep_data_*.txt"))

print(f"找到 {len(txt_list)} 个 txt 文件")

for txt_path in txt_list:
    filename = os.path.basename(txt_path)
    index = filename.replace("sweep_data_", "").replace(".txt", "")

    print("正在画图：", filename)

    freq, gain, power = [], [], []
    with open(txt_path, 'r') as f:
        for line in f:
            parts = line.strip().split()
            freq.append(float(parts[0]))
            gain.append(float(parts[1]))
            power.append(float(parts[2]))

    plt.figure(figsize=(8, 4))
    plt.plot(freq, gain, label='Gain')
    plt.plot(freq, power, label='Power')
    plt.grid(True)
    plt.legend()
    plt.title("Point " + index)

    # 强制保存路径
    save_path = os.path.join(folder, f"sweep_plot_{index}.png")
    print("图片保存到：", os.path.abspath(save_path))

    plt.savefig(save_path, dpi=150, bbox_inches='tight')
    plt.close()

print("\n✅ 全部画完！去上面打印的路径找图片！")