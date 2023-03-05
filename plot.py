import seaborn as sns
import pandas as pd
import matplotlib.pyplot as plt

try:
    df = pd.read_csv('analysis/SAT2_timing.csv')
    plot = sns.lineplot(data=df, x='n', y='t')
    plot.get_figure().savefig('analysis/SAT2_timing.png')
    plot.get_figure().clf()
    df = pd.read_csv('analysis/Square_timing.csv')
    plot = sns.lineplot(data=df, x='n', y='t', hue='dim')
    plot.get_figure().savefig('analysis/Square_timing.png')
    plot.get_figure().clf()
    df = pd.read_csv('analysis/Select_timing.csv')
    plot = sns.lineplot(data=df, x='n', y='t')
    plot.get_figure().savefig('analysis/Select_timing.png')
    plot.get_figure().clf()
    df = pd.read_csv('result/points.csv')
    plot = sns.scatterplot(data=df, x='dim0', y='dim1', hue='color', palette=sns.color_palette("hls", len(df)//2))
    plt.axis("equal")
    plot.get_figure().savefig('result/points.png') 
    plot.get_figure().clf()

except Exception as e: 
    print(e)