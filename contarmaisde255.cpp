#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image, mask;
  int width, height;
  int nobjects;
  
  CvPoint p;
  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  
  if(!image.data){
    cout << "imagem nao carregou corretamente\n";
    return(-1);
  }
  width=image.size().width;
  height=image.size().height;

  p.x=0;
  p.y=0;

  // busca objetos com buracos presentes
  nobjects=0;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 255){
		    // achou um objeto
        if(nobjects == 254)//Nao deixar mudar tudo para a cor 255
          nobjects++;

		    nobjects++;
		    p.x=j;
		    p.y=i;
		    floodFill(image,p,nobjects);
      }
    }
  }
  if(nobjects > 254)
    cout << "Numero de objetos: " << nobjects-1 << endl;
  else
    cout << "Numero de objetos: " << nobjects << endl;
  imshow("image", image);
  imwrite("labeling.png", image);
  waitKey();
  imwrite("contarmaisde255.png", image);
  return 0;
}
