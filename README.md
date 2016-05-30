#Compile

## Install CImg library:

    sudo apt-get install cimg-dev

## Compile

    make
    
Note: please modify the variable cmd in main.cpp if you use a different command of tesseract.

## Install tesseract

For windows: see https://github.com/tesseract-ocr/tesseract/wiki .

You may need to download an extra .traineddata file.

    
For Linux (Ubuntu):

    sudo apt-get install tesseract-ocr
    

#Usage

## Scan one jpg/png file:

    ./image -i input.jpg

The result will be printed to screen directly.

## Scan all jpg/png files in a directory:

    ./image -f input_directory

The result will be saved to input_directory/serial.csv .
