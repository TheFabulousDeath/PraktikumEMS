import sys
import pandas as pd
import plotly.graph_objects as go

# Load the CSV file into a pandas DataFrame
data = pd.read_csv(sys.argv[1])

# Extract the columns
pressure = data.iloc[:, 0]
x = data.iloc[:, 1]
y = data.iloc[:, 2]
z = data.iloc[:, 3]

# Create a sequence of natural numbers starting from 0
natural_numbers = range(len(data))

# Calculate the double cumulative sum of the z column
z_cumsum = z.cumsum().cumsum()

# Create traces for each column
trace_pressure = go.Scatter(x=list(natural_numbers), y=pressure, mode='lines', name='Pressure')
trace_x = go.Scatter(x=list(natural_numbers), y=x, mode='lines', name='X')
trace_y = go.Scatter(x=list(natural_numbers), y=y, mode='lines', name='Y')
trace_z = go.Scatter(x=list(natural_numbers), y=z, mode='lines', name='Z')
trace_r = go.Scatter(x=list(natural_numbers), y=z_cumsum, mode='lines', name='R (Double Cumulative Sum of Z)')

# Combine the traces into a single figure
fig = go.Figure()
fig.add_trace(trace_pressure)
fig.add_trace(trace_x)
fig.add_trace(trace_y)
fig.add_trace(trace_z)
fig.add_trace(trace_r)

# Update layout
fig.update_layout(title='Pressure and XYZ Readings Over Time with Double Cumulative Sum of Z',
                  xaxis_title='Natural Numbers',
                  yaxis_title='Readings',
                  legend_title='Legend')

# Show the plot
fig.show()
