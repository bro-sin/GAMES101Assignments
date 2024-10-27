# 环境配置
> 参考 https://games-cn.org/forums/topic/graphics-intro-hw0 中的要求进行配置

## conan 2
为了便于全局使用，我使用系统包管理器安装的`conan`，而不是官方推荐的使用`pip`安装：
```bash
sudo dnf install conan
```

## 依赖安装和编译

在`hw0`文件夹下，创建[`conanfile.txt`](./conanfile.txt),用于安装依赖
> 目前的依赖系统是有的，不用安装也能正常编译，这里写依赖文件是为了以后项目增添依赖了能够方便编译
```bash
conan install . --output-folder=build --build=missing 
```
使用下面命令进行编译
```bash
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
```