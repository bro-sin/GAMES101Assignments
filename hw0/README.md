# 环境配置
> 参考 https://games-cn.org/forums/topic/graphics-intro-hw0 中的要求进行配置

## conan 2
为了便于全局使用，我使用系统包管理器安装的`conan`，而不是官方推荐的使用`pip`安装：

### Fedora
```sh
sudo dnf install conan
```

### MacOS
```sh
brew install conan cmake
```

## 依赖安装和编译

在`hw0`文件夹下，创建[`conanfile.txt`](./conanfile.txt),用于安装依赖
> 目前的依赖Fedora系统是有的，不用安装也能正常编译，这里写依赖文件是为了以后项目增添依赖了能够方便编译，还有就是MacOS中安装和使用该依赖，让各个系统可以统一进行编译
```sh
conan install . --output-folder=build --build=missing 
```
启用`conan`的环境：
```sh
cd build && source conanbuild.sh
```
依次输入下面命令进行编译

调用`conan`的工具链生成`CMakeFiles`

```sh
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
```

编译

```sh
cmake --build .
```

> `eigen`库以及`cmake`版本更新导致原本的`CMakeLists.txt`不能成功编译，折腾了太久；按照[`Eigen3.4.90`版本的官方文档说明](https://eigen.tuxfamily.org/dox/TopicCMakeGuide.html)修改后即可正常编译。