#include <iostream>
#include <iomanip>
#include "tgen_math.h"

int main(int argc, char ** const argv) {
	std::cout << "TGen Math (debug binary: " << std::boolalpha << TGen::isMathDebug() << ")" << std::endl;
	
	// 1.
	std::cout << "1. Matrix3x3::Identity" << std::endl;
	std::cout << std::string(TGen::Matrix3x3::Identity) << std::endl;

	// 2.
	std::cout << "2. Matrix4x4::Identity" << std::endl;
	std::cout << std::string(TGen::Matrix4x4::Identity) << std::endl;
	
	// 3.
	std::cout << "3. Matrix4x4::Identity * Matrix4x4::Identity" << std::endl;
	std::cout << std::string(TGen::Matrix4x4::Identity * TGen::Matrix4x4::Identity) << std::endl;
	
	// 4.
	std::cout << "4. Matrix3x3::Identity * Matrix4x4::Identity" << std::endl;
	std::cout << std::string(TGen::Matrix3x3::Identity * TGen::Matrix4x4::Identity) << std::endl;
	
	// 5.
	std::cout << "5. Matrix4x4::Identity * Matrix3x3::Identity" << std::endl;
	std::cout << std::string(TGen::Matrix4x4::Identity * TGen::Matrix3x3::Identity) << std::endl;
	
	// 6.
	std::cout << "6. PI radians = " << TGen::Degree(TGen::Radian(TGen::PI)).angle << " degrees " << std::endl;
	
	// 7.
	std::cout << "7. 180 degrees = " << TGen::Radian(TGen::Degree(180.0)).angle << " radians " << std::endl;
		
	// 8.
	std::cout << "8. PI = " << TGen::PI << std::endl;
	
	// 9.
	std::cout << "9. sizeof(scalar) = " << sizeof(scalar) << std::endl;
	
	// 8.
	std::cout << "8. PI =  " << TGen::Sine(TGen::PI) << std::endl;
	
	return EXIT_SUCCESS;
}
