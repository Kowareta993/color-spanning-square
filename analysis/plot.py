import seaborn as sns
import pandas as pd

try:
    df = pd.read_csv('SAT2_timing.csv')
    sns.lineplot(data=df, x='n', y='t').get_figure().savefig('SAT2_timing.png')
except Exception as e: 
    print(e)