#Compile

1. Install CImg library:

    sudo apt-get install cimg-dev

2. make

Note: please modify the variable cmd in main.cpp if you use a different command of tesseract.

#Usage

1. Scan one jpg file:

    ./image -i input.jpg

The result will be printed to screen directly.

2. Scan all jpg files in a directory:

    ./image -f input_directory

The result will be saved to <input_directory/serial.csv>.
