import pandas as pd
import plotly.express as px

df = pd.read_csv('J:/task1Log_195605_14_05_2024.csv')
df_melted = df.melt(id_vars='Time', value_vars=['xAcc', 'yAcc', 'zAcc', 'zGyro'], var_name='Variable', value_name='Value')
fig = px.line(df_melted, x='Time', y='Value', color='Variable', title='Values over Time')

fig.show()
