# BMP Image Processing with Grayscale and Dithering

This program processes BMP images by transforming them into grayscale images and applying dithering to the resulting image.

### Requirements

This program requires the following:

- C++ compiler (GCC or Clang recommended)
- BMP image file (24-bit depth)
### Installation

1. Clone this repository to your local machine using https://github.com/username/bmp-image-processing.git
2. Compile the program with your C++ compiler using the bmp.cpp file.
Copy code
```
g++ bmp.cpp -o bmp
```
3. Run the program with an input image. The output image will be saved as resultImage.png.
### Usage

The program takes an input BMP image and performs grayscale and dithering operations on the image.

### Load Image
To load the input BMP image, use the Image.load() function:
```
Image input;
input.load("lena.bmp");
```
### Grayscale
To convert the input image to grayscale, use the grayscale() function:
```
Image output = grayscale(input);
```
### Dithering
To apply dithering to the grayscale image, use the dithering() function:
```
Image output = dithering(input);
```
### Save Image
To save the output image, use the Image.save() function:
```
output.save("resultImage.png");
```
###Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

###License

This project is licensed under the MIT License - see the LICENSE file for details.
