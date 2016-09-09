#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image, mask;
  int width, height;
  int nobjects = 0;
  int nburaco = 0;
  int ultimo = 0;
  
  CvPoint p,p1;
  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  
  if(!image.data){
    cout << "imagem nao carregou corretamente\n";
    return(-1);
  }
  width=image.size().width;
  height=image.size().height;

  p.x=0;
  p.y=0;

  p1.x=0;
  p1.y=0;

  //Inicia retirada de bolhas na borda
  for(int i=0; i<height; i++){
    if(image.at<uchar>(i,0) != 0){
      p.x=0;
      p.y=i;
      floodFill(image,p,0);
    }
  }
  
  for(int i=0; i<height; i++){
    if(image.at<uchar>(i,(width-1)) != 0){
      p.x=width-1;
      p.y=i;
      floodFill(image,p,0);
    }
  }

  for(int i=0; i<width; i++){
    if(image.at<uchar>(0,i) != 0){
      p.x=i;
      p.y=0;
      floodFill(image,p,0);
    }
  }

  for(int i=0; i<width; i++){
    if(image.at<uchar>((height-1),i) != 0){
      p.x=i;
      p.y=height-1;
      floodFill(image,p,0);
    }
  }//Termina retirada de bolhas da borda

  p.x=0;
  p.y=0;

  //busca objetos
  nobjects=0;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 255){
		    // achou um objeto
   	    nobjects++;
		    p.x=j;
		    p.y=i;
		    floodFill(image,p,nobjects);
      }
    }
  }
  //Mudar cor do fundo
  floodFill(image,p1,nobjects+1);
  
  //Encontrar buracos
  bool tburaco = false;
  for(int k=nobjects; k>0; k--){
    for(int i=0; i<height; i++){
      for(int j=0; j<width; j++){
        if(ultimo == k){
          if(image.at<uchar>(i,j) == 0){
            tburaco = true;
            p.x=j;
            p.y=i;
            floodFill(image,p,k+nobjects+1);
          }
        }
        ultimo = image.at<uchar>(i,j);
      }
    }
    if(tburaco)
      nburaco++;
    tburaco = false;
  } 

  cout << "Numero de bolhas total: " << nobjects << endl;
  cout << "Numero de bolhas sem buraco: " << nobjects - nburaco << endl;
  cout << "Numero de bolhas com buraco: " << nburaco << endl;
  imshow("image", image);
  imwrite("contarburacos.png", image);
  waitKey();
  return 0;
}