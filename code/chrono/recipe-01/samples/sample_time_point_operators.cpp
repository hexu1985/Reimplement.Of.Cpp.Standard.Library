// time_point operators
#include <iostream>
#include "chrono.hpp"

int main ()
{
    
    using namespace Hx::chrono;

    system_clock::time_point tp,tp2;                // epoch value
    system_clock::duration dtn (duration<int>(1));  // 1 second

                        //  tp     tp2    dtn
                        //  ---    ---    ---
    tp += dtn;          //  e+1s   e      1s
    tp2 -= dtn;         //  e+1s   e-1s   1s
    tp2 = tp + dtn;     //  e+1s   e+2s   1s
    tp = dtn + tp2;     //  e+3s   e+2s   1s
    tp2 = tp2 - dtn;    //  e+3s   e+1s   1s
    dtn = tp - tp2;     //  e+3s   e+1s   2s

    std::cout << std::boolalpha;
    std::cout << "tp == tp2: " << (tp==tp2) << std::endl;
    std::cout << "tp > tp2: " << (tp>tp2) << std::endl;
    std::cout << "dtn: " << dtn.count() << std::endl;

    return 0;
}
