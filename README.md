
# Lightweight Authentic Wireless Communications for Micro:Bit IoT Device

The Micro:Bit IoT Authentication Project demonstrates a sophisticated system of secure communication between two Micro:Bit devices. The core of this project lies in its use of advanced cryptographic techniques - AES for encryption, MD5 for hashing, and SHA-256 for enhanced security. This setup is pivotal in understanding the intricacies of IoT security, embedded systems programming, and secure wireless communication protocols.


## Project Setup and Execution Guide
This guide provides detailed instructions on how to set up the development environment in Visual Studio Code (VS Code), compile the project, and run it on Micro:Bit devices.
## Prerequisites

Before you begin, ensure you have the following installed:

Visual Studio Code (VS Code)
Python

C/C++ extension for VS Code
Micro:Bit USB drivers


## Cloning the Repository
Clone the project repository to your local machine using Git.

git clone [repository URL]

## Step 1: 
cd [repository directory]

## Step 2: Opening the Project in VS Code
Open the project directory in VS Code.

## Step 3: Installing Required Libraries

Ensure all required libraries for AES, MD5, and SHA-256 are included in your project directory. If they're not included, download and place them in the respective directories.

## Step 4: Compiling the Code
Open the terminal in VS Code.
Navigate to the directory containing sender.cpp and rec.cpp.

Run the build script for each file

python build.py sender.cpp
python build.py rec.cpp

This will compile the code and generate hex files for each Micro:Bit.

## Step 5: Uploading to Micro:Bit
Connect the Micro:Bit devices to your computer via USB. 
Copy the hex files generated from the build process to each Micro:Bit device.
This can be done by dragging and dropping the hex files onto the respective Micro:Bit drives in your file explorer.

## Step 6: Running the Project
Reset the Micro:Bit devices to start the programs. Open a terminal and connect to the Micro:Bit using the following command:

sudo screen /dev/ttyACM0 115200

Interact with the Micro:Bit's functionalities by pressing buttons A, B, and A+B.

Troubleshooting
If you encounter any issues, ensure that:

All connections to the Micro:Bit are secure.

The correct hex files are uploaded to the respective Micro:Bit devices.

Make sure that you have the necessary permissions to execute commands (especially when using sudo).

## Support
For additional support or queries, please open an issue in the GitHub repository.


