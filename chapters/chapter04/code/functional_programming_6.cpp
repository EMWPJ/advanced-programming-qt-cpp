#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>
#include <iterator>
#include <random>
#include <set>

int main() {
    std::vector<int> data = {1, 2, 3, 4, 5};
    // 1. std::for_each：对每个元素加1
    std::for_each(data.begin(), data.end(), [](int& x) { x += 1; });
    std::cout << "for_each每个元素加1后: ";
    for (auto x : data) std::cout << x << " ";
    std::cout << std::endl;

    // 2. std::transform：所有元素平方，结果存入新vector
    std::vector<int> squared(data.size());
    std::transform(data.begin(), data.end(), squared.begin(), [](int x) { return x * x; });
    std::cout << "transform平方后: ";
    for (auto x : squared) std::cout << x << " ";
    std::cout << std::endl;

    // 3. std::count_if：统计偶数个数
    int even_count = std::count_if(data.begin(), data.end(), [](int x) { return x % 2 == 0; });
    std::cout << "count_if偶数个数: " << even_count << std::endl;

    // 4. std::accumulate：求和
    int sum = std::accumulate(data.begin(), data.end(), 0);
    std::cout << "accumulate求和: " << sum << std::endl;

    // 5. std::find_if：查找第一个大于3的元素
    auto it = std::find_if(data.begin(), data.end(), [](int x) { return x > 3; });
    if (it != data.end()) {
        std::cout << "find_if第一个大于3的元素: " << *it << std::endl;
    } else {
        std::cout << "没有找到大于3的元素" << std::endl;
    }

    // 6. std::all_of / any_of / none_of
    bool all_positive = std::all_of(data.begin(), data.end(), [](int x) { return x > 0; });
    bool any_even = std::any_of(data.begin(), data.end(), [](int x) { return x % 2 == 0; });
    bool none_negative = std::none_of(data.begin(), data.end(), [](int x) { return x < 0; });
    std::cout << "all_of全为正数: " << std::boolalpha << all_positive << std::endl;
    std::cout << "any_of存在偶数: " << std::boolalpha << any_even << std::endl;
    std::cout << "none_of没有负数: " << std::boolalpha << none_negative << std::endl;

    // 7. std::remove_if + erase：移除所有小于3的元素
    auto new_end = std::remove_if(data.begin(), data.end(), [](int x) { return x < 3; });
    data.erase(new_end, data.end());
    std::cout << "remove_if移除小于3后: ";
    for (auto x : data) std::cout << x << " ";
    std::cout << std::endl;

    // 8. std::reverse：反转vector
    std::reverse(data.begin(), data.end());
    std::cout << "reverse反转后: ";
    for (auto x : data) std::cout << x << " ";
    std::cout << std::endl;

    // 9. std::unique：去重（需先排序）
    std::vector<int> dup = {1, 2, 2, 3, 3, 3, 4, 5, 5};
    std::sort(dup.begin(), dup.end());
    auto last = std::unique(dup.begin(), dup.end());
    dup.erase(last, dup.end());
    std::cout << "unique去重后: ";
    for (auto x : dup) std::cout << x << " ";
    std::cout << std::endl;

    // 10. std::partition：分区（偶数在前，奇数在后）
    std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8};
    std::partition(nums.begin(), nums.end(), [](int x) { return x % 2 == 0; });
    std::cout << "partition偶数在前: ";
    for (auto x : nums) std::cout << x << " ";
    std::cout << std::endl;

    // 11. std::min_element / std::max_element
    auto min_it = std::min_element(nums.begin(), nums.end());
    auto max_it = std::max_element(nums.begin(), nums.end());
    if (min_it != nums.end() && max_it != nums.end()) {
        std::cout << "min_element最小值: " << *min_it << std::endl;
        std::cout << "max_element最大值: " << *max_it << std::endl;
    }

    // 12. std::copy_if：复制所有大于4的元素到新vector
    std::vector<int> greater_than_4;
    std::copy_if(nums.begin(), nums.end(), std::back_inserter(greater_than_4), [](int x) { return x > 4; });
    std::cout << "copy_if大于4的元素: ";
    for (auto x : greater_than_4) std::cout << x << " ";
    std::cout << std::endl;

    // 13. std::set_union：求两个集合的并集
    std::vector<int> a = {1, 2, 3, 5, 7};
    std::vector<int> b = {2, 3, 4, 6, 8};
    std::vector<int> union_result;
    std::set_union(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(union_result));
    std::cout << "set_union并集: ";
    for (auto x : union_result) std::cout << x << " ";
    std::cout << std::endl;

    // 14. std::set_intersection：求交集
    std::vector<int> intersection_result;
    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(intersection_result));
    std::cout << "set_intersection交集: ";
    for (auto x : intersection_result) std::cout << x << " ";
    std::cout << std::endl;

    // 15. std::set_difference：求差集
    std::vector<int> diff_result;
    std::set_difference(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(diff_result));
    std::cout << "set_difference差集(a-b): ";
    for (auto x : diff_result) std::cout << x << " ";
    std::cout << std::endl;

    // 16. std::generate：生成序列
    std::vector<int> seq(10);
    int v = 1;
    std::generate(seq.begin(), seq.end(), [&v]() { return v++; });
    std::cout << "generate生成序列: ";
    for (auto x : seq) std::cout << x << " ";
    std::cout << std::endl;

    // 17. std::shuffle：随机打乱
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(seq.begin(), seq.end(), g);
    std::cout << "shuffle打乱后: ";
    for (auto x : seq) std::cout << x << " ";
    std::cout << std::endl;

    // 18. std::fill：填充
    std::fill(seq.begin(), seq.end(), 7);
    std::cout << "fill填充为7: ";
    for (auto x : seq) std::cout << x << " ";
    std::cout << std::endl;

    // 19. std::replace_if：条件替换
    std::vector<int> rep = {1, 2, 3, 4, 5, 6};
    std::replace_if(rep.begin(), rep.end(), [](int x) { return x % 2 == 0; }, 0);
    std::cout << "replace_if偶数替换为0: ";
    for (auto x : rep) std::cout << x << " ";
    std::cout << std::endl;

    // 20. std::merge：合并有序序列
    std::vector<int> m1 = {1, 3, 5};
    std::vector<int> m2 = {2, 4, 6};
    std::vector<int> merged;
    std::merge(m1.begin(), m1.end(), m2.begin(), m2.end(), std::back_inserter(merged));
    std::cout << "merge合并有序序列: ";
    for (auto x : merged) std::cout << x << " ";
    std::cout << std::endl;

    // 21. std::lower_bound / std::upper_bound
    std::vector<int> sorted = {1, 2, 4, 4, 5, 7};
    auto lb = std::lower_bound(sorted.begin(), sorted.end(), 4);
    auto ub = std::upper_bound(sorted.begin(), sorted.end(), 4);
    std::cout << "lower_bound(4)位置: " << (lb - sorted.begin()) << std::endl;
    std::cout << "upper_bound(4)位置: " << (ub - sorted.begin()) << std::endl;

    // 22. std::equal：判断两个序列是否相等
    std::vector<int> eq1 = {1, 2, 3};
    std::vector<int> eq2 = {1, 2, 3};
    std::cout << "equal两个序列是否相等: " << std::boolalpha << std::equal(eq1.begin(), eq1.end(), eq2.begin()) << std::endl;

    // 23. std::find_end：查找子序列最后一次出现的位置
    std::vector<int> big = {1, 2, 3, 2, 3, 4, 2, 3};
    std::vector<int> sub = {2, 3};
    auto end_pos = std::find_end(big.begin(), big.end(), sub.begin(), sub.end());
    if (end_pos != big.end()) {
        std::cout << "find_end子序列最后一次出现的位置: " << (end_pos - big.begin()) << std::endl;
    } else {
        std::cout << "find_end未找到子序列" << std::endl;
    }

    // 24. std::adjacent_find：查找相邻重复元素
    std::vector<int> adj = {1, 2, 2, 3, 4, 4, 4, 5};
    auto adj_it = std::adjacent_find(adj.begin(), adj.end());
    if (adj_it != adj.end()) {
        std::cout << "adjacent_find第一个相邻重复元素: " << *adj_it << std::endl;
    } else {
        std::cout << "adjacent_find未找到相邻重复元素" << std::endl;
    }

    // 25. std::inplace_merge：原地合并有序区间
    std::vector<int> im = {1, 3, 5, 2, 4, 6};
    std::inplace_merge(im.begin(), im.begin() + 3, im.end());
    std::cout << "inplace_merge原地合并后: ";
    for (auto x : im) std::cout << x << " ";
    std::cout << std::endl;

    // 26. std::nth_element：部分排序
    std::vector<int> nth = {7, 2, 5, 3, 9, 1};
    std::nth_element(nth.begin(), nth.begin() + 2, nth.end());
    std::cout << "nth_element第3小的元素: " << nth[2] << std::endl;
    std::cout << "nth_element后前3小: ";
    for (int i = 0; i < 3; ++i) std::cout << nth[i] << " ";
    std::cout << std::endl;

    // 27. std::is_sorted / std::is_sorted_until
    std::vector<int> isort = {1, 2, 3, 5, 4, 6};
    std::cout << "is_sorted是否有序: " << std::boolalpha << std::is_sorted(isort.begin(), isort.end()) << std::endl;
    auto until = std::is_sorted_until(isort.begin(), isort.end());
    std::cout << "is_sorted_until第一个无序位置: " << (until - isort.begin()) << std::endl;

    // 28. std::for_each_n：对前n个元素操作（C++17）
#if __cplusplus >= 201703L
    std::vector<int> nvec = {1, 2, 3, 4, 5};
    std::for_each_n(nvec.begin(), 3, [](int& x) { x *= 10; });
    std::cout << "for_each_n前3个元素乘10: ";
    for (auto x : nvec) std::cout << x << " ";
    std::cout << std::endl;
#endif

    // 29. std::sample：从序列中随机采样（C++17）
#if __cplusplus >= 201703L
    std::vector<int> pop = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> samp;
    std::sample(pop.begin(), pop.end(), std::back_inserter(samp), 4, g);
    std::cout << "sample随机采样4个元素: ";
    for (auto x : samp) std::cout << x << " ";
    std::cout << std::endl;
#endif

    // 30. std::includes：判断一个有序序列是否包含另一个有序序列
    std::vector<int> inc1 = {1, 2, 3, 4, 5, 6};
    std::vector<int> inc2 = {2, 4, 6};
    std::cout << "includes inc1是否包含inc2: " << std::boolalpha
              << std::includes(inc1.begin(), inc1.end(), inc2.begin(), inc2.end()) << std::endl;

    std::cin.get();
    return 0;
}
