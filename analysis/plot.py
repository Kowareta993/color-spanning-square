import seaborn as sns
import pandas as pd
try:
    df = pd.read_csv('SAT2_timing.csv')
    plot = sns.lineplot(data=df, x='n', y='t')
    plot.get_figure().savefig('SAT2_timing.png')
    plot.get_figure().clf()
    df = pd.read_csv('Square_timing.csv')
    plot = sns.lineplot(data=df, x='n', y='t', hue='dim')
    plot.get_figure().savefig('Square_timing.png')
    plot.get_figure().clf()
    df = pd.read_csv('Select_timing.csv')
    plot = sns.lineplot(data=df, x='n', y='t')
    plot.get_figure().savefig('Select_timing.png')
    plot.get_figure().clf()
except Exception as e: 
    print(e)