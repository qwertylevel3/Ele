#include "util.h"

double Util::evaluate(double k1, double k2, double k3, double k4, double r15, double t15, double r30, double t30, double r45, double t45, double r60, double t60)
{



    double value1=(t15==0?0:(r15/t15));
    double value2=(t30==0?0:(r30/t30));
    double value3=(t45==0?0:(r45/t45));
    double value4=(t60==0?0:(r60/t60));

    return k1*value1+k2*value2+k3*value3+k4*value4;
}
