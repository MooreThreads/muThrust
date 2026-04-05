#!/bin/bash

# 更新 CMake 文件中的 CUDA 引用为 MUSA

# 只处理注释和文档字符串中的 CUDA，保留变量名（如 CMAKE_CUDA_FLAGS）
find . -name "CMakeLists.txt" -o -name "*.cmake" | grep -v "build" | while read file; do
    # 备份原文件
    cp "$file" "$file.bak"

    # 替换注释中的 CUDA 为 MUSA
    # 注意：只替换注释和文档，不替换变量名
    sed -i 's/# CUDA /# MUSA /g' "$file"
    sed -i 's/# CUDA-/ MUSA-/g' "$file"
    sed -i 's/(CUDA /(MUSA /g' "$file"
    sed -i 's/testing\/cuda/testing\/musa/g' "$file"
    sed -i 's/examples\/cuda/examples\/musa/g' "$file"

    echo "Updated: $file"
done

echo "CMake CUDA references update completed"
