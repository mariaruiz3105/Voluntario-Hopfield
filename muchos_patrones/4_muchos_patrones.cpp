//parte 4 código para ver cómo disminuyen el número de patrones recordados en función del número de patrones almacenados

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <fstream>
#include <string>
#include <sys/stat.h>

using namespace std;

int main(){
    const int N=20;
    const double T=0.0001;
    const int pasos=40;
    const int P_MAX=100;

    auto tiempo_inicio_total = chrono::high_resolution_clock::now();

    mkdir("muchos_patrones", 0777);
    ofstream archivo_final("muchos_patrones/almacenados_vs_recordados.txt");

    //genardor de numeros aleatorios
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis_0_o_1(0, 1);
    uniform_int_distribution<> dis_pos(0, N-1); 
    uniform_real_distribution<> dis_prob(0.0, 1.0);

    int i,j,k,l, mu, P, i2, j2;
    int s[N][N];
    int xi[P_MAX][N][N];
    double a[P_MAX];
    double d_xi[P_MAX][N][N];
    double theta[N][N];
    double suma_w;
    double sumatoria_H;
    double deltaH;


    //vamos aumentando el numero de patrones almacenados para ver cuaéntos se recuerdan
    for (P=1; P<=P_MAX; P++){
        
        //primero genero P patrones aleatorios y calculo a
        for (mu=0; mu<P; mu++){
            a[mu]=0.0;
            for (i=0; i<N; i++){
                for (j=0; j<N; j++){
                    xi[mu][i][j]=dis_0_o_1(gen);
                    a[mu]+=xi[mu][i][j];
                }
            }
            a[mu] /= (double)(N*N);
        }

        //calculo d_xi
        for(mu=0; mu<P; mu++){
            for (i=0; i<N; i++){
                for (j=0; j<N; j++){
                    d_xi[mu][i][j]=xi[mu][i][j]-a[mu];
                }
            }
        }

        //calculo umbral de disparo theta_ij
        for (i=0; i<N; i++){
            for (j=0; j<N; j++){
                suma_w = 0.0;
                for (k=0; k<N; k++){
                    for (l=0; l<N; l++){
                        if (i==k && j==l) continue; 
                        for (mu=0; mu<P; mu++){
                            suma_w += d_xi[mu][i][j] * d_xi[mu][k][l];
                        }
                    }
                }
                theta[i][j]=0.5*suma_w/(N*N);
            }
        }

        int patrones_recordados =0;

        //vamos patron por patrón para ver si la red recuerda cada uno
        for (int nu=0; nu<P; nu++){
            //el patrón inicial es el patrón nu que queremos comrpobar
            for (i=0; i<N; i++){
                for (j=0; j<N; j++){
                    s[i][j]=xi[nu][i][j];
                }
            }

            for (i= 0; i<pasos; i++){
                for (j=0; j<N*N; j++){
                    int n=dis_pos(gen);
                    int m=dis_pos(gen);
                    
                    double suma_terminos=0.0;
                    for (mu=0; mu<P; mu++){
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
            }

            //solapamiento final con el patrón nu
            double m_solap = 0.0;
            for (i2=0; i2<N; i2++){
                for (j2=0; j2<N; j2++){
                    m_solap += (xi[nu][i2][j2] -a[nu])*(s[i2][j2]-a[nu]);
                }
            }
            m_solap /= (N*N *a[nu]*(1.0-a[nu]));

            if (abs(m_solap)>0.75){
                patrones_recordados++;
            }
        }

        //guardamos en fichero
        archivo_final << P << "\t" << patrones_recordados << endl;
        cout << "P almacenados: " << P << " | P recordados: " << patrones_recordados << endl;
    }
    
    archivo_final.close();
    auto tiempo_fin_total = chrono::high_resolution_clock::now();
    chrono::duration<double> duracion_total = tiempo_fin_total - tiempo_inicio_total;
    
    cout << "\nFIN " << duracion_total.count() / 60.0 << " minutos." << endl;
    return 0;
}
