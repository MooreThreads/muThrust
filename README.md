# muThrust简介
muThrust 是摩尔线程面向 MUSA 生态提供的 C++ 并行编程库，提供与标准模板库风格一致的并行算法、迭代器、函数对象、容器适配接口及执行策略，可用于排序、规约、扫描、变换、重排、集合操作、内存管理等常见 GPU 加速场景。muThrust 基于开源 Thrust 1.17 版本进行 MUSA 适配与工程化维护，在保持原有 API 组织方式与使用习惯的基础上，为 C++ 并行程序提供可直接面向 MUSA 平台部署的高层算法接口，并可与 muAlg 配套使用。

# 项目依赖
使用 muThrust 需要安装摩尔线程 GPU 驱动及 MUSA SDK，并建议同时安装 [muAlg](https://github.com/MooreThreads/muAlg) 以获得完整的底层算法支撑。

# 安装方式
为了方便集成和部署，仓库提供安装脚本。典型用法如下：
```bash
# 默认安装到 /usr/local/musa
./mt_build.sh -i

# 安装到指定目录，例如 /tmp
./mt_build.sh -i -d /tmp

# 从安装目录卸载，不指定 -d 时默认从 /usr/local/musa 卸载
./mt_build.sh -u
```

# 开发者指南
muThrust 基于上游 Thrust 1.17 版本维护。关于构建方法、测试流程、平台约束和已知限制，请参考仓库内开发文档与测试说明。
