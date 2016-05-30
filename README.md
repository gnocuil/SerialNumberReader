#Install

## Install python2.7 and python image library:

    sudo apt-get install python-imaging

## Install tesseract

For windows: see https://github.com/tesseract-ocr/tesseract/wiki .

You may need to download an extra .traineddata file.

    
For Linux (Ubuntu):

    sudo apt-get install tesseract-ocr
  

#Usage

## Scan one jpg/png file:

    python image.py -i input.jpg
    

The result will be printed to screen directly.

## Scan all jpg/png files in a directory:

    python image.py -f input_directory

The result will be saved to input_directory/serial.csv.
