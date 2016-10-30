#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define gamaL_slider_max 100
#define gamaH_slider_max 100
#define d_slider_max     100
#define c_slider_max     100

using namespace cv;
using namespace std;

Mat image, padded, imagegray, tmp, filtrada, complexImage, res;
Mat_<float> realInput, zeros;
vector<Mat> planos;

int dft_M, dft_N, gamaL_slider, gamaH_slider, d_slider, c_slider;
float c, d, gamaH, gamaL;

char key;

// troca os quadrantes da imagem da DFT
void deslocaDFT(Mat& image ){
  Mat tmp, A, B, C, D;

  // se a imagem tiver tamanho impar, recorta a regiao para
  // evitar cópias de tamanho desigual
  image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
  int cx = image.cols/2;
  int cy = image.rows/2;

  // reorganiza os quadrantes da transformada
  // A B   ->  D C
  // C D       B A
  A = image(Rect(0, 0, cx, cy));
  B = image(Rect(cx, 0, cx, cy));
  C = image(Rect(0, cy, cx, cy));
  D = image(Rect(cx, cy, cx, cy));

  // A <-> D
  A.copyTo(tmp);  D.copyTo(A);  tmp.copyTo(D);
  // C <-> B
  C.copyTo(tmp);  B.copyTo(C);  tmp.copyTo(B);
}
  
void Filtro(int, void*){
  int dft_M, dft_N;

  dft_M = tmp.size().height;
  dft_N = tmp.size().width;

  gamaL = gamaL_slider / 10.0;
  gamaH = gamaH_slider / 10.0;
  d     = d_slider / 10.0;
  c     = c_slider / 10.0;

  // prepara o filtro passa-baixas ideal
  for(int i=0; i<dft_M; i++){
    for(int j=0; j<dft_N; j++){
      tmp.at<float> (i,j) = (gamaH - gamaL) * 
      (1 - exp(-1 * c * (pow(i - dft_M/2, 2) +
      pow(j - dft_N/2, 2))/pow(d,2))) + gamaL;
    }
  }

  // cria a matriz com as componentes do filtro e junta
  // ambas em uma matriz multicanal complexa
  Mat comps[]= {tmp, tmp};
  merge(comps, 2, filtrada);
}

int main(int argc, char** argv){
 
  namedWindow("filtrada", CV_WINDOW_AUTOSIZE);
  createTrackbar("Valor gamaH",
                "filtrada",
                &gamaH_slider,
                gamaH_slider_max,
                Filtro);
  createTrackbar("Valor gamaL",
                "filtrada",
                &gamaL_slider,
                gamaL_slider_max,
                Filtro);
  createTrackbar("Valor d",
                "filtrada",
                &d_slider,
                d_slider_max,
                Filtro);
  createTrackbar("Valor c",
                "filtrada",
                &c_slider,
                c_slider_max,
                Filtro);

  // carregar imagem
  image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

  if(!image.data){
    cout << "Nao abriu: " << argv[1] << endl;
    cout << argv[0] << " imagem.jpg";
    exit(0);
  }

  // identifica os tamanhos otimos para
  // calculo do FFT
  dft_M = getOptimalDFTSize(image.rows);
  dft_N = getOptimalDFTSize(image.cols);
  
  // realiza o padding da imagem
  copyMakeBorder(image, padded, 0,
                 dft_M - image.rows, 0,
                 dft_N - image.cols,
                 BORDER_CONSTANT, Scalar::all(0));

  // parte imaginaria da matriz complexa (preenchida com zeros)
  zeros = Mat_<float>::zeros(padded.size());

  // prepara a matriz complexa para ser preenchida
  complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));

  // a função de transferência (filtro frequencial) deve ter o
  // mesmo tamanho e tipo da matriz complexa
  filtrada = complexImage.clone();

  // cria uma matriz temporária para criar as componentes real
  // e imaginaria do filtro ideal
  tmp = Mat(dft_M, dft_N, CV_32F);

  for(;;){
    imagegray = image.clone();
    imshow("original", imagegray);

    // realiza o padding da imagem
    copyMakeBorder(imagegray, padded, 0,
                   dft_M - image.rows, 0,
                   dft_N - image.cols,
                   BORDER_CONSTANT, Scalar::all(0));

    // limpa o array de matrizes que vao compor a
    // imagem complexa
    planos.clear();
    // cria a compoente real
    realInput = Mat_<float>(padded);

    //calculando o logaritmo da imagem
    log(realInput, realInput);

    // insere as duas componentes no array de matrizes
    planos.push_back(realInput);
    planos.push_back(zeros);

    // combina o array de matrizes em uma unica
    // componente complexa
    merge(planos, complexImage);

    // calcula o dft
    dft(complexImage, complexImage);

    // realiza a troca de quadrantes
    deslocaDFT(complexImage);

    // aplica o filtro frequencial
    mulSpectrums(complexImage,filtrada,complexImage,0);

    // troca novamente os quadrantes
    deslocaDFT(complexImage);

    // calcula a DFT inversa
    idft(complexImage, complexImage, DFT_SCALE);

    // limpa o array de planos
    planos.clear();

    // separa as partes real e imaginaria da
    // imagem filtrada
    split(complexImage, planos);

    //calcular exponencial
    exp(planos[0], planos[0]);

    // normaliza a parte real para exibicao
    normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
    imshow("filtrada", planos[0]);

    key = (char) waitKey(10);
    if( key == 27 )
      break;
  }
    planos[0].convertTo(res, CV_8UC1, 255.0);
    imwrite("ImagemOriginal.png", imagegray);
    imwrite("filtrada.png", res);

  return 0;
}

