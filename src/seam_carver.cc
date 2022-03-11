#include "seam_carver.hpp"
#include "pixel.hpp"

// implement the rest of SeamCarver's functions here

const ImagePPM& SeamCarver::GetImage() const {
  return image_;
}

int SeamCarver::GetHeight() const {
  return height_;
}

int SeamCarver::GetWidth() const {
  return width_;
}

int SeamCarver::GetEnergy(int row, int col) const {
  
  int col_minus_one = col - 1;
  int col_plus_one = col + 1;
  int row_minus_one = row - 1;
  int row_plus_one = row + 1;

  
  if (col_minus_one < 0) {
    col_minus_one = width_ - 1;
  }
  if (col_plus_one > width_ - 1) {
    col_plus_one = 0;
  }
  if (row_minus_one < 0) { 
    row_minus_one = height_ - 1;
  }
  if (row_plus_one > height_ - 1) {
    row_plus_one = 0;
  }

  int r_col = abs(image_.GetPixel(row, col_minus_one).GetRed() - image_.GetPixel(row, col_plus_one).GetRed());
  int g_col = abs(image_.GetPixel(row, col_minus_one).GetGreen() - image_.GetPixel(row, col_plus_one).GetGreen());
  int b_col = abs(image_.GetPixel(row, col_minus_one).GetBlue() - image_.GetPixel(row, col_plus_one).GetBlue());

  int delta_col_sqr = (r_col * r_col) + (g_col * g_col) + (b_col * b_col);

  int r_row = abs(image_.GetPixel(row_minus_one, col).GetRed() - image_.GetPixel(row_plus_one, col).GetRed());
  int g_row = abs(image_.GetPixel(row_minus_one, col).GetGreen() - image_.GetPixel(row_plus_one, col).GetGreen());
  int b_row = abs(image_.GetPixel(row_minus_one, col).GetBlue() - image_.GetPixel(row_plus_one, col).GetBlue());

  int delta_row_sqr = (r_row * r_row) + (g_row * g_row) + (b_row * b_row);

  //calculating energy
  int energy = delta_col_sqr + delta_row_sqr;

  return energy;
  
}

int* SeamCarver::GetHorizontalSeam() const {
  std::cout << "GetHorizontalSeam was called" << std::endl;
  int** values = nullptr;
  values = new int* [height_];
  for (int row = 0; row < height_; row++) {
    values[row] = new int[width_];
  }  
  for (int row = 0; row < height_; row++) {
    for (int col = 0; col < width_; col ++) {
      values[row][col] = GetEnergy(row, col);
    }
  }
  for (int row = 0; row < height_; row++) {
    for (int col = width_-2; col >= 0; col--) {
      if (row - 1 < 0 || row + 1 >= height_) {
        if (row - 1 < 0) {
          int left = values[row][col + 1];
          int left_down = values[row + 1][col + 1];
          int best = left <= left_down ? left : left_down;
          values[row][col] = best + GetEnergy(row, col);
        }
        else if (row + 1 >= height_) {
          int left_up = values[row - 1][col + 1];
          int left = values[row][col + 1];
          int best = left <= left_up ? left : left_up;
          values[row][col] = best + GetEnergy(row, col);
        }
      }
      else if (row - 1 >= 0 && row + 1 < height_) {
        int best = FindMin(values[row - 1][col + 1], values[row][col + 1], values[row + 1][col + 1]);
        values[row][col] = best + GetEnergy(row, col);
      }
      else {
          int best = values[row][col+1];
          values[row][col] = best + GetEnergy(row, col);
      }
    }
  }
  std::cout << "found best in GetHorizontalSeam" << std::endl;
  return (int*)values;
}

int* SeamCarver::GetVerticalSeam() const {
  int** values = nullptr;
  values = new int* [height_];
  for (int row = 0; row < height_; row++) {
    values[row] = new int[width_];
  }
  std::cout << "successfully constructed values in GetVerticalSeam" << std::endl;
  for (int row = 0; row < height_; row++) {
    for (int col = 0; col < width_; col ++) {
      values[row][col] = GetEnergy(row, col);
    }
  }
  for (int row = height_ - 2; row >= 0; row--) {
    for (int col = 0; col < width_; col++) {
      if (col - 1 < 0 || col + 1 >= width_) {
        if (col - 1 < 0) {
          int down = values[row + 1][col];
          int down_right = values[row + 1][col + 1];
          int best = down_right < down ? down_right : down;
          values[row][col] = best + GetEnergy(row, col);
        }
        else if (col + 1 >= width_) {
          int down = values[row + 1][col];
          int down_left = values[row + 1][col - 1];
          int best = down_left < down ? down_left : down;
          values[row][col] = best + GetEnergy(row, col);
        }
      }
      else if (col - 1 >= 0 && col + 1 < width_) {
        int best = FindMin(values[row + 1][col + 1], values[row + 1][col], values[row + 1][col - 1]);
        values[row][col] = best + GetEnergy(row, col);
      } 
      else {
        int best = values[row+1][col];
        values[row][col] = best + GetEnergy(row,col);
      }
    }
  }
  std::cout << "found best in GetVerticalSeam" << std::endl;
  return (int*)values;
}

int SeamCarver::FindMin(int num_1, int num_2, int num_3) const {

  int smallest = kMaxNum;
  if (num_1 <= smallest) {
    smallest = num_1;
  }
  if (num_2 <= smallest) {
    smallest= num_2;
  }
  if(num_3 <= smallest) {
    smallest= num_3;
  }
  return smallest;
}

int SeamCarver::FindMinIdx(int num_1, int idx_1, int num_2, int idx_2, int num_3, int idx_3) const {

  int smallest = kMaxNum;
  int smallest_idx = idx_1;
  if (num_1 <= smallest) {
    smallest_idx = idx_1;
  }
  if (num_2 <= smallest) {
    smallest_idx = idx_2;
  }
  if(num_3 <= smallest) {
    smallest_idx = idx_3;
  }
  return smallest_idx;
}

int SeamCarver::FindMinIdx(int num_1, int idx_1, int num_2, int idx_2) const {
  int smallest = kMaxNum;
  int smallest_idx = idx_1;

  if (num_1 <= smallest) {
    smallest_idx = idx_1;
  }
  if (num_2 <= smallest) {
    smallest_idx = idx_2;
  }
  return smallest_idx;
}

int* SeamCarver::CarveSeamHorizontal() const {
  int** hor_seam = (int**)GetHorizontalSeam();
  std::cout << "size of int** is: " << sizeof(int**) << " and size of int* is: " << sizeof(int*) << std::endl;
  std::cout << "address of vertical seam is: " << hor_seam << std::endl;
  std::cout << "this is the width: " << width_ << std::endl;
  int* seam_carver = new int [width_];
  std::cout << "pointer to pointer hor seam finder created in CarveSeamHorizontal" << std::endl;
  int min_val = kMaxNum;
  int min_idx = hor_seam[0][0];
  for (int j = 0; j < height_; j++) {
    if (hor_seam[j][0] < min_val) {
      min_val = hor_seam[j][0];
      min_idx = j;
    }
  }
  seam_carver[0] = min_idx;

  for (int i = 1; i < width_; i++) {
    if (min_idx - 1 < 0) {
      int right_up = hor_seam[min_idx + 1][i];
      int right = hor_seam[min_idx][i];
      min_idx = FindMinIdx(right, min_idx, right_up, min_idx + 1);
    } 
    else if (min_idx + 1 >= height_) {
      int right = hor_seam[min_idx][i];
      int right_down = hor_seam[min_idx - 1][i];
      min_idx = FindMinIdx(right, min_idx, right_down, min_idx - 1);
    }
    else if (min_idx - 1 >= 0 && min_idx + 1 < height_) {
        min_idx = FindMinIdx(hor_seam[min_idx - 1][i], min_idx - 1, hor_seam[min_idx][i], min_idx, hor_seam[min_idx + 1][i], min_idx + 1);
    }
    seam_carver[i] = min_idx;
  }
  std::cout << "done with carve horizontal, deleting" << std::endl;
  for (int i = 0; i < height_; i++) {
    delete[] hor_seam[i];
  } 
  delete[] hor_seam;
  hor_seam = nullptr; 
  return seam_carver;
}

int* SeamCarver::CarveSeamVertical() const {
  int** vert_seam = (int**)GetVerticalSeam();
  int* seam_carver = new int [height_];
  
  int min_val = kMaxNum;
  int min_idx = vert_seam[0][0];
  for (int j = 0; j < width_; j++) {
    if (vert_seam[0][j] < min_val) {
      min_val = vert_seam[0][j];
      min_idx = j;
    }
  }
  seam_carver[0] = min_idx;

  for (int i = 1; i < height_; i++) {
    if (min_idx - 1 < 0) {
      int down_right = vert_seam[i][min_idx + 1];
      int down = vert_seam[i][min_idx];
      min_idx = FindMinIdx(down, min_idx, down_right, min_idx + 1);
    } 
    else if (min_idx + 1 >= width_) {
      int down = vert_seam[i][min_idx];
      int down_left = vert_seam[i][min_idx - 1];
      min_idx = FindMinIdx(down, min_idx, down_left, min_idx - 1);
    } 
    else if (min_idx - 1 >= 0 && min_idx + 1 < width_) {
      int down_right = vert_seam[i][min_idx + 1];
      int down = vert_seam[i][min_idx];
      int down_left = vert_seam[i][min_idx - 1];
      std::cout << "down right is: " << down_right << " down is: " << down << " down_left is: " << down_left << std::endl;
      min_idx = FindMinIdx(down_right, min_idx + 1, down, min_idx, down_left, min_idx - 1);
    }
    seam_carver[i] = min_idx;
  }
  for (int i = 0; i < height_; i++) {
      delete[] vert_seam[i];
  } 
  delete[] vert_seam;
  vert_seam = nullptr;  
  return seam_carver;
}

void SeamCarver::RemoveHorizontalSeam() { 
  std::cout << "Remove Horizontal Seam was called" << std::endl; 
  int* horizontal_seam = CarveSeamHorizontal();
  std::cout << "CarveSeamHorizontal was successfully implemented" << std::endl;
  image_.RemoveHorizontalSeam(horizontal_seam);
  std::cout << "horizontal seam was removed successfully" << std::endl;
  height_ = image_.GetHeight();
}

void SeamCarver::RemoveVerticalSeam() {
  std::cout << "Remove Vertical Seam was called" << std::endl; 
  int* vertical_seam = CarveSeamVertical();
  std::cout << "CarveSeamVertical was successfully implemented" << std::endl;
  image_.RemoveVerticalSeam(vertical_seam);
  std::cout << "vertical seam was removed successfully" << std::endl;
  width_ = image_.GetWidth();
}




// given functions below, DO NOT MODIFY

SeamCarver::SeamCarver(const ImagePPM& image): image_(image) {
  height_ = image.GetHeight();
  width_ = image.GetWidth();
}

void SeamCarver::SetImage(const ImagePPM& image) {
  image_ = image;
  width_ = image.GetWidth();
  height_ = image.GetHeight();
}
