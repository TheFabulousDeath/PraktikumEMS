import os
import sys
import glob
import pandas as pd
import plotly.graph_objects as go

def process_csv(file_path):
    # Load the CSV file into a pandas DataFrame
    data = pd.read_csv(file_path)

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
    fig.update_layout(title=f'Pressure and XYZ Readings for {os.path.basename(file_path)} with Double Cumulative Sum of Z',
                      xaxis_title='Natural Numbers',
                      yaxis_title='Readings',
                      legend_title='Legend')

    # Show the plot
    fig.show()

def main():
    # Check for folder path from the command line argument
    if len(sys.argv) == 2:
        folder_path = sys.argv[1]
    else:
        # Default to the current working directory
        folder_path = os.getcwd()

    # Get a list of all CSV files in the folder
    csv_files = glob.glob(os.path.join(folder_path, '*.csv'))

    # Process each CSV file
    for csv_file in csv_files:
        process_csv(csv_file)

if __name__ == "__main__":
    main()
