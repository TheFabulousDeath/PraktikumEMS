import pandas as pd
import plotly.express as px

#Make sure the csv has column names on the top
df = pd.read_csv('J:/task1Log_112108_02_05_2024.csv')
df_melted = df.melt(id_vars='Time', value_vars=['x', 'y', 'z'], var_name='Variable', value_name='Value')
fig = px.line(df_melted, x='Time', y='Value', color='Variable', title='Values over Time')
fig.show()
