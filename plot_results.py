import pandas as pd
import matplotlib.pyplot as plt

# Load data
df = pd.read_csv('results.csv')
df = df[df['Trial'] != 'Average']

df['Trial'] = df['Trial'].astype(int)

# Plot
plt.figure(figsize=(12, 6))
plt.plot(df['Trial'], df['Volatile(ns)'], label='Volatile', marker='o')
plt.plot(df['Trial'], df['Optimized(ns)'], label='Optimized', marker='x')

plt.title("Volatile vs Optimized Instruction Timing")
plt.xlabel("Trial")
plt.ylabel("Time (ns)")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("timing_comparison_plot.png")
plt.show()
