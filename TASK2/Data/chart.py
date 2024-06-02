import pandas as pd
import plotly.express as px

df = pd.read_csv(sys.argv[1])
df_melted = df.melt(id_vars='Time/ms', value_vars=['Pressure/hPa'], var_name='Variable', value_name='Value')
fig = px.line(df_melted, x='Time/ms', y='Value', color='Variable', title='Drift Measurement')

fig.show()
