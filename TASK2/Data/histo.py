import sys
import pandas as pd
import plotly.express as px

# Load the data from the CSV file
data = pd.read_csv(sys.argv[1])

# Extract the pressure column
pressure_data = data.iloc[:, 0]

# Create a histogram using Plotly
fig = px.histogram(pressure_data, x=pressure_data, nbins=100, title='Histogram of Pressure Readings')
fig.update_layout(xaxis_title='Pressure', yaxis_title='Count')

# Show the plot
fig.show()
