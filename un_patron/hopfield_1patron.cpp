//MODELO DE HOPFIELD PARA UNA RED NEURONAL
#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <fstream>
#include <string>
#include <sys/stat.h> //para crear la carpeta

using namespace std;

int main(){
    const int N=128;
    const int pasos=150;
    const int pasos_medida=2;
    const int TAM_MAX=pasos/pasos_medida; 
    int i,j,k,l;
    auto tiempo_inicio_total = chrono::high_resolution_clock::now();

    //crear la carpeta un_patron (0777 da permisos de lectura/escritura)
    mkdir("un_patron", 0777);

    //genardor de numeros aleatorios
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis_0_o_1(0, 1);
    uniform_int_distribution<> dis_pos(0, N-1); 
    uniform_real_distribution<> dis_prob(0.0, 1.0);

    //genero un array de temperaturas
    double temperaturas[14];
    temperaturas[0]=0.0001;
    for (int i=1; i<14; i++){
        temperaturas[i]=i*0.002;
    }

    //matriz xi (que es la de la imagen) y calculo a
    ifstream imagen1("im1-boo.txt");
    int s[N][N], xi[N][N];
    double a=0.0;
    for (i=0; i<N; i++){
        for (j= 0; j<N; j++){
            imagen1 >>xi[i][j];
            a+=xi[i][j];
        }
    }
    a=a/((double)(N*N));
    imagen1.close();

    //calculo umbral de disparo theta_ij
    double theta[N][N]={0.0};
    double suma_w;

    double d_xi[N][N]={0.0}; //para no tener que repetir el calculo de xi[i][j]-a tantas veces luego
    for(i=0; i<N; i++){
        for(j=0; j<N; j++){
            d_xi[i][j] =xi[i][j]-a;
        }
    }
    for (i=0; i<N; i++){
        for (j=0; j<N; j++){
            suma_w=0.0;
            for (k=0; k<N; k++){
                for (l=0; l<N; l++){
                    if (i==k && j==l) continue; 
                    suma_w += d_xi[i][j]*d_xi[k][l];
                }
            }
            theta[i][j]=0.5*suma_w/(N*N);
        }
    }


    double sumatoria_H;
    double deltaH;

    for (double T :temperaturas){

        string nombre_m = "un_patron/m_T_" + to_string(T) + ".txt";
        string nombre_s = "un_patron/s_historial_T_" + to_string(T) + ".txt";

        ofstream archivo_m(nombre_m);
        ofstream archivo_s(nombre_s);

        //si quiero que al inicio la matriz s sea aleatorio descomentar esto y comentar lo de abajo
        /* for (i=0; i<N; i++){
            for (j=0; j<N; j++){
                s[i][j] = dis_0_o_1(gen);
            }
        } */

        //con deformacion inicial
        //inicializo matriz s con patrón inicial
        for (i=0; i<N; i++){
            for (j= 0; j<N; j++){
                s[i][j]=xi[i][j];
            }
        }
        
        double prob_deformacion = 0.3; //deformacion del 30% de la imagen inicial

        for (i=0; i<N; i++){
            for (j=0;j<N; j++){
                //genero num aleatorio entre 0 y 1, si es menor a la prob de deformación cambio el pixel 
                if (dis_prob(gen)<prob_deformacion){
                    s[i][j] =1-xi[i][j];
                }else{
                    s[i][j] =xi[i][j];
                }
            }
        }

        for (i=0; i<pasos; i++){
            for ( j=0; j<N*N;j++){
                int n=dis_pos(gen);
                int m=dis_pos(gen);
                
                sumatoria_H=0.0;
                for (k=0; k<N; k++){
                    for (l=0; l<N; l++){
                        if (n==k && m==l) continue; 
                        sumatoria_H +=d_xi[k][l]*s[k][l];
                    }
                }
                deltaH=(1-2*s[n][m])*(-d_xi[n][m]*sumatoria_H/(N*N)+theta[n][m]);

                if (deltaH <=0){
                    s[n][m] =1-s[n][m];
                } else{
                    double p=exp(-deltaH/T);
                    if (dis_prob(gen)<p){
                        s[n][m] =1-s[n][m];
                    }
                }
            }

            //calcular solapamiento m
            double m_solap=0.0;
            for (int i2 = 0; i2 < N; i2++){
                for (int j2 = 0; j2 < N; j2++){
                    m_solap += (xi[i2][j2] -a)*(s[i2][j2]-a);
                }
            }
            m_solap /= (N*N *a*(1.0-a));

            //guardamos en fichero
            archivo_m << i << "\t" << m_solap << endl;

            for (int x=0; x<N; x++){
                for (int y=0; y<N; y++){
                    archivo_s << s[x][y] << " ";
                }
                archivo_s << "\n";
            }
            
        }
        archivo_m.close();
        archivo_s.close();
        cout << "Finalizado para T=" << T << endl;
    }

    auto tiempo_fin_total= chrono::high_resolution_clock::now();
    chrono::duration<double> duracion_total = tiempo_fin_total-tiempo_inicio_total;
    
    cout << "Cálculos hechos en " << duracion_total.count() /60.0 <<" minutos" << endl;

    return 0;
}