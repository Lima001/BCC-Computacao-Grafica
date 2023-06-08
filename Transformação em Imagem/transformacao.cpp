#include <opencv2/opencv.hpp>
#include <math.h>

/*
    Definição para identificação dos diferentes métodos de interpolação implementados
*/
#define NN 0        // Nearest Neighbor
#define LI 1        // Linear
#define BL 2        // Bilinear

/*
    Parâmetros para configuração das matrizes de transformação. Modifique para
    alterar a transformação aplicada à imagem
*/
#define ANGLE 10*M_PI/180
#define SCALE_X 2
#define SCALE_Y 2
#define TRANSPOSE_X -128
#define TRANSPOSE_Y -128

// Define o fator de escala do grid de saída
#define OUT_BACKGROUND_SCALE 2

/*
    Implementação dos métodos de interpolação
*/
double linearInterpolation(double xl, double x0, double x1, double y0, double y1){
    double u = (xl-x0)/(x1-x0);
    return ((1-u)*y0 + u*y1);
}

double bilinearInterpolation(double xl, double yl, double x0, double x1, double y0, double y1, double fx0y0, double fx0y1, double fx1y0, double fx1y1){
    double r1 = linearInterpolation(xl, x0, x1, fx0y0, fx1y0);
    double r2 = linearInterpolation(xl, x0, x1, fx0y1, fx1y1);

    return linearInterpolation(yl, y0, y1, r1, r2);
}

double nearestNeighborInterpolation(int x){
    return std::floor(x);
}

/*
    Função que aplica uma transformação para produção de um output. A transformação é
    realizada da saída na entrada, logo, aplica-se a transformação inversa no output
    para obter os dados de input. Além disso, utiliza-se um método (por padrão, interpolação bilinear) 
    para interpolação de dados
*/
void applyTransformation(cv::Mat* trasformation, cv::Mat* input, cv::Mat* output, int interpolation_method=2){
    double value, x0, x1, y0, y1, f00, f01, f10, f11;
    
    cv::Mat inv_transformation = trasformation->inv();
    cv::Mat pos(3, 1, CV_64FC1);

    // Iteração sobre as coordenadas do output
    for (int i=0; i<output->size().width; i++){
        for (int j=0; j<output->size().height; j++){
            
            // Matriz com as coordenadas (homogêneas) atuais
            pos.at<double>(0,0) = i;
            pos.at<double>(1,0) = j;
            pos.at<double>(2,0) = 1;

            // Aplicação da transformação
            pos = inv_transformation * pos;
            
            // Caso a posição transformada esteja fora do grid do output, ignorar e prosseguir para a próxima iteração
            if (pos.at<double>(0,0) > output->size().width || pos.at<double>(0,0) < 0 || pos.at<double>(1,0) > output->size().height || pos.at<double>(1,0) < 0)
                continue;

            /* 
                Se o método de interpolação escolhido for Nearest Neighbor, aplica-o
                para encontrar o valor do píxel na posição transformada com base
                nos valores de input
            */
            if (interpolation_method == NN){
                x0 = nearestNeighborInterpolation(pos.at<double>(0,0));
                y0 = nearestNeighborInterpolation(pos.at<double>(1,0));
                value = input->at<double>(x0,y0);
                output->at<double>(i,j) = value;
                continue;
            }

            // Calcula os pontos limites para interpolaçao linear/bilinear
            x1 = std::ceil(pos.at<double>(0,0));
            y1 = std::ceil(pos.at<double>(1,0));
            x0 = std::min(x1-1, std::floor(pos.at<double>(0,0)));
            y0 = std::min(y1-1, std::floor(pos.at<double>(1,0)));
            
            /* 
                Verifica se algum dos pontos limitantes externos estão fora do grid de saída. Se sim,
                utiliza-se o valor mais próximo do pixel analisado
            */
            if (x1 > input->size().width-1 || y1 > input->size().height-1){
                value = input->at<double>((int)(pos.at<double>(0,0)), (int)(pos.at<double>(1,0)));
                output->at<double>(i,j) = value;
                continue;
            }

            /*
                Obtenção dos valores dos pixels refentes aos pontos de limite
                para interpolação linear/bilienar
            */
            f00 = input->at<double>(x0,y0);
            f01 = input->at<double>(x0,y1);
            f10 = input->at<double>(x1,y0);
            f11 = input->at<double>(x1,y1);
            
            
            // Se foi escolhida a interpolação linear, aplica-a em relação ao eixo horizontal da imagem
            if (interpolation_method == LI){
                value = linearInterpolation(pos.at<double>(0,0), x0, x1, f00, f10);
            }
            // Caso contrário, aplica-se a interpolação bilinear
            else{
                value = bilinearInterpolation(pos.at<double>(0,0), pos.at<double>(1,0), x0, x1, y0, y1, f00, f01, f10, f11);
            }

            output->at<double>(i,j) = value;
        }
    }
}

/*
    Função que aplica uma transformação em uma entrada para obtenção da saída. Diferentemente
    do método anterior, a transformação é aplicada da entrada para a saída. Sendo assim, em
    muitos casos existão "buracos" na imagem, pois não foi possível obter todos os pontos
    de saída necessários. 

    Essa função serve para exemplificar o motivo pela qual aplica-se uma transformação inversa
    da saída para a entrada para produzir o resultado esperado
*/
void applyTransformationInOut(cv::Mat* trasformation, cv::Mat* input, cv::Mat* output){
    double value;
    cv::Mat pos(3, 1, CV_64FC1);

    for (int i=0; i<input->size().width; i++){
        for (int j=0; j<input->size().height; j++){
            
            pos.at<double>(0,0) = i;
            pos.at<double>(1,0) = j;
            pos.at<double>(2,0) = 1;

            pos = (*trasformation) * pos;

            if (pos.at<double>(0,0) > output->size().width || pos.at<double>(0,0) < 0 || pos.at<double>(1,0) > output->size().height || pos.at<double>(1,0) < 0)
                continue;

            value = input->at<double>(i,j);
            output->at<double>((int)(pos.at<double>(0,0)),(int)(pos.at<double>(1,0))) = value;
        }
    }
}

/* 
    Converte uma matriz de inteiros sem sinal 8 bits de canal único para
    uma matriz de double de canal único.

    Utiliza-se essa função para obter matrizes que possam ser trabalhadas
    na hora da aplicação de transformações
*/ 
cv::Mat convert8UITo64F(cv::Mat* image){
    cv::Mat output = cv::Mat(image->size().height, image->size().width, CV_64FC1);
    
    for (int i=0; i<output.size().width; i++)
        for (int j=0; j<output.size().height; j++)
            output.at<double>(i,j) = (double)(image->at<uchar>(i,j))/255.0;

    return output;
}

/*
    Faz a operação inversa da função anterior. Utilizada para poder gerar uma matriz
    que pode ser salva como uma imagem em escala de cinza
*/
cv::Mat convert64FTo8UI(cv::Mat* image){
    cv::Mat output = cv::Mat(image->size().height, image->size().width, 0);

    for (int i = 0; i < output.size().width; i++)
        for (int j = 0; j < output.size().height; j++)
            output.at<uchar>(i,j) = (uchar)std::round((image->at<double>(i,j)*255));
    
    return output;
}

int main(){

    /*
        Definição de matrizes de transformação. Aqui, cada transformação
        foi separada inicialmente em uma matriz individual, visando apenas
        facilitar a sua visualização
    */

    // Rotação 2D (Coordenadas homogêneas)
    cv::Mat Rotate(3, 3, CV_64FC1);
    
    Rotate.at<double>(0,0) = std::cos(ANGLE); 
    Rotate.at<double>(0,1) = -std::sin(ANGLE);
    Rotate.at<double>(0,2) = 1;

    Rotate.at<double>(1,0) = std::sin(ANGLE);
    Rotate.at<double>(1,1) = std::cos(ANGLE);
    Rotate.at<double>(1,2) = 1;

    Rotate.at<double>(2,0) = 0;
    Rotate.at<double>(2,1) = 0;
    Rotate.at<double>(2,2) = 1;

    // Escalonamento 2D (Coordenadas homogêneas)
    cv::Mat Scale(3, 3, CV_64FC1);

    Scale.at<double>(0,0) = SCALE_X; 
    Scale.at<double>(0,1) = 0;
    Scale.at<double>(0,2) = 1;

    Scale.at<double>(1,0) = 0;
    Scale.at<double>(1,1) = SCALE_Y;
    Scale.at<double>(1,2) = 1;

    Scale.at<double>(2,0) = 0;
    Scale.at<double>(2,1) = 0;
    Scale.at<double>(2,2) = 1;

    // Transposição 2D (Coordenadas homogêneas)
    cv::Mat Transpose(3, 3, CV_64FC1);
    
    Transpose.at<double>(0,0) = 1; 
    Transpose.at<double>(0,1) = 0;
    Transpose.at<double>(0,2) = TRANSPOSE_X;

    Transpose.at<double>(1,0) = 0;
    Transpose.at<double>(1,1) = 1;
    Transpose.at<double>(1,2) = TRANSPOSE_Y;

    Transpose.at<double>(2,0) = 0;
    Transpose.at<double>(2,1) = 0;
    Transpose.at<double>(2,2) = 1;

    /* 
        Criação de uma matriz de trasformação única.
        Modifique a matriz T para compor as transformações
    */
    cv::Mat T(3, 3, CV_64FC1);
    T =  Scale*Transpose.inv()*Rotate*Transpose;

    // Leitura de uma imagem (escala de cinza) de entrada e conversão para matriz de double
    cv::Mat input = cv::imread("input.jpg", cv::IMREAD_GRAYSCALE);
    input = convert8UITo64F(&input);

    // Ciração das matrizes que resultarão nas imagens de saída
    cv::Mat output1((int)(input.size().width*OUT_BACKGROUND_SCALE), (int)(input.size().height*OUT_BACKGROUND_SCALE), CV_64FC1);
    cv::Mat output2((int)(input.size().width*OUT_BACKGROUND_SCALE), (int)(input.size().height*OUT_BACKGROUND_SCALE), CV_64FC1);

    /* 
        Aplicação da trasnformação em ambos os sentidos (Output->Input e Input->Output)
        para produção de duas imagens diferentes
    */
    applyTransformation(&T, &input, &output1);
    applyTransformationInOut(&T, &input, &output2);

    // Salvar as imagens de saída
    cv::imwrite("output1.jpg", convert64FTo8UI(&output1));
    cv::imwrite("output2.jpg", convert64FTo8UI(&output2));
    
    return 0;
}