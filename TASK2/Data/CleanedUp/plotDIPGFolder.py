import os
import sys
import glob
import pandas as pd
import plotly.graph_objects as go
import numpy as np

def convert_accelerometer_data(raw_data, sensitivity=4096):
    return raw_data / sensitivity

def process_csv(file_path, temperature_k=288.15, accelerometer_sensitivity=4096):
    # Constants for the barometric formula
    R = 8.3144598  # J/(mol·K)
    g = 9.80665  # m/s²
    M = 0.0289644  # kg/mol
    
    # Load the CSV file into a pandas DataFrame
    data = pd.read_csv(file_path)

    # Extract the columns
    pressure = data.iloc[:, 0]
    raw_x = data.iloc[:, 1]
    raw_y = data.iloc[:, 2]
    raw_z = data.iloc[:, 3]

    # Convert raw accelerometer data to g
    x = convert_accelerometer_data(raw_x, accelerometer_sensitivity)
    y = convert_accelerometer_data(raw_y, accelerometer_sensitivity)
    z = convert_accelerometer_data(raw_z, accelerometer_sensitivity)

    # Create a sequence of natural numbers starting from 0
    natural_numbers = range(len(data))

    # Reference pressure (first data point)
    P0 = pressure.iloc[0]

    # Calculate altitude difference using the barometric formula
    altitude_diff = (R * temperature_k / (g * M)) * np.log(P0 / pressure)
    
    # Add the altitude difference as a new column to the DataFrame
    data['Altitude Difference'] = altitude_diff

    # Calculate the double cumulative sum of the z column
    z_cumsum = z.cumsum().cumsum()

    # Create traces for each column
    trace_pressure = go.Scatter(x=list(natural_numbers), y=pressure, mode='lines', name='Pressure')
    trace_x = go.Scatter(x=list(natural_numbers), y=x, mode='lines', name='X')
    trace_y = go.Scatter(x=list(natural_numbers), y=y, mode='lines', name='Y')
    trace_z = go.Scatter(x=list(natural_numbers), y=z, mode='lines', name='Z')
    trace_r = go.Scatter(x=list(natural_numbers), y=z_cumsum, mode='lines', name='R (Double Cumulative Sum of Z)')
    trace_altitude = go.Scatter(x=list(natural_numbers), y=altitude_diff, mode='lines', name='Altitude Difference')

    # Combine the traces into a single figure
    fig = go.Figure()
    fig.add_trace(trace_pressure)
    fig.add_trace(trace_x)
    fig.add_trace(trace_y)
    fig.add_trace(trace_z)
    fig.add_trace(trace_r)
    fig.add_trace(trace_altitude)

    # Update layout
    fig.update_layout(title=f'Pressure and XYZ Readings for {os.path.basename(file_path)} with Altitude Difference',
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
