# 移植步骤
## 1 仓库准备
下载thrust 1.12.1 的release版本。本项目初始基于thrust 1.12.1版本。

## 2 musify一键迁移
musify迁移工具的使用教程详见[博客](https://blog.mthreads.com/blog/musa/2024-05-28-%E4%BD%BF%E7%94%A8musify%E5%AF%B9%E4%BB%A3%E7%A0%81%E8%BF%9B%E8%A1%8C%E5%B9%B3%E5%8F%B0%E8%BF%81%E7%A7%BB/)。在项目根目录下执行命令, 即可完成迁移。
```
/usr/local/musa/tools/musify-text --inplace -- `find ./ -name '*.cu' -o -name '*.cuh' -o -name '*.cpp' -o -name '*.h'  -o -name '*.inl'`
```

## 3 cmake改造
MUSA cmake的使用教程详见[博客](https://blog.mthreads.com/blog/musa/2024-05-20-%E4%BD%BF%E7%94%A8cmake%E6%9E%84%E5%BB%BAMUSA%E5%B7%A5%E7%A8%8B/)。
* CMakeLists.txt:94, 替换成cmake/ThrustMusaConfig.cmake。添加新文件`cmake/ThrustMusaConfig.cmake`。用于做MUSA camke的配置。
* `cmake/ThrustHeaderTesting.cmake`, `examples/CMakeLists.txt`, `examples/cmake/add_subdir/CMakeLists.txt`, `testing/CMakeLists.txt`, `testing/unittest/CMakeLists.txt`,等文件中将add_executable改成musa_add_executable, 将add_library改成musa_add_library，为了让代码能使用mcc编译器编译。
* cmake/ThrustHeaderTesting.cmake:24 目录改成musa
* testing/CMakeLists.txt:155,158, muThrust/examples/CMakeLists.txt:152, 暂时注释不支持的case。

## 4 其他
这里记录一些非普适也非所有项目必要的改动。
* 更改submodule的仓库路径到../muAlg.git。
* 添加muThrust库的安装脚本`mt_build.sh`。
* 添加新的`README.md`和`DeveloperGuide.md`指引文件。
* 为修改的代码文件添加版权许可说明。
* 增加module_version目录，目的是给muThrust库增加版本查询功能。使用见`CMakeLists.txt:27`。

## 5 GPU适配改动
* 复制目录thrust/system/cuda到thrust/system/musa，然后还原thrust/system/cuda目录
* 移动目录testing/unittest/cuda到testing/unittest/musa。
* thrust/system/musa/detail/core/util.h:56-91 增加MTGPU相关架构支持。
* thrust/system/musa/detail/adjacent_difference.h, copy_if.h, merge.h, parallel.h, partition.h, reduce_by_key.h, reduce.h, scan_by_key.h, set_operations.h, sort.h, unique_by_key.h, unique.h等文件中定义架构相关的struct Tuning，开发者可以进一步调整到最佳性能。





