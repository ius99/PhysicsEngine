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

        //operator overloading 

        //scalar multiplication
        Vector3 operator*=(const real value) { //returns the same vector scaled 
            this->x = x*value; 
            this->y = y*value; 
            this->z = z*value; 
            return (*this); 
        }

        Vector3 operator*(const real value) { 
            return Vector3(x*value, y*value, z*value); 
        }

        void operator+=(const Vector3& other) {
            x += other.x; 
            y += other.y; 
            z += other.z; 
        }



    



        //memeber functions
        void invert() {} //returns inversion of the vector 


        real magnitude() {
            return real_sqrt(x*x + y*y + z*z); 
        } //returns the magnitude of the vector 

        Vector3 normalize() {
            //1/magnitude * vector 
            real magnitude = cyclone::Vector3::magnitude(); 
            if(magnitude > 0 ) {
                (*this) *= ((real)1/magnitude); 
            }


        }; //returns the direction of the vector 

        real squaredMagnitude() {
            return (x*x + y*y + z*z); //because sqrt can be slow on some SLOW machines
        }



    }; 


}


#endif 