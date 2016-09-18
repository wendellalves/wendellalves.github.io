#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image;
  //int width, height;
  VideoCapture cap;
  vector<Mat> planes;
  Mat hist, histAux;
  int nbins = 64;
  float range[] = {0, 256};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;

  cap.open(0);
  
  if(!cap.isOpened()){
    cout << "cameras indisponiveis";
    return -1;
  }

  cap >> image;
  split (image, planes);

  calcHist(&planes[0], 1, 0, Mat(), hist, 1,
           &nbins, &histrange,
           uniform, acummulate);

  while(1){
    cap >> image;
    split (image, planes);

    calcHist(&planes[0], 1, 0, Mat(), histAux, 1,
             &nbins, &histrange,
             uniform, acummulate);

    float somador = 0;

    for(int i=0; i<nbins; i++){
        somador = somador + abs(cvRound(hist.at<float>(i)) - cvRound(histAux.at<float>(i)));
    }
// Esse valor vai de acordo com a qualidade da sua camera, quanto mais ruido maior deve ser
    if(somador > (20000)){ 
      cout << "Alerta de Intruso!!!" << endl;
    }

    hist = histAux.clone();

    imshow("image", image);
    if(waitKey(30) >= 0) break;
  }
  return 0;
}
