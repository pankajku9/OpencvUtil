all:
	g++ --std=c++17 -Wall -Wextra -g3 OpencvUtil.cpp `pkg-config --libs opencv` -o ImageUtil.exe

lib:
	g++ --std=c++17 -Wall -Wextra -DLIB_MODE -fPIC -shared OpencvUtil.cpp `pkg-config --libs opencv` -o libopencvutil