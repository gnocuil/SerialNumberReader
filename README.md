Install

1. Install python2.7 and python image library:
    sudo apt-get install python-imaging

2. Install tesseract
For windows: 
    See https://github.com/tesseract-ocr/tesseract/wiki
    
For Linux (Ubuntu):
    sudo apt-get install tesseract-ocr
    
*INPORTANT: you may need to download an extra .traineddata file. See https://github.com/tesseract-ocr/tesseract/wiki.



Usage
1. Scan one jpg/png file:
    python image.py -i input.jpg

The result will be printed to screen directly.

2. Scan all jpg/png files in a directory:
    python image.py -f input_directory

The result will be saved to <input_directory/serial.csv>.
