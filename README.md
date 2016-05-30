#Compile

## Install CImg library:

    sudo apt-get install cimg-dev

## Compile

    make
    
Note: please modify the variable cmd in main.cpp if you use a different command of tesseract.

## Install tesseract

For windows: See https://github.com/tesseract-ocr/tesseract/wiki
    
For Linux (Ubuntu):

    sudo apt-get install tesseract-ocr
    
INPORTANT: you may need to download an extra .traineddata file. See https://github.com/tesseract-ocr/tesseract/wiki.


#Usage

## Scan one jpg file:

    ./image -i input.jpg

The result will be printed to screen directly.

## Scan all jpg files in a directory:

    ./image -f input_directory

The result will be saved to input_directory/serial.csv .
