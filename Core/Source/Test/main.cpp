#include <iostream>
#include <iomanip>
#include "tgen_core.h"

class MyClass {
public:
	MyClass(int num) : num(num) {std::cout << "ctor " << num << std::endl; }
	~MyClass() {std::cout << "dtor " << num << std::endl; }
	
	int num;
};

TGen::auto_ptr<MyClass> hey() {
	return TGen::auto_ptr<MyClass>(new MyClass(123));
}



int main(int argc, char ** const argv) {
	std::cout << "TGen Core (debug binary: " << std::boolalpha << TGen::isCoreDebug() << ")" << std::endl;
	
	// 1.
	if (TGen::TGenDataType<float>().type == TGen::TypeFloat)
		std::cout << "1. OK" << std::endl;
	
	// 2.
	std::cout << "2. " << sizeof(float) << " == " << TGen::FormatTypeSize(TGen::TGenDataType<float>().type) << std::endl;
	std::cout << "   " << sizeof(long long) << " == " << TGen::FormatTypeSize(TGen::TGenDataType<long long>().type) << std::endl;
	std::cout << "   " << sizeof(int) << " == " << TGen::FormatTypeSize(TGen::TGenDataType<int>().type) << std::endl;
	std::cout << "   " << sizeof(short) << " == " << TGen::FormatTypeSize(TGen::TGenDataType<short>().type) << std::endl;
	std::cout << "   " << sizeof(char) << " == " << TGen::FormatTypeSize(TGen::TGenDataType<char>().type) << std::endl;
	std::cout << "   " << sizeof(bool) << " == " << TGen::FormatTypeSize(TGen::TGenDataType<bool>().type) << std::endl;

	// 3.
	#ifdef _PLATFORM_WINDOWS
	std::cout << "3. Windows" << std::endl;
	#elif defined(_PLATFORM_OSX)
	std::cout << "3. OS X" << std::endl;
	#else
	std::cout << "3. Unknown" << std::endl;
	#endif
	
	// 4.
	try {
		throw TGen::RuntimeException("Core", "working");
	}
	catch (const std::exception & e) {
		std::cout << "4. exception: " << e.what() << std::endl;
	}

	// 5.
	std::cout << "5. lexical_cast: " << TGen::lexical_cast<int>("12345") + TGen::lexical_cast<int>("54321") << " == " << 12345 + 54321 << std::endl;
	
	// 6.
	std::cout << "6. " << TGen::lexical_cast<bool>("false") << " == false, " << TGen::lexical_cast<bool>("true") << " == true, " << TGen::lexical_cast<std::string>(false) << " == false, " << TGen::lexical_cast<std::string>(true) << " == true" << std::endl;

	// 7.
	try {
		std::cout << TGen::lexical_cast<int>("hejsan") << std::endl;
	}
	catch (const std::exception & e) {
		std::cout << "7. bad_lexical_cast: " << e.what() << std::endl;
	}
	
	// 8.
	TGen::Time now = TGen::Time::Now();
	std::cout << "8. now: " << std::fixed << double(now) << " ";
	TGen::Sleep(TGen::Time(1.0));
	TGen::Time really = TGen::Time::Now();
	std::cout << "sleep diff: " << double(really - now) << std::endl;
	
	// 9.
	TGen::PropertyTree props;
	props.addNode(TGen::PropertyTree("game"));
	props.getNode("game").addNode(TGen::PropertyTree("env"));
	props.getNode("game").getNode("env").addProperty(TGen::PropertyTree::Property("width", "1024"));
	std::cout << "9. 1024 = " << props.getProperty("game/env/width").second << std::endl;
	
	// 10.
	char code[] = "game {env {width 2048}\nstuff {name peter}}";
	TGen::PropertyTreeParser parser;
	std::cout << "10. 2048 = " << parser.parse(code).getProperty("game/env/width").second << std::endl;
	
	// 11.
	std::cout << "11. " << TGen::prependPath("/textures/coolman.png", "/textures/") << std::endl;
	std::cout << "    " << TGen::prependPath("coolman.png", "/textures/") << std::endl;
	std::cout << "    " << TGen::prependPath("/coolman.png", "/textures/") << std::endl;
	
	
	// 12.
	std::cout << "12." << std::endl;
	{
		TGen::auto_ptr<MyClass> pekare = new MyClass(1);
		TGen::auto_ptr<MyClass> pekare2 = pekare;
		pekare2 = new MyClass(2);
		pekare2 = pekare2;
		TGen::auto_ptr<MyClass> blah(pekare);
		std::cerr << "bo" << std::endl;

	}
	
	std::cout << "13." << std::endl;
	TGen::auto_ptr<MyClass> blah = hey();
	
	#ifdef _PLATFORM_WINDOWS
	int hej;
	std::cin >> hej;
	#endif
	

	return EXIT_SUCCESS;
}
