//MODELO DE HOPFIELD PARA UNA RED NEURONAL (3 patrones)
#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <fstream>
#include <string>
#include <sys/stat.h>

using namespace std;

int main(){
    const int N=128;
    const int pasos=150;
    const int pasos_medida=2;
    const int TAM_MAX=pasos/pasos_medida; 
    int i,j,k,l;
    auto tiempo_inicio_total = chrono::high_resolution_clock::now();

    mkdir("varios_patrones", 0777);

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

    //matriz xi y calculo a
    ifstream imagen1("im1-boo.txt");
    ifstream imagen2("im2-trifuerza.txt");
    ifstream imagen3("im3-luna.txt");
    int s[N][N], xi[3][N][N]; 
    double a[3] = {0.0, 0.0, 0.0};

    for (i=0; i<N; i++){
        for (j= 0; j<N; j++){
            imagen1 >>xi[0][i][j];
            imagen2 >>xi[1][i][j];
            imagen3 >>xi[2][i][j];
            a[0]+=xi[0][i][j];
            a[1]+=xi[1][i][j];
            a[2]+=xi[2][i][j];
        }
    }
    a[0] = a[0]/((double)(N*N));
    a[1] = a[1]/((double)(N*N));
    a[2] = a[2]/((double)(N*N));
    imagen1.close();
    imagen2.close();
    imagen3.close();

    //calculo umbral de disparo theta_ij
    double theta[N][N]={0.0};
    double suma_w;
    int mu;
    double d_xi[3][N][N]={0.0};
    for(mu=0; mu<3; mu++){
        for(i=0; i<N; i++){
            for(j=0; j<N; j++){
                d_xi[mu][i][j]=xi[mu][i][j]-a[mu];
            }
        }
    }
    for (i=0; i<N; i++){
        for (j=0; j<N; j++){
            suma_w=0.0;
            for (k=0; k<N; k++){
                for (l=0; l<N; l++){
                    if (i==k && j==l) continue; 
                    for(mu=0; mu<3; mu++){
                        suma_w += d_xi[mu][i][j]*d_xi[mu][k][l];
                    }
                }
            }
            theta[i][j]=0.5*suma_w/(N*N);
        }
    }

    double sumatoria_H;
    double deltaH;

    for (double T :temperaturas){

        string nombre_m = "varios_patrones/con_deformacion/m_T_" + to_string(T) + ".txt";
        string nombre_s = "varios_patrones/con_deformacion/s_historial_T_" + to_string(T) + ".txt";

        ofstream archivo_m(nombre_m);
        ofstream archivo_s(nombre_s);

        //si quiero que al inicio la matriz s sea aleatorio descomentar esto y comentar lo de abajo
        /* for (i=0; i<N; i++){
            for (j=0; j<N; j++){
                s[i][j] = dis_0_o_1(gen);
            }
        } */

        //con deformacion inicial
        //inicializo matriz s con patrón inicial, usando im1 como base
        for (i=0; i<N; i++){
            for (j= 0; j<N; j++){
                s[i][j]=xi[0][i][j];
            }
        }
        
        double prob_deformacion = 0.3; //deformacion del 30% de la imagen inicial

        for (i=0; i<N; i++){
            for (j=0;j<N; j++){
                //genero num aleatorio entre 0 y 1, si es menor a la prob de deformación cambio el pixel 
                if (dis_prob(gen)<prob_deformacion){
                    s[i][j] =1-xi[0][i][j];
                }else{
                    s[i][j] =xi[0][i][j];
                }
            }
        }

        for (i=0; i<pasos; i++){
            for ( j=0; j<N*N;j++){
                int n=dis_pos(gen);
                int m=dis_pos(gen);
                
                sumatoria_H=0.0;
                double suma_terminos=0.0;
                for(mu=0; mu<3; mu++){
                    sumatoria_H=0.0;
                    for (k=0; k<N; k++){
                        for (l=0; l<N; l++){
                            if (n==k && m==l) continue; 
                            sumatoria_H += d_xi[mu][k][l]*s[k][l];
                        }
                    }
                    suma_terminos += d_xi[mu][n][m]*sumatoria_H;
                }

                deltaH=(1-2*s[n][m])*(-suma_terminos/(N*N)+theta[n][m]);

                if (deltaH <=0){
                    s[n][m] =1-s[n][m];
                }else{
                    double p=exp(-deltaH/T);
                    if (dis_prob(gen)<p){
                        s[n][m] =1-s[n][m];
                    }
                }
            }

            //calcular solapamiento m
            double m_solap[3] = {0.0, 0.0, 0.0};
            for(mu=0; mu<3; mu++){
                for (int i2=0; i2<N; i2++){
                    for (int j2=0; j2<N; j2++){
                        m_solap[mu] += (xi[mu][i2][j2] -a[mu])*(s[i2][j2]-a[mu]);
                    }
                }
                m_solap[mu] /= (N*N *a[mu]*(1.0-a[mu]));
            }

            //guardamos en fichero
            archivo_m << i << "\t" << m_solap[0] << "\t" << m_solap[1] << "\t" << m_solap[2] << endl;

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