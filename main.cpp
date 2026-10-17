#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "structs.hpp"

// if the value is not in 0-255 we should set a limit value
// else value is saved
void check(int& value) {
    if (value < 0) { value = 0; }
    else if (value > 255) { value = 255; }
}

template <typename Img>
void multiplication(int x, int y, const Img& image, Img& newimage, const std::vector<std::vector<int>>& matrix) {
    int r = 0;
    int g = 0;
    int b = 0;

    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int k = matrix[i + 1][j + 1];
            r += image[y + i][x + j].r * k;
            g += image[y + i][x + j].g * k;
            b += image[y + i][x + j].b * k;
        }
    }
    
    check(r);
    check(g);
    check(b);

    newimage[y][x].r = static_cast<uint8_t>(r);
    newimage[y][x].g = static_cast<uint8_t>(g);
    newimage[y][x].b = static_cast<uint8_t>(b);
}

int main(int argc, char* argv[]) {
    // expected command is: *.exe InputFile.bmp OutputFile.bmp number_of_neighbours
    // where number_of_neighbours is 4 or 8
    if (argc != 4 || (std::string(argv[3]) != "4" && std::string(argv[3]) != "8")) { 
        std::cout << "Unknown format. Use --help..." << std::endl;
        return 1;
    }

    // reference matrix
    std::vector<std::vector<int>> matrix;
    if (std::string(argv[3]) == "4") {
        matrix = {
            {0, -1, 0},
            {-1, 5, -1},
            {0, -1, 0}
        };
    }
    else {
        matrix = {
            {-1, -1, -1},
            {-1, 9, -1},
            {-1, -1, -1}
        };
    }

    std::ifstream inputfile(argv[1], std::ios::binary);
    if (!inputfile) {
        std::cout << "Can not open input file" << std::endl;
        return 1;
    }

    std::ofstream outputfile(argv[2], std::ios::binary);
    if (!outputfile) {
        std::cout << "Can not open output file" << std::endl;
        inputfile.close();
        return 1;
    }

    // first heading
    BMPfileheader fileheader;
    inputfile.read(reinterpret_cast<char*>(&fileheader), sizeof(fileheader));
    // second heading
    BMPinfoheader infoheader;
    inputfile.read(reinterpret_cast<char*>(&infoheader), sizeof(infoheader));

    std::vector<std::vector<Pixel>> image(infoheader.height, std::vector<Pixel>(infoheader.width));
    inputfile.seekg(fileheader.offset, std::ios::beg);

    int h = infoheader.height;
    int w = infoheader.width;
    int p = (4 - (w * 3) % 4) % 4;

    // reading pixels and writting in an array
    for (int y = h - 1; y >= 0; --y) {
        for (int x = 0; x < w; ++x) {
            Pixel pixel;
            inputfile.read(reinterpret_cast<char*>(&pixel), sizeof(Pixel));
            image[y][x] = pixel;
        }
        inputfile.ignore(p);
    }

    std::vector<std::vector<Pixel>> newimage(infoheader.height, std::vector<Pixel>(infoheader.width));

    for (int x = 0; x < w; ++x) {
        for (int y = 0; y < h; ++y) {
            // checking boundary pixel
            if (x == 0 || x == w - 1 || y == 0 || y == h - 1) {
                newimage[y][x] = image[y][x]; //preserve the value
                continue;
            }
            // changing pixels
            multiplication(x,y,image,newimage,matrix);
        }
    } 
    inputfile.close();

    outputfile.write(reinterpret_cast<const char*>(&fileheader), sizeof(fileheader));
    outputfile.write(reinterpret_cast<const char*>(&infoheader), sizeof(infoheader));

    // save result bmp
    char pad[3] = {0, 0, 0};
    for (int y = h - 1; y >= 0; --y) {
        for (int x = 0; x < w; ++x) {
            outputfile.write(reinterpret_cast<const char*>(&newimage[y][x]), sizeof(Pixel));
        }
        outputfile.write(pad,p);
    }
    outputfile.close();

    return 0;
}
