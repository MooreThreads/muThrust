#!/usr/bin/env python3
"""
Parse Thrust test verbose log and generate markdown report
"""

import re
import sys
from datetime import datetime
from pathlib import Path
from collections import defaultdict
from dataclasses import dataclass, field
from typing import List, Dict, Tuple

@dataclass
class TestCase:
    """A failed test case"""
    description: str
    error_detail: str

@dataclass
class TestInfo:
    """Test program info"""
    name: str
    result: str  # "Passed", "Failed", "Timeout", "Exception"
    elapsed_time: float
    total_cases: int = 0
    passed_cases: int = 0
    failed_cases: int = 0
    failed_details: List[TestCase] = field(default_factory=list)

def parse_log(log_path: str) -> Tuple[List[TestInfo], int]:
    """Parse test log and return test info list and total test count"""

    with open(log_path, 'r', encoding='utf-8', errors='ignore') as f:
        content = f.read()

    lines = content.split('\n')

    # Parse test summary lines - handle various formats:
    # " 1/96 Test  #1: thrust.cpp17.test.scan ... Passed 0.70 sec"
    # " 3/96 Test  #3: thrust.cpp17.test.sort ... ***Failed 0.19 sec"
    # "63/96 Test #63: thrust.cpp17.test.reduce ... Subprocess aborted***Exception: 7.21 sec"
    test_pattern = re.compile(r'^\s*(\d+)/(\d+)\s+Test\s+#(\d+):\s+(\S+)\s+(?:\.+)\s*(Passed|\*\*\*Failed|\*\*\*Timeout|Subprocess aborted\*\*\*Exception:)\s+(\d+\.?\d*)\s+sec')

    tests = []
    test_map = {}  # test number -> TestInfo

    for line in lines:
        match = test_pattern.match(line)
        if match:
            test_num = int(match.group(3))
            test_name = match.group(4)
            result_raw = match.group(5)
            elapsed = float(match.group(6))

            # Normalize result
            if "Passed" in result_raw:
                result = "Passed"
            elif "Failed" in result_raw:
                result = "Failed"
            elif "Timeout" in result_raw:
                result = "Timeout"
            elif "Exception" in result_raw:
                result = "Exception"
            else:
                result = "Failed"

            info = TestInfo(
                name=test_name,
                result=result,
                elapsed_time=elapsed
            )
            tests.append(info)
            test_map[test_num] = info

    # Parse PASS/FAIL cases for each test
    # Support multiple formats:
    # 1. Format: "17: 	Scan results: PASS"  -> <num>: <desc>: PASS/FAIL
    # 2. Format: "3: 	PASS"                  -> <num>: PASS/FAIL (desc on previous line)
    # 3. Format: "29: 	Channel 0 PASS"       -> <num>: <desc> PASS/FAIL (no colon before PASS)
    # 4. Format: "47: 	PASSInvoking..."      -> <num>: PASS/FAIL<something>
    # 5. Format: "51: 	 Keys PASS 	 Values PASS 	 Count PASS"  -> multi PASS/FAIL in one line
    # 6. Thrust specific: "[test_name] PASSED/FAILED" or "PASSED/FAILED"
    # 7. Thrust TestStats: "X out of Y tests passed"

    case_pattern1 = re.compile(r'^(\d+):\s+(.+):\s+(PASS|FAIL)')
    case_pattern2 = re.compile(r'^(\d+):\s+(PASS|FAIL)$')
    case_pattern3 = re.compile(r'^(\d+):\s+(.+?)\s+(PASS|FAIL)$')
    case_pattern4 = re.compile(r'^(\d+):\s+(PASS|FAIL)(?=\S|$)')
    # Thrust-specific patterns
    thrust_pass_pattern = re.compile(r'\bPASSED\b')
    thrust_fail_pattern = re.compile(r'\bFAILED\b')
    # Thrust test summary: "X out of Y tests passed" or "passed X tests"
    thrust_summary_pattern = re.compile(r'(\d+)\s+out\s+of\s+(\d+)\s+tests?\s+passed', re.IGNORECASE)
    thrust_summary_pattern2 = re.compile(r'passed\s+(\d+)\s+tests?', re.IGNORECASE)

    # Multiple PASS/FAIL on same line
    case_pattern5 = re.compile(r'^(\d+):\s+.*\b(PASS|FAIL)\b.*\b(PASS|FAIL)\b')

    # TestStats summary patterns
    summary_pattern1 = re.compile(r'^All\s+(\d+)\s+test\s+cases?\s+passed', re.IGNORECASE)
    summary_pattern2 = re.compile(r'^(\S+):\s+(\d+)\s+cases?\s+passed', re.IGNORECASE)
    summary_pattern3 = re.compile(r'^(\d+):\s+(\S+):\s+(\d+)\s+cases?\s+passed')

    prev_line = ""
    current_test_num = 0

    for line in lines:
        matched = False
        test_num = 0
        test_desc = ""
        result = ""
        multi_case = False

        # First try format 5: multi PASS/FAIL in one line
        match = case_pattern5.match(line)
        if match:
            test_num = int(match.group(1))
            pass_count = len(re.findall(r'\bPASS\b', line))
            fail_count = len(re.findall(r'\bFAIL\b', line))
            if pass_count > 0 or fail_count > 0:
                multi_case = True
                matched = True
                prev_match = re.match(r'^(\d+):\s+(.+)$', prev_line)
                if prev_match and int(prev_match.group(1)) == test_num:
                    test_desc = prev_match.group(2).strip()
                else:
                    test_desc = "multi-test"
                result = "PASS" if fail_count == 0 else "FAIL"

        if not matched:
            # Try format 1: <num>: <desc>: PASS/FAIL
            match = case_pattern1.match(line)
            if match:
                test_num = int(match.group(1))
                test_desc = match.group(2).strip()
                result = match.group(3)
                matched = True
            else:
                # Try format 4 first
                match = case_pattern4.match(line)
                if match:
                    test_num = int(match.group(1))
                    result = match.group(2)
                    prev_match = re.match(r'^(\d+):\s+(.+)$', prev_line)
                    if prev_match and int(prev_match.group(1)) == test_num:
                        test_desc = prev_match.group(2).strip()
                    else:
                        test_desc = "test"
                    matched = True
                else:
                    # Try format 2
                    match = case_pattern2.match(line)
                    if match:
                        test_num = int(match.group(1))
                        result = match.group(2)
                        prev_match = re.match(r'^(\d+):\s+(.+)$', prev_line)
                        if prev_match and int(prev_match.group(1)) == test_num:
                            test_desc = prev_match.group(2).strip()
                        else:
                            test_desc = "test"
                        matched = True
                    else:
                        # Try format 3
                        match = case_pattern3.match(line)
                        if match:
                            test_num = int(match.group(1))
                            test_desc = match.group(2).strip()
                            result = match.group(3)
                            matched = True

        if matched and test_num in test_map:
            current_test_num = test_num
            info = test_map[test_num]
            if multi_case:
                pass_count = len(re.findall(r'\bPASS\b', line))
                fail_count = len(re.findall(r'\bFAIL\b', line))
                info.total_cases += pass_count + fail_count
                info.passed_cases += pass_count
                info.failed_cases += fail_count
                if fail_count > 0 and len(info.failed_details) < 10:
                    info.failed_details.append(TestCase(
                        description=test_desc,
                        error_detail=f"FAIL ({fail_count}/{pass_count + fail_count})"
                    ))
            else:
                info.total_cases += 1
                if result == "PASS":
                    info.passed_cases += 1
                else:
                    info.failed_cases += 1
                    if len(info.failed_details) < 10:
                        info.failed_details.append(TestCase(
                            description=test_desc,
                            error_detail="FAIL"
                        ))

        # Handle TestStats summary patterns
        if not matched:
            match = summary_pattern1.match(line)
            if match and current_test_num > 0 and current_test_num in test_map:
                pass_count = int(match.group(1))
                info = test_map[current_test_num]
                info.total_cases += pass_count
                info.passed_cases += pass_count
                matched = True
            else:
                match = summary_pattern3.match(line)
                if match:
                    test_num = int(match.group(1))
                    if test_num in test_map:
                        pass_count = int(match.group(3))
                        info = test_map[test_num]
                        info.total_cases += pass_count
                        info.passed_cases += pass_count
                        matched = True
                if not matched:
                    match = summary_pattern2.match(line)
                    if match and current_test_num > 0 and current_test_num in test_map:
                        pass_count = int(match.group(2))
                        info = test_map[current_test_num]
                        info.total_cases += pass_count
                        info.passed_cases += pass_count
                        matched = True

        # Thrust-specific: "X out of Y tests passed"
        if not matched:
            match = thrust_summary_pattern.search(line)
            if match:
                passed = int(match.group(1))
                total = int(match.group(2))
                if current_test_num > 0 and current_test_num in test_map:
                    info = test_map[current_test_num]
                    info.total_cases += total
                    info.passed_cases += passed
                    info.failed_cases += (total - passed)
                    matched = True

        prev_line = line

    return tests, len(tests)

def get_category(test_name: str) -> str:
    """Extract test category from test name"""
    # Remove prefix
    name = test_name.replace("thrust.cpp17.test.", "").replace("thrust.cpp17.example.", "")
    name = name.replace("thrust.test.cmake.", "").replace("thrust.example.cmake.", "")
    name = name.replace("thrust.test.", "").replace("thrust.example.", "")

    # Extract the test type (first segment before dot or underscore)
    parts = name.replace("_", ".").split(".")

    # For cmake tests
    if parts[0] in ["cmake", "add_subdir", "check_source_files", "test_install"]:
        return "cmake"

    return parts[0]

def generate_report(tests: List[TestInfo], output_path: str, filter_tests: bool = True):
    """Generate markdown report"""

    # Filter tests if requested (keep only thrust.test.* tests)
    if filter_tests:
        tests = [t for t in tests if ".test." in t.name]

    # Categorize tests
    categories: Dict[str, Dict] = defaultdict(lambda: {"passed": 0, "failed": 0, "timeout": 0, "exception": 0, "total": 0})

    for t in tests:
        cat = get_category(t.name)
        categories[cat]["total"] += 1

        # Determine effective result
        effective_result = t.result
        if t.result == "Passed" and t.failed_cases > 0:
            effective_result = "Failed"

        if effective_result == "Passed":
            categories[cat]["passed"] += 1
        elif effective_result == "Timeout":
            categories[cat]["timeout"] += 1
        elif effective_result == "Exception":
            categories[cat]["exception"] += 1
        else:
            categories[cat]["failed"] += 1

    # Calculate totals
    def get_effective_result(t):
        if t.result == "Passed" and t.failed_cases > 0:
            return "Failed"
        return t.result

    total_tests = len(tests)
    passed_tests = sum(1 for t in tests if get_effective_result(t) == "Passed")
    failed_tests = sum(1 for t in tests if get_effective_result(t) == "Failed")
    timeout_tests = sum(1 for t in tests if get_effective_result(t) == "Timeout")
    exception_tests = sum(1 for t in tests if get_effective_result(t) == "Exception")

    total_cases = sum(t.total_cases for t in tests)
    passed_cases = sum(t.passed_cases for t in tests)
    failed_cases = sum(t.failed_cases for t in tests)

    program_pass_rate = (passed_tests / total_tests * 100) if total_tests > 0 else 0
    case_pass_rate = (passed_cases / total_cases * 100) if total_cases > 0 else 0

    if failed_cases > 0 and case_pass_rate >= 99.95:
        case_pass_rate = 99.9

    # Build report
    report = []
    report.append("# Thrust 测试统计报告 (ctest verbose output)")
    report.append("")
    report.append(f"**生成时间:** {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
    report.append("")
    report.append("---")
    report.append("")

    # Table 1: Test program statistics by category
    report.append("## 表格1: 测试程序统计")
    report.append("")
    report.append("| 测试类型 | 总数 | 通过 | 失败 | 超时 | 异常 | 通过率 |")
    report.append("|:---------|-----:|-----:|-----:|-----:|-----:|-------:|")

    for cat in sorted(categories.keys()):
        stats = categories[cat]
        total = stats["total"]
        passed = stats["passed"]
        failed = stats["failed"]
        timeout = stats["timeout"]
        exception = stats["exception"]
        rate = (passed / total * 100) if total > 0 else 0
        report.append(f"| {cat} | {total} | {passed} | {failed} | {timeout} | {exception} | {rate:.1f}% |")

    report.append(f"| **总计** | **{total_tests}** | **{passed_tests}** | **{failed_tests}** | **{timeout_tests}** | **{exception_tests}** | **{program_pass_rate:.1f}%** |")
    report.append("")

    # Table 2: Detailed test case statistics
    report.append("## 表格2: 测试用例详细统计")
    report.append("")
    report.append("| 状态 | 测试程序 | 总Case | 通过 | 失败 | 通过率 |")
    report.append("|:----:|:---------|-------:|-----:|-----:|-------:|")

    for t in sorted(tests, key=lambda x: x.name):
        short_name = t.name.replace("thrust.cpp17.test.", "").replace("thrust.cpp17.example.", "")
        short_name = short_name.replace("thrust.test.cmake.", "").replace("thrust.example.cmake.", "")
        short_name = short_name.replace("thrust.test.", "").replace("thrust.example.", "")

        effective_result = get_effective_result(t)
        status = "✓" if effective_result == "Passed" else "✗"

        if t.total_cases > 0:
            rate = (t.passed_cases / t.total_cases * 100) if t.total_cases > 0 else 0
            report.append(f"| {status} | {short_name} | {t.total_cases} | {t.passed_cases} | {t.failed_cases} | {rate:.1f}% |")
        else:
            if t.result == "Timeout":
                reason = "超时"
            elif t.result == "Exception":
                reason = "异常"
            elif t.result == "Failed":
                reason = "失败"
            else:
                reason = "-"
            report.append(f"| {status} | {short_name} | - | - | - | {reason} |")

    report.append(f"| | **总计** | **{total_cases}** | **{passed_cases}** | **{failed_cases}** | **{case_pass_rate:.1f}%** |")
    report.append("")

    # Table 3: Failed test details
    failed_tests_list = [t for t in tests if t.result != "Passed" or t.failed_cases > 0]

    if failed_tests_list:
        report.append("## 表格3: 失败用例详情")
        report.append("")

        for t in sorted(failed_tests_list, key=lambda x: x.name):
            short_name = t.name.replace("thrust.cpp17.test.", "").replace("thrust.cpp17.example.", "")
            short_name = short_name.replace("thrust.test.cmake.", "").replace("thrust.example.cmake.", "")
            short_name = short_name.replace("thrust.test.", "").replace("thrust.example.", "")

            report.append(f"### {short_name}")
            report.append("")

            if t.result == "Timeout":
                report.append(f"> **TIMEOUT:** 超时({t.elapsed_time:.0f}秒)")
            elif t.result == "Exception":
                report.append(f"> **EXCEPTION:** 子进程异常终止")
            elif t.failed_cases > 0:
                report.append(f"**失败:** {t.failed_cases}/{t.total_cases} cases")
                report.append("")

                if t.failed_details:
                    shown = min(len(t.failed_details), 5)
                    report.append("| 测试描述 |")
                    report.append("|:---------|")
                    for fc in t.failed_details[:shown]:
                        report.append(f"| {fc.description} |")
                    if len(t.failed_details) > shown:
                        report.append(f"| ... (还有 {len(t.failed_details) - shown} 个失败用例) |")

            report.append("")

    report.append("---")
    report.append("")
    report.append("## 总结")
    report.append("")
    report.append(f"- **测试程序:** {passed_tests}/{total_tests} 通过 ({program_pass_rate:.1f}%)")
    report.append(f"- **测试用例:** {passed_cases}/{total_cases} 通过 ({case_pass_rate:.1f}%)")
    if failed_tests + timeout_tests + exception_tests > 0:
        report.append(f"- 有 **{failed_tests + timeout_tests + exception_tests}** 个测试程序未通过")
    report.append("")

    report_str = "\n".join(report)

    with open(output_path, 'w', encoding='utf-8') as f:
        f.write(report_str)

    print(f"Report saved to: {output_path}")
    return report_str

def main():
    if len(sys.argv) < 2:
        log_path = "test_verbose.log"
    else:
        log_path = sys.argv[1]

    output_path = "test_report.md"
    if len(sys.argv) >= 3:
        output_path = sys.argv[2]

    print(f"Parsing log: {log_path}")
    tests, total = parse_log(log_path)
    print(f"Found {total} tests")

    report = generate_report(tests, output_path)
    print("\n" + report)

if __name__ == "__main__":
    main()
