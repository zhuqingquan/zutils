# zutils
Gather some utils code. 

## 构建
```shell
# 初始化与更新子模块
git submodule update --init --recursive
# 构建
cmake -S cpp/ -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug --target all -j4
# 运行单元测试
cd build/bin/
./zutils_test
```