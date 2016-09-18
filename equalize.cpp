#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image, saida;
  VideoCapture cap;
  vector<Mat> planes;

  cap.open(0);
  
  if(!cap.isOpened()){
    cout << "cameras indisponiveis";
    return -1;
  }

  while(1){
    cap >> image;
    split (image, planes);

    cvtColor(image, image, CV_BGR2GRAY);

    equalizeHist(image, saida);

    imshow("image", image);
    imshow("equalizada", saida);
    if(waitKey(30) >= 0) break;
  }
  imwrite("entradaEqualize.png", image);
  imwrite("saidaEqualize.png", saida);

  return 0;
}