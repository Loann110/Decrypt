import serial
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Serial port configuration
SERIAL_PORT = "COM3"  # Replace with your Arduino's COM port
BAUD_RATE = 115200

# Initialize the serial connection
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)

# Initialize data storage
data = []

# Function to update the graph
def update(frame):
    global data
    
    if ser.in_waiting > 0:  # Check if data is available
        line = ser.readline().decode('utf-8').strip()  # Read a line of data
        try:
            value = float(line)  # Convert to float (if numeric)
            data.append(value)  # Add to data list
            if len(data) > 50:  # Keep the last 50 points
                data.pop(0)
        except ValueError:
            pass  # Ignore invalid data

    # Clear the plot and replot
    plt.cla()
    plt.plot(data, label="E32 Data")
    plt.legend(loc="upper right")
    plt.title("Live Data from E32")
    plt.xlabel("Time")
    plt.ylabel("Value")
    plt.grid()

# Configure the live plot
fig = plt.figure()
ani = FuncAnimation(fig, update, interval=100)  # Update every 100ms
plt.show()
