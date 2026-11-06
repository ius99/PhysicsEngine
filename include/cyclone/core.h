#ifndef CYCLONE_CORE_H
#define CYCLONE_CORE_H

#include "cyclone/precision.h"
#include <math.h>




namespace cyclone {

    class Vector3 {
        public: 
        real x; 
        real y; 
        real z; 

        private:
        real pad; 

        public: 
        Vector3() : x(0), y(0), z(0) {} //origin 

        Vector3(real x, real y, real z) : x(x), y(y), z(z) {} //parameterized constructor

        void invert() {} //returns inversion of the vector 


        real magnitude() {} //returns the magnitude of the vector 

        Vector3 normalize() {}; //returns the direction of the vector 



    }


}


#endif 