# LSB Steganography Tool

A command line utility which allows reading, writing and analysis of hidden messages in images.

## Table of Contents

1. [Overview](#overview)
2. [Usage](#usage)
    1. [Building](#building)
    2. [Execution](#execution)
3. [Images](#images)
4. [Steganography](#steganography)
    1. [Encoding](#encoding)
    2. [Bytes](#bytes)
    3. [Primitives](#primitives)
    4. [Messages](#messages)
5. [Steganalysis](#steganalysis)

## Overview

<img align="right" width="50%" height="auto" src="https://github.com/user-attachments/assets/d912354f-c649-4b72-b92b-befe58bb5bab">

The features this program offers include:

- Programmer interface for reading and writing data ([primitive types and bytes](#bytes)) from images.
- User interface for reading and writing messages (uses a [structured protocol](#messages)) from images.
- Analysis of images using chi-squared and RS methods.

Currently only supports `.bmp` files, but is easily extensible.

## Usage

### Building

Building requires a C++ compiler supporting C++ 20 and `cmake`.

To build simply run `cmake -S . -B build` and `cmake --build build/`.

The compiled binary will be in `build/` called `steg`.

### Execution

<img align="right" width="35%" height="auto" src="https://github.com/user-attachments/assets/c465beba-0a7c-4de7-824b-3c6ac5327df0">

This program uses a non-interactive CLI. To execute it you must run the binary like so:

```
steg <command> <path> [OPTIONS]
```

`path` is the path to the file you want to use.

`OPTIONS` are dependent on the `command` you use. Available commands and their options are:

- `read` - Reads one of the messages embedded inside your image.

    - `-i`, `--index` **(optional)** - Index of the message you want to read. An image may contain more than 1 message, so specifying this allows you to read a single one of them. `0` by default.

    Examples:

    Reading the first message of `image.bmp`:
    ```
    steg read image.bmp
    ```

    Reading the second message of `image.bmp`:
    ```
    steg read image.bmp -i 1
    ```

- `readall` - Reads all messages embedded in the image.

    Examples:
    ```
    steg readall image.bmp
    ```

- `write` - Writes a new hidden message inside your image. Can be used multiple times on one image without losing data.

    - `-m`, `--message` **(required)** - Content of the message to be written.
    - `-o`, `--output` **(optional)** - Name of the output image. If not specified it's saved as the input name with `m_` prefix.

    Examples:

    Reading `image.bmp` and writing `message`. Outputs `m_image.bmp`:
    ```
    steg write image.bmp -m "message"
    ```

    Reading `image.bmp`, writing "hello world" and saving it as `hello.bmp`:
    ```
    steg write image.bmp -m "hello world" -o hello.bmp
    ```

- `analyze` - Performs steganalysis on your image. Method depends on options.

    You must use either `--chi2` or `--rs`. Not specifying defaults to `--chi2`

    - `--chi2` - Performs chi-squared attack on each channel of your input image.
    - `--rs` - Performs RS analysis on each channel of your input image.
    - `--rs-group-size` - Only valid when using the RS method. Specifies the algorithm's group size. When using this you must also specify `--rs-mask`.
    - `--rs-mask` - Only valid when using the RS method. Specifies the algorithm's mask. It must be a series of integers divided by `,`. When using this you must also specify `--rs-group-size`. The amount of numbers must match the group size.

    Examples:

    Performing chi-squared analysis on `image.bmp`:
    ```
    steg analyze image.bmp --chi2
    ```

    Performing RS analysis with default arguments (`group-size = 4`, `mask = { 1, 0, 1, 0 }`):
    ```
    steg analyze image.bmp --rs
    ```

    Performing RS analysis with `group-size = 2`, `mask = { 1, 0 }`:
    ```
    steg analyze image.bmp --rs --rs-group-size 2 --rs-mask 1,0
    ```

## Images

`Image` is an abstract class which provides a basic interface for accessing and modifying an image's pixel data array, without worrying about its format.

All types of images should derive from it. Currently only `BMPImage` is available.

You shouldn't create instances of `Image` yourself. Here, the program uses a Factory pattern - `ImageLoader`, given the file path, looks at the contents and creates the correct instance for you.

## Steganography

### Encoding

This tool uses `LSB replacement`. This means that the least significant bit of every byte is set to a consecutive bit of the data you want to write.

Giving an abstract and minimal example: if you wanted to write `0b1010` into your image with the content `0b11111111 0b11111111 0b11111111 0b11111111` that would output: 

```
0b11111111 0b11111110 0b11111111 0b11111110
         ^1         ^0         ^1         ^0
```

All reading an writing is done using `vectors` and not memory access. This completely bypasses the problem of endianness.

### Bytes

`Steganographer` is the class which gives you the interface for working with the embedded data inside images. It allows to read and write series of bytes into images (represented by `std::vector<uint8_t>`). For example:

```c++
std::vector<uint8_t> input = { 0b10101010, 0b11110000 };
steganographer.write(image, 0, input); // middle number (0 here) is the index where the input should start in the pixel data array
std::vector<uint8_t> output = steganographer.read(image, 0, 2); // middle number is same as above, second number is how many bytes to read
```

### Primitives

The `Steganographer` class' interface also allows for easy embedding of variables into images. Due to the use of templates any integral type (`int`, `char`, `bool`) is usable like:

```c++
steganographer.write<int>(image, 5); // writes 5
int output = steganographer.read<int>(image);
```

### Messages

The `Messenger` class is responsible for writing and reading more complex data than simple variables or bytes.

It applies the following protocol to allow for easy access to data inside the image:

```
[number of messages][message 0 length][message 0 content][message 1 length][... and so on]
```

Where:

- `number of messages` is `uint32`
- `message length` is `uint32`
- `message content` is a series of `chars`

## Steganalysis

The program currently supports two types of steganalysis:

- Chi-squared - Scans pairs of values on the histogram. Outputs a float representing the chi-squared statistic, which is the sum of squared deviations within pairs. The lower the number, the higher the chance of steganography, as embedding data equalizes these values.

- RS - Measures how the image reacts to bit flips. Returns the estimated embedding rate, which is essentially how noisy an image is. The higher the embedding rate, the higher the chance of steganography.
