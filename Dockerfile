FROM ubuntu:22.04

# Install system packages
RUN apt-get update && apt-get install -y \
    cmake \
    g++ \
    python3 \
    python3-pip \
    make

# Set working directory inside the container
WORKDIR /app

# Copy everything from your project into the container
COPY . .

# Install Flask
RUN pip3 install --no-cache-dir -r requirements.txt

# Build your C++ code
RUN mkdir build && cd build && cmake .. && cmake --build . --config Release

# Set Flask environment variable
ENV FLASK_APP=app.py
ENV FLASK_RUN_PORT=8080

# Expose the port that Flask will use
EXPOSE 8080

# Start the Flask server
CMD ["flask", "run", "--host=0.0.0.0", "--port=8080"]
