#include <iostream>
#include <iomanip>
#include "unique_ptr.hpp"
 
struct Vec3
{
    int x, y, z;
    // following constructor is no longer needed since C++20
    Vec3(int x = 0, int y = 0, int z = 0) noexcept : x(x), y(y), z(z) { }
    friend std::ostream& operator<<(std::ostream& os, const Vec3& v)
    {
        return os << '{' << "x:" << v.x << " y:" << v.y << " z:" << v.z  << '}';
    }
};
 
int main()
{
    // Use the default constructor.
    mini_stl::unique_ptr<Vec3> v1 = mini_stl::make_unique<Vec3>();
    // Use the constructor that matches these arguments
    mini_stl::unique_ptr<Vec3> v2 = mini_stl::make_unique<Vec3>(0,1,2);
    // Create a unique_ptr to an array of 5 elements
    mini_stl::unique_ptr<Vec3[]> v3 = mini_stl::make_unique<Vec3[]>(5);
 
    std::cout << "make_unique<Vec3>():      " << *v1 << '\n'
              << "make_unique<Vec3>(0,1,2): " << *v2 << '\n'
              << "make_unique<Vec3[]>(5):   ";
    for (int i = 0; i < 5; i++) {
        std::cout << std::setw(i?27:0) << v3[i] << '\n';
    }
}

/*
Output:

make_unique<Vec3>():      {x:0 y:0 z:0}
make_unique<Vec3>(0,1,2): {x:0 y:1 z:2}
make_unique<Vec3[]>(5):   {x:0 y:0 z:0}
                          {x:0 y:0 z:0}
                          {x:0 y:0 z:0}
                          {x:0 y:0 z:0}
                          {x:0 y:0 z:0}
*/
