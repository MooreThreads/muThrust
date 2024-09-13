# muThrust简介
muThrust 库是一个C++并行编程库，类似于 C++ 标准头文件库中的算法和数据结构库。muThrust库提供了与标准库基本一致的高级接口，让c++开发人员容易编写c++代码来使用GPU加速如排序，规约，前缀和等基础常用算法。muThrust是开源库thrust的MUSA移植项目，这个项目的目的主要有两点。第一，在GPU加速领域的生态中，有大量的项目使用到了thrust库，为了让这些项目能运行在摩尔线程的GPU上，需要进行MUSA移植，muThrust项目便提供了移植好的可以直接使用的thrust库。另一个目的是，通过这个项目，给广大开发者提供MUSA移植的实践案例，供大家参考。
# 项目依赖
确保环境中使用摩尔线程GPU，并请访问官网下载和安装[MUSA SDK](https://developer.mthreads.com/sdk/download/musa)。另外muThrust的使用还需要一同安装好[muAlg](https://github.com/MooreThreads/muAlg)库。
# 安装方式
为了使用方便，提供了安装脚本，使用方式如下：
```bash
  # 默认安装目录为/usr/local/musa
  ./mt_build.sh -i

  # 安装到指定目录，例如安装到/tmp
  ./mt_build.sh -i -d /tmp

  # 从安装目录中卸载，不加-d则默认从/usr/local/musa卸载
  ./mt_build.sh -u
```

# 开发者指南
由于thrust项目较大，muThrust的移植与针对性优化需要一个过程，欢迎广大开发者参与muThrust库的完善。[开发者指南](./DevelopGuide.md)文档中记录了初版提交的移植步骤以及一些说明。