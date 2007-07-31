#include <iostream>
#include <iomanip>
#include "tgen_math.h"
#include "tgen_core.h"

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
	
	// 10.
	std::cout << "10. cos(PI) =  " << std::fixed << TGen::Cos(TGen::Radian(TGen::PI)) << std::endl;
	
	// 11.
	TGen::Quaternion4 front(0.0f, 0.0f, 1.0f);
	//front *= TGen::Quaternion4::Rotation(TGen::Vector3(0.0f, 1.0f, 0.0f), TGen::Degree(90.0));
	TGen::Quaternion4 rot = TGen::Quaternion4::Rotation(TGen::Vector3(0.0f, 1.0f, 0.0f), TGen::Degree(90.0));
	TGen::Quaternion4 result = rot * front * -rot;
	std::cout << "11. (1, 0, 0) == " << std::string(TGen::Vector3(result)) << std::endl;
		
	// 12.
	TGen::Matrix4x4 matRot = TGen::Matrix4x4::Rotation(TGen::Vector3(1.0f, 0.0f, 0.0f), TGen::Degree(-90.0f));
	std::cout << "12. (0, 1, 0) ~= " << std::string(matRot * TGen::Vector3(0.0f, 0.0f, 1.0f)) << std::endl;
	
	#ifdef _PLATFORM_WINDOWS
	int hej;
	std::cin >> hej;
	#endif

	return EXIT_SUCCESS;
}
