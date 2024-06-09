compile:
	cd build && cmake --build .

dependency:
	cd build && cmake ..

prepare:
	rm -rf build
	mkdir build

clean:
	rm -rf build