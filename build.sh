arg=$1
gcc_path="D:/App/env/tools/gnu_gcc/arm_gcc/mingw/bin"


if [[ $1 == "clean" ]];then
    echo make clean
    make clean
else
    echo make all
    make GCC_PATH=$gcc_path
fi