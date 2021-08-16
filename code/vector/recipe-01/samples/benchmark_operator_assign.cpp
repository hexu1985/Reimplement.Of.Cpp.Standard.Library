#include <vector>
#include <iostream>
 
void display_sizes(char const* comment,
                   const std::vector<int>& nums1,
                   const std::vector<int>& nums2,
                   const std::vector<int>& nums3)
{
    std::cout << comment
              << " nums1: " << nums1.size() << ','
              << " nums2: " << nums2.size() << ','
              << " nums3: " << nums3.size() << '\n';
}
 
void display(char const* comment, const std::vector<int>& v)
{
    std::cout << comment << "{ ";
    for (int e : v) {
        std::cout << e << ' ';
    }
    std::cout << "}\n";
}
 
int main()
{
    std::vector<int> nums1 {3, 1, 4, 6, 5, 9};
    std::vector<int> nums2;
    std::vector<int> nums3;
 
    display_sizes("Initially:\n", nums1, nums2, nums3);
 
    // 从 nums1 复制赋值数据到 nums2
    nums2 = nums1;
 
    display_sizes("After assigment:\n", nums1, nums2, nums3);
 
    // 从 nums1 移动赋值数据到 nums3,
    // 修改 nums1 和 nums3
    nums3 = std::move(nums1);
 
    display_sizes("After move assigment:\n", nums1, nums2, nums3);
 
    display("Now nums3 = ", nums3);
 
    // initializer_list 的复制赋值复制数据给 nums3
    nums3 = {1, 2, 3};
 
    display("After assignment of initializer_list \n nums3 = ", nums3);
}
