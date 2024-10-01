sudo apt-get install libx11-dev
git clone https://github.com/glfw/glfw.git
cd glfw
mkdir build
cd build
cmake ..
make
sudo make install
sudo ldconfig