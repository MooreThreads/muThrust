#!/usr/bin/env python3
"""
CUDA 到 MUSA 重命名脚本

自动处理以下修改：
1. 命名空间：cuda_cub → musa_cub
2. Include 引用：thrust/system/cuda → thrust/system/musa
3. 命名空间别名：thrust::cuda → thrust::musa
4. 错误处理：cuda_category → musa_category
5. 文档注释：CUDA → MUSA
"""

import re
import os
import sys
from pathlib import Path
from typing import List, Tuple, Dict
from dataclasses import dataclass


@dataclass
class Replacement:
    """表示一次替换操作"""
    file_path: str
    line_number: int
    original: str
    replaced: str
    pattern_name: str


class CudaToMusaRenamer:
    """CUDA 到 MUSA 重命名器"""

    def __init__(self, dry_run: bool = False):
        self.dry_run = dry_run
        self.replacements: List[Replacement] = []

    def find_source_files(self, root_path: str) -> List[str]:
        """查找所有需要修改的源文件"""
        extensions = ['.h', '.cuh', '.cu', '.cpp', '.cxx', '.inl']
        exclude_dirs = {'build', 'build_mp_22', 'build_mp_31', '.git', 'musa_alias_backup'}

        source_files = []
        root = Path(root_path)

        for ext in extensions:
            for file_path in root.rglob(f'*{ext}'):
                # 排除构建目录和备份目录
                if any(exclude_dir in file_path.parts for exclude_dir in exclude_dirs):
                    continue
                source_files.append(str(file_path))

        return sorted(source_files)

    def apply_all_replacements(self, content: str, file_path: str) -> Tuple[str, List[Replacement]]:
        """应用所有替换规则"""
        replacements = []
        result = content

        # 按优先级应用替换规则
        patterns = [
            ('1. Include 路径', self._pattern_include_paths),
            ('2. 命名空间 cuda_cub', self._pattern_namespace_cuda_cub),
            ('3. 命名空间 thrust::cuda', self._pattern_namespace_thrust_cuda),
            ('4. cuda_category 函数', self._pattern_cuda_category),
            ('5. cuda_error_category 类', self._pattern_cuda_error_category),
            ('6. 文档字符串 CUDA', self._pattern_documentation_cuda),
        ]

        for pattern_name, pattern_func in patterns:
            result, pattern_replacements = pattern_func(result, file_path, pattern_name)
            replacements.extend(pattern_replacements)

        return result, replacements

    def _pattern_include_paths(self, content: str, file_path: str, pattern_name: str) -> Tuple[str, List[Replacement]]:
        """替换 include 路径"""
        replacements = []
        pattern = r'#include\s*<thrust/system/cuda/'

        def replace_include(match):
            original = match.group(0)
            replaced = original.replace('thrust/system/cuda/', 'thrust/system/musa/')

            replacements.append(Replacement(
                file_path=file_path,
                line_number=self._get_line_number(content, match.start()),
                original=original,
                replaced=replaced,
                pattern_name=pattern_name
            ))
            return replaced

        result = re.sub(pattern, replace_include, content)
        return result, replacements

    def _pattern_namespace_cuda_cub(self, content: str, file_path: str, pattern_name: str) -> Tuple[str, List[Replacement]]:
        """替换命名空间 cuda_cab/cub"""
        replacements = []

        # 匹配 namespace cuda_cab 或 namespace cuda_cub
        patterns = [
            (r'\bnamespace\s+cuda_cab\b', 'cuda_cab'),
            (r'\bnamespace\s+cuda_cub\b', 'cuda_cub'),
            (r'\bcuda_cab::', 'cuda_cab::'),
            (r'\bcuda_cub::', 'cuda_cub::'),
            (r'using\s+cuda_cab::', 'using cuda_cab::'),
            (r'using\s+cuda_cub::', 'using cuda_cub::'),
        ]

        result = content
        for pattern, original_text in patterns:
            def make_replacer(orig_text):
                def replacer(match):
                    replaced = match.group(0).replace('cuda_cab', 'musa_cub').replace('cuda_cub', 'musa_cub')
                    replacements.append(Replacement(
                        file_path=file_path,
                        line_number=self._get_line_number(content, match.start()),
                        original=match.group(0),
                        replaced=replaced,
                        pattern_name=pattern_name
                    ))
                    return replaced
                return replacer

            result = re.sub(pattern, make_replacer(original_text), result)

        return result, replacements

    def _pattern_namespace_thrust_cuda(self, content: str, file_path: str, pattern_name: str) -> Tuple[str, List[Replacement]]:
        """替换命名空间 thrust::cuda"""
        replacements = []

        # 匹配 thrust::cuda:: 但不匹配 thrust::cuda_cab/cub
        pattern = r'\bthrust::cuda::(?!cab|cub)'

        def replace_namespace(match):
            original = match.group(0)
            replaced = original.replace('thrust::cuda::', 'thrust::musa::')

            replacements.append(Replacement(
                file_path=file_path,
                line_number=self._get_line_number(content, match.start()),
                original=original,
                replaced=replaced,
                pattern_name=pattern_name
            ))
            return replaced

        result = re.sub(pattern, replace_namespace, content)
        return result, replacements

    def _pattern_cuda_category(self, content: str, file_path: str, pattern_name: str) -> Tuple[str, List[Replacement]]:
        """替换 cuda_category 函数调用"""
        replacements = []
        pattern = r'\bcuda_category\s*\('

        def replace_category(match):
            original = match.group(0)
            replaced = original.replace('cuda_category', 'musa_category')

            replacements.append(Replacement(
                file_path=file_path,
                line_number=self._get_line_number(content, match.start()),
                original=original,
                replaced=replaced,
                pattern_name=pattern_name
            ))
            return replaced

        result = re.sub(pattern, replace_category, content)
        return result, replacements

    def _pattern_cuda_error_category(self, content: str, file_path: str, pattern_name: str) -> Tuple[str, List[Replacement]]:
        """替换 cuda_error_category 类名"""
        replacements = []
        pattern = r'\bcuda_error_category\b'

        def replace_error_category(match):
            original = match.group(0)
            replaced = original.replace('cuda_error_category', 'musa_error_category')

            replacements.append(Replacement(
                file_path=file_path,
                line_number=self._get_line_number(content, match.start()),
                original=original,
                replaced=replaced,
                pattern_name=pattern_name
            ))
            return replaced

        result = re.sub(pattern, replace_error_category, content)
        return result, replacements

    def _pattern_documentation_cuda(self, content: str, file_path: str, pattern_name: str) -> Tuple[str, List[Replacement]]:
        """替换文档字符串中的 CUDA"""
        replacements = []

        # 只替换注释中的 CUDA，不替换代码中的
        patterns = [
            # Doxygen 注释
            (r'(/\*[\*!][\s\S]*?)\bCUDA\b([\s\S]*?\*/)', 'Doxygen block'),
            # 单行注释
            (r'(//.*?)\bCUDA\b', 'Single line comment'),
            # 文档字符串中的 "CUDA system"
            (r'"CUDA system"', 'String literal'),
            (r"'CUDA system'", 'String literal'),
        ]

        result = content
        for pattern, desc in patterns:
            def make_replacer(description):
                def replacer(match):
                    if len(match.groups()) >= 2:
                        # 多行注释模式
                        replaced = match.group(0).replace('CUDA', 'MUSA')
                    else:
                        # 单行或字符串模式
                        replaced = match.group(0).replace('CUDA', 'MUSA')

                    if replaced != match.group(0):
                        replacements.append(Replacement(
                            file_path=file_path,
                            line_number=self._get_line_number(content, match.start()),
                            original=match.group(0),
                            replaced=replaced,
                            pattern_name=f"{pattern_name} ({description})"
                        ))
                    return replaced
                return replacer

            result = re.sub(pattern, make_replacer(desc), result)

        return result, replacements

    def _get_line_number(self, content: str, pos: int) -> int:
        """获取位置对应的行号"""
        return content[:pos].count('\n') + 1

    def process_file(self, file_path: str) -> int:
        """处理单个文件"""
        try:
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                original_content = f.read()
        except Exception as e:
            print(f"Error reading {file_path}: {e}")
            return 0

        converted_content, replacements = self.apply_all_replacements(original_content, file_path)

        if replacements and not self.dry_run:
            # 备份原文件
            backup_path = f"{file_path}.bak"
            try:
                with open(backup_path, 'w', encoding='utf-8') as f:
                    f.write(original_content)
            except Exception as e:
                print(f"Warning: Could not backup {file_path}: {e}")

            # 写入修改后的内容
            try:
                with open(file_path, 'w', encoding='utf-8') as f:
                    f.write(converted_content)
                print(f"✓ {file_path} ({len(replacements)} replacements)")
            except Exception as e:
                print(f"Error writing {file_path}: {e}")
                return 0

        self.replacements.extend(replacements)
        return len(replacements)

    def process_directory(self, root_path: str) -> Dict[str, int]:
        """处理整个目录"""
        source_files = self.find_source_files(root_path)
        print(f"Found {len(source_files)} source files to process\n")

        stats = {}
        for file_path in source_files:
            count = self.process_file(file_path)
            if count > 0:
                stats[file_path] = count

        return stats

    def generate_report(self, output_path: str = None):
        """生成替换报告"""
        if not self.replacements:
            report = "# CUDA to MUSA Renaming Report\n\nNo replacements made.\n"
        else:
            report = f"# CUDA to MUSA Renaming Report\n\n"
            report += f"Total replacements: {len(self.replacements)}\n\n"

            # 按模式分组统计
            pattern_stats = {}
            for repl in self.replacements:
                if repl.pattern_name not in pattern_stats:
                    pattern_stats[repl.pattern_name] = []
                pattern_stats[repl.pattern_name].append(repl)

            report += "## Statistics by Pattern\n\n"
            for pattern_name, repls in sorted(pattern_stats.items()):
                report += f"### {pattern_name}\n\n"
                report += f"Count: {len(repls)}\n\n"

                # 显示前10个示例
                for repl in repls[:10]:
                    report += f"- **{repl.file_path}:{repl.line_number}**\n"
                    report += f"  - Original: `{repl.original}`\n"
                    report += f"  - Replaced: `{repl.replaced}`\n\n"

                if len(repls) > 10:
                    report += f"  ... and {len(repls) - 10} more\n\n"

        if output_path:
            try:
                with open(output_path, 'w', encoding='utf-8') as f:
                    f.write(report)
                print(f"\nReport saved to: {output_path}")
            except Exception as e:
                print(f"Error saving report: {e}")

        return report


def main():
    import argparse

    parser = argparse.ArgumentParser(
        description='CUDA to MUSA Renaming Script',
        formatter_class=argparse.RawDescriptionHelpFormatter
    )

    parser.add_argument('path', help='Root directory to process')
    parser.add_argument('--dry-run', action='store_true',
                       help='Preview changes without modifying files')
    parser.add_argument('--report', '-r', metavar='FILE',
                       help='Generate detailed report to file')

    args = parser.parse_args()

    path = Path(args.path)
    if not path.exists():
        print(f"Error: Path does not exist: {args.path}")
        sys.exit(1)

    renamer = CudaToMusaRenamer(dry_run=args.dry_run)

    if args.dry_run:
        print("=== DRY RUN MODE ===\n")

    stats = renamer.process_directory(str(path))

    print(f"\n{'='*60}")
    print(f"Total files modified: {len(stats)}")
    print(f"Total replacements: {sum(stats.values())}")

    if args.report:
        renamer.generate_report(args.report)
    elif args.dry_run:
        print("\n--- Preview Report ---")
        print(renamer.generate_report())


if __name__ == '__main__':
    main()