#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

int main(int, char**){
  Mat image;
  Vec3b val;
  CvPoint p1, p2;

  cout << "Digite as coordenadas do ponto P1:" << endl;
  cin >> p1.x;
  cin >> p1.y;

  cout << "Digite as coordenadas do ponto P2:" << endl;
  cin >> p2.x;
  cin >> p2.y;

  image= imread("biel.png",CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data)
    cout << "Nao abriu biel.png" << endl;

  namedWindow("janela",WINDOW_AUTOSIZE);

  for(int i= p1.x;i<p2.x;i++){
    for(int j=p1.y;j<p2.y;j++){
      image.at<uchar>(i,j) = (255 - image.at<uchar>(i,j));
    }
  }

  imshow("janela", image);  
  waitKey();
  imwrite("saidaNegativoRegiao.png", image);
  return 0;
}
