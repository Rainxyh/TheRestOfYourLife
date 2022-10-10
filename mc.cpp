#include<math.h>
#include<stdlib.h>
#include<iostream>
#include "rand.h"

inline float pdf(vec::vec3 p){
    return 1/(4*M_PI);
}

int main(){
    float sum=0;
    int N=1000000;
    for(int i=0;i<N;i++){
        vec::vec3 d = random_in_unit_sphere();
        float cos=d.z()*d.z();
        sum+=cos/pdf(d);
    }
    std::cout<<sum/N<<std::endl;
    std::cout<<4./3.*M_PI<<std::endl;
}