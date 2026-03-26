rm compile_commands.json
mkdir -p build
cd build
cmake ..
make
cd ..
ln -s build/compile_commands.json compile_commands.json
