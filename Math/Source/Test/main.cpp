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
	std::cout << "12. (1, 0, 0) == " << std::string(TGen::Matrix4x4::Rotation(TGen::Vector3(0.0f, 0.0f, 1.0f), TGen::Degree(-90.0f)) * TGen::Vector3(0.0f, 1.0f, 0.0f)) << std::endl;
	std::cout << "    (0, 1, 0) == " << std::string(TGen::Matrix4x4::Rotation(TGen::Vector3(1.0f, 0.0f, 0.0f), TGen::Degree(-90.0f)) * TGen::Vector3(0.0f, 0.0f, 1.0f)) << std::endl;
	std::cout << "    (0, 0, 1) == " << std::string(TGen::Matrix4x4::Rotation(TGen::Vector3(0.0f, 1.0f, 0.0f), TGen::Degree(-90.0f)) * TGen::Vector3(1.0f, 0.0f, 0.0f)) << std::endl;
	std::cout << "    (1, 0, 0) == " << std::string(TGen::Matrix4x4::RotationY(TGen::Degree(90.0f)) * TGen::Vector3(0.0f, 0.0f, 1.0f)) << std::endl;
	
	// 13.
	std::cout << "13. identity == " << std::endl << std::string(TGen::Matrix4x4::LookInDirection(TGen::Vector3(0.0f, 0.0f, 1.0f), TGen::Vector3(0.0f, 1.0f, 0.0f))) << std::endl;
	std::cout << "    identity -X, Y, -Z == " << std::endl << std::string(TGen::Matrix4x4::LookInDirection(TGen::Vector3(0.0f, 0.0f, -1.0f), TGen::Vector3(0.0, 1.0f, 0.0f))) << std::endl;
						
	// 14.
	std::cout << "14. I^-1 == " << std::endl << std::string(TGen::Matrix4x4::Identity.invert()) << std::endl;
		
	// 15.
	TGen::Vector3 orient(0.0f, 0.0f, 1.0f);
	
	orient = TGen::Matrix4x4(TGen::Quaternion4::Rotation(TGen::Vector3(0.0f, 1.0f, 0.0f), TGen::Radian(TGen::PI))) * orient;
	
	TGen::Quaternion4 kord = orient; //orient * TGen::Vector3(0.0f, 0.0f, 1.0f) * -orient;
	
	std::cout << "15. " << std::string(TGen::Vector3(kord)) << std::endl << std::endl;
	
	// 16.
	TGen::Vector3 vectors[3];
	vectors[0] = TGen::Vector3(10.0f, 0.0f, 0.0f);
	vectors[1] = TGen::Vector3(10.0f, 5.0f, 10.0f);
	vectors[2] = TGen::Vector3(10.0f, -5.0f, 10.0f);
	
	TGen::Plane3 plane(vectors[0], vectors[1], vectors[2]);
	std::cout << "16. " << std::string(plane.normal) << " " << plane.distance << std::endl << std::endl;
	
	
	// 17.
	std::cout << "17. " << std::endl;
	
	TGen::CoordSet3 box1, box2;
	box1.addCoord(TGen::Vector3(-1.0f, -1.0f, 0.0f));
	box1.addCoord(TGen::Vector3( 1.0f, -1.0f, 0.0f));
	box1.addCoord(TGen::Vector3( 1.0f,  1.0f, 0.0f));
	box1.addCoord(TGen::Vector3(-1.0f,  1.0f, 0.0f));
	
	box2.addCoord(TGen::Vector3(-1.0f, -1.0f, 0.0f));
	box2.addCoord(TGen::Vector3( 1.0f, -1.0f, 0.0f));
	box2.addCoord(TGen::Vector3( 1.0f,  1.0f, 0.0f));
	box2.addCoord(TGen::Vector3(-1.0f,  1.0f, 0.0f));
	
	box2 *= TGen::Matrix4x4::Translation(TGen::Vector2::Identity * 2);
	
	std::cout << std::string(box1) << std::endl;
	std::cout << std::string(box2) << std::endl;
	
	#ifdef _PLATFORM_WINDOWS
	int hej;
	std::cin >> hej;
	#endif

	return EXIT_SUCCESS;
}
