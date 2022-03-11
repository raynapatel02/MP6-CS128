#include "image_ppm.hpp"

// implement the rest of ImagePPM's functions here

Pixel ImagePPM::GetPixel(int row, int col) const {
    Pixel* p_row_pixel = *(pixels_ + row);
    Pixel pixel = *(p_row_pixel + col);
    //std::cout << "address of pixels_ is: " << pixels_ << std::endl;
    //std::cout << "new row of pixels_ is:" <<pixels_ + 1 << std::endl;
    return pixel;

}

int ImagePPM::GetMaxColorValue() const {
  return max_color_value_;
}

// given functions below, DO NOT MODIFY

ImagePPM::ImagePPM(const std::string& path) {
  std::ifstream input_file(path);
  input_file >> *this;
  input_file.close();
}

ImagePPM::ImagePPM(const ImagePPM& source) { *this = source; }

ImagePPM& ImagePPM::operator=(const ImagePPM& source) {
  if (this == &source) {
    return *this;
  }

  Clear();

  height_ = source.height_;
  width_ = source.width_;
  max_color_value_ = source.max_color_value_;

  pixels_ = new Pixel*[height_];
  for (int row = 0; row < height_; row++) {
    pixels_[row] = new Pixel[width_];

    for (int col = 0; col < width_; col++) {
      pixels_[row][col] = source.pixels_[row][col];
    }
  }

  return *this;
}

ImagePPM::~ImagePPM() { Clear(); }

void ImagePPM::Clear() {
  for (int i = 0; i < height_; i++) {
    delete[] pixels_[i];
  }

  delete[] pixels_;

  height_ = 0;
  width_ = 0;
  pixels_ = nullptr;
}

std::ostream& operator<<(std::ostream& os, const ImagePPM& image) {
  //to do implement me
  os << "P3" << std::endl;
  os << image.width_ << " " << image.height_ << std::endl;
  os << image.max_color_value_ << std::endl;
  for (int row = 0; row < image.height_; row++) {
    for (int col = 0; col < image.width_; col++) {
      Pixel pixel = image.pixels_[row][col];
      os << pixel.GetRed() << '\n';
      os << pixel.GetGreen() << '\n';
      os << pixel.GetBlue() << '\n';
    }
  }
  return os;
}


std::istream& operator>>(std::istream& is, ImagePPM& image) {
  image.Clear();
  std::string line;
  // ignore magic number line
  getline(is, line);
  // check to see if there's a comment line
  getline(is, line);
  if (line[0] == '#') {
    getline(is, line);
  }
  // parse width and height
  int space = line.find_first_of(' ');
  image.width_ = std::stoi(line.substr(0, space));
  image.height_ = std::stoi(line.substr(space + 1));
  // get max color value
  getline(is, line);
  image.max_color_value_ = std::stoi(line);
  // init and fill in Pixels array
  image.pixels_ = new Pixel*[image.height_];
  for (int i = 0; i < image.height_; i++) {
    image.pixels_[i] = new Pixel[image.width_];
  }
  for (int row = 0; row < image.height_; row++) {
    for (int col = 0; col < image.width_; col++) {
      getline(is, line);
      int red = std::stoi(line);
      getline(is, line);
      int green = std::stoi(line);
      getline(is, line);
      int blue = std::stoi(line);

      Pixel p(red, green, blue);
      image.pixels_[row][col] = p;
    }
  }
  return is;
}

void ImagePPM::RemoveHorizontalSeam(int* horizontal_seam) {
  //create 2d array with one less height
  Pixel** pixels = new Pixel*[height_ - 1];
  std::cout << "height is: " << height_ - 1 << "width is: " << width_ << std::endl;

  for (int row = 0; row < height_ -  1; row++) {
    pixels[row] = new Pixel[width_];
  }
  int nrow = 0;
  for (int col = 0; col < width_; col++) {
    for (int row = 0; row < height_; row++) {
      if (horizontal_seam[col] != row) {
        if (nrow == height_ - 1) {
          std::cout << "nrow is: " << nrow << " col is: " << col << std::endl;
          std::cout << "horizontal_seam[col] is: " << horizontal_seam[col] << std::endl;
        }
        pixels[nrow][col] = pixels_[row][col];
        nrow++;
      }
    }
    nrow = 0;
  }
  pixels_ = pixels;
  height_ = height_ - 1;

  delete[] horizontal_seam;
  horizontal_seam = nullptr;
}

void ImagePPM::RemoveVerticalSeam(int* vertical_seam) {
  //create 2d array with one less width
  Pixel** pixels = new Pixel*[height_];
  std::cout << "height is: " << height_ << "width is: " << width_ - 1 << std::endl;
  for (int row = 0; row < height_; row++) {
    pixels[row] = new Pixel[width_ - 1];
  }
  int ncol = 0;
  for (int row = 0; row < height_; row++) {
    for (int col = 0; col < width_; col++) {
      if (vertical_seam[row] != col) {
        //std::cout << "ncol is: " << ncol << "row is: " << row << std::endl;
        pixels[row][ncol] = pixels_[row][col];
        ncol++;
      }
    }
    ncol = 0;
  }
  pixels_ = pixels;
  width_ = width_ - 1;

  delete[] vertical_seam;
  vertical_seam = nullptr;
}