#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char**){
  Mat image, imageAux;
  Vec3b val;

  image= imread("biel.png",CV_LOAD_IMAGE_GRAYSCALE);
  imageAux = image.clone();
  
  if(!image.data)
    cout << "nao abriu biel.png" << endl;

  namedWindow("janela",WINDOW_AUTOSIZE);
  
  for(int i=0, i1=(image.rows/2)  ; i<(image.rows/2), i1<image.rows; i++, i1++){
    for(int j=0, j1=(image.cols/2); j<(image.cols/2), j1<image.cols; j++, j1++){
      imageAux.at<uchar>(i1,j1) = image.at<uchar>(i,j);
    }
  }

  for(int i=(image.rows/2), i1=0  ; i<(image.rows), i1<(image.rows/2); i++, i1++){
    for(int j=0, j1=(image.cols/2); j<(image.cols/2), j1<image.cols  ; j++, j1++){
      imageAux.at<uchar>(i1,j1) = image.at<uchar>(i,j);
    }
  }

  for(int i=0, i1=(image.rows/2)  ; i<(image.rows/2), i1<(image.rows); i++, i1++){
    for(int j=(image.cols/2), j1=0; j<(image.cols), j1<(image.cols/2); j++, j1++){
      imageAux.at<uchar>(i1,j1) = image.at<uchar>(i,j);
    }
  }
  
  for(int i=(image.rows/2), i1=0  ; i<(image.rows), i1<(image.rows/2); i++, i1++){
    for(int j=(image.cols/2), j1=0; j<(image.cols), j1<(image.cols/2); j++, j1++){
      imageAux.at<uchar>(i1,j1) = image.at<uchar>(i,j);
    }
  }

  imshow("janela", imageAux);  
  waitKey();
  imwrite("trocaregioes.png", imageAux);
  return 0;
}