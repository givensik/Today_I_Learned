#include <stdio.h>
double funx(double x, double a){
    return x*x - a;
}
double funxx(double a){
    return 2*a;
}
int main(){

    double x,a;//우리는 루트 a를 구할거임 (x = a일때 시작)
    scanf("%lf",&a);
    int i = 0;
    double b = a;
    while(i<100){
        // printf("i = %d\n",i++);
        // printf("funx = %f, funxx = %f\n",funx(a,a), funxx(a));
        a = a - funx(a,b)/funxx(a);//다음 a값 
        // printf("a = %f\n",a);
        if(a<=1){
            break;
        }
    }
    printf("sqrt(a) = %.3f\n",a);

    return 0;
}