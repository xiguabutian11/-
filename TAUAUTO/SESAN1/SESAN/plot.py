import numpy as np
import matplotlib.pyplot as plt
import os
import re  # 用于正则表达式解析

# 设置字体
plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False

# ====================== 关键：修复编码问题 + 解析static double参数 ======================
# 获取当前Python脚本(plot.py)的目录
current_script_dir = os.path.dirname(os.path.abspath(__file__))
# 头文件路径
cpp_header_path = os.path.join(current_script_dir, "../DATA/data_start.h")
cpp_header_path = os.path.normpath(cpp_header_path)

def read_file_with_auto_encoding(file_path):
    
    encodings = ['gbk', 'utf-8', 'gb2312', 'latin-1']
    for encoding in encodings:
        try:
            with open(file_path, 'r', encoding=encoding) as f:
                return f.read(), encoding
        except UnicodeDecodeError:
            continue
    raise ValueError(f"无法识别文件 {file_path} 的编码格式")

def get_cpp_static_double_value(header_file, var_name):
   
    try:
        # 1. 检查文件是否存在
        if not os.path.exists(header_file):
            raise FileNotFoundError(f"头文件不存在: {header_file}")
        
        # 2. 自动适配编码读取文件
        content, used_encoding = read_file_with_auto_encoding(header_file)
        print(f" 头文件编码：{used_encoding}")
        
        # 3. 正则匹配 static double 变量
        pattern = re.compile(
            r'static\s+double\s+' + re.escape(var_name) + r'\s*=\s*([\d\.]+)\s*;.*',
            re.IGNORECASE
        )
        match = pattern.search(content)
        
        if match:
            value = float(match.group(1))
            print(f"成功读取 {var_name} = {value}")
            return value
        else:
            raise ValueError(f"未找到 static double {var_name} 的定义")
    
    except Exception as e:
        print(f"读取 {var_name} 失败（使用默认值）: {e}")
        # 默认值匹配你头文件的数值
        return 30.0 if var_name == "Gain" else 500.0

# 读取参数（这次能真正读到头文件里的值了）
gain_threshold = get_cpp_static_double_value(cpp_header_path, "Gain")
power_threshold = get_cpp_static_double_value(cpp_header_path, "Pout")

# ============================================
# 第一部分：处理 output.txt
# ============================================
output_file = os.path.join(current_script_dir, "output.txt")
print(f"\n读取 output.txt: {output_file}")

try:
    data = np.loadtxt(output_file, skiprows=1)
    f = data[:, 0]
    vp = data[:, 1]
    kc = data[:, 3]
    print(f"成功读取 {len(f)} 行数据")
    
    plt.figure(figsize=(14, 10))
    
    plt.subplot(2, 2, 1)
    plt.plot(f, vp, 'b-', linewidth=2)
    plt.xlabel('f')
    plt.ylabel('vp')
    plt.title('vp vs f')
    plt.grid(True, alpha=0.3)
    
    plt.subplot(2, 2, 2)
    plt.plot(f, kc, 'r-', linewidth=2)
    plt.xlabel('f')
    plt.ylabel('kc')
    plt.title('kc vs f')
    plt.grid(True, alpha=0.3)
    
except Exception as e:
    print(f"读取 output.txt 失败: {e}")

# ============================================
# 第二部分：处理 gain_data.txt
# ============================================
gain_file = os.path.join(current_script_dir, "gain_data.txt")
print(f"\n读取 gain_data.txt: {gain_file}")

try:
    with open(gain_file, 'r', encoding='gbk') as f:  # 同样适配GBK编码
        lines = f.readlines()
    
    data_start = 0
    for i, line in enumerate(lines):
        if "工作频率(Hz)" in line:
            data_start = i + 1
            break
    
    freq = []
    gain = []
    output_power = []
    
    for i in range(data_start, len(lines)):
        line = lines[i].strip()
        if line:
            parts = line.split()
            if len(parts) >= 4:
                freq.append(float(parts[0]))
                gain.append(float(parts[1]))
                output_power.append(float(parts[3]))
    
    freq = np.array(freq)
    gain = np.array(gain)
    output_power = np.array(output_power)
    
    print(f"成功读取 {len(freq)} 个数据点")
    
    plt.subplot(2, 2, 3)
    plt.plot(freq, gain, 'g-', linewidth=2)
    plt.axhline(y=gain_threshold, color='red', linestyle='-', linewidth=2, 
                label=f'增益阈值: {gain_threshold} dB')
    plt.xlabel('Frequency (GHz)')
    plt.ylabel('Gain (dB)')
    plt.title('Frequency vs Gain')
    plt.grid(True, alpha=0.3)
    plt.legend()
    
    plt.subplot(2, 2, 4)
    plt.plot(freq, output_power, 'm-', linewidth=2)
    plt.axhline(y=power_threshold, color='red', linestyle='-', linewidth=2,
                label=f'功率阈值: {power_threshold} W')
    plt.xlabel('Frequency (GHz)')
    plt.ylabel('Output Power (W)')
    plt.title('Frequency vs Output Power')
    plt.grid(True, alpha=0.3)
    plt.legend()
    
except Exception as e:
    print(f"读取 gain_data.txt 失败: {e}")

plt.tight_layout()
output_image = os.path.join(current_script_dir, 'combined_plots.png')
plt.savefig(output_image, dpi=300, bbox_inches='tight')
print(f"\n 图片已保存: {output_image}")

plt.show()
print("\n完成！")