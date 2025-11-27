#include <iostream>
#include <vector>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <windows.h>

using namespace std;
namespace bg = boost::geometry;

// 定义点类型（2D笛卡尔坐标）
typedef bg::model::point<double, 2, bg::cs::cartesian> point_t;

// 定义多边形类型
typedef bg::model::polygon<point_t> polygon_t;

// 定义线段类型
typedef bg::model::linestring<point_t> linestring_t;

// 定义矩形类型
typedef bg::model::box<point_t> box_t;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Boost.Geometry几何库示例 ===" << endl;

    // 示例1: 点和距离计算
    cout << "\n1. 点和距离计算:" << endl;

    point_t p1(0.0, 0.0);
    point_t p2(3.0, 4.0);

    double dist = bg::distance(p1, p2);
    cout << "点1: (" << bg::get<0>(p1) << ", " << bg::get<1>(p1) << ")" << endl;
    cout << "点2: (" << bg::get<0>(p2) << ", " << bg::get<1>(p2) << ")" << endl;
    cout << "距离: " << dist << endl;

    // 示例2: 创建多边形
    cout << "\n2. 创建多边形:" << endl;

    polygon_t poly;
    // 添加外环点（逆时针）
    bg::append(poly, point_t(0.0, 0.0));
    bg::append(poly, point_t(4.0, 0.0));
    bg::append(poly, point_t(4.0, 4.0));
    bg::append(poly, point_t(0.0, 4.0));
    bg::append(poly, point_t(0.0, 0.0));  // 闭合

    double area = bg::area(poly);
    double perimeter = bg::perimeter(poly);

    cout << "多边形顶点数: " << poly.outer().size() << endl;
    cout << "面积: " << area << endl;
    cout << "周长: " << perimeter << endl;

    // 示例3: 点在多边形内判断
    cout << "\n3. 点在多边形内判断:" << endl;

    point_t pt1(2.0, 2.0);  // 在多边形内
    point_t pt2(5.0, 5.0);  // 在多边形外

    bool inside1 = bg::within(pt1, poly);
    bool inside2 = bg::within(pt2, poly);

    cout << "点(" << bg::get<0>(pt1) << ", " << bg::get<1>(pt1)
         << ") 在多边形内: " << (inside1 ? "是" : "否") << endl;
    cout << "点(" << bg::get<0>(pt2) << ", " << bg::get<1>(pt2)
         << ") 在多边形内: " << (inside2 ? "是" : "否") << endl;

    // 示例4: 计算质心
    cout << "\n4. 计算质心:" << endl;

    point_t centroid;
    bg::centroid(poly, centroid);

    cout << "多边形质心: (" << bg::get<0>(centroid) << ", "
         << bg::get<1>(centroid) << ")" << endl;

    // 示例5: 创建复杂多边形（带内环）
    cout << "\n5. 复杂多边形（带内环）:" << endl;

    polygon_t complex_poly;

    // 外环
    bg::append(complex_poly, point_t(0.0, 0.0));
    bg::append(complex_poly, point_t(5.0, 0.0));
    bg::append(complex_poly, point_t(5.0, 5.0));
    bg::append(complex_poly, point_t(0.0, 5.0));
    bg::append(complex_poly, point_t(0.0, 0.0));

    // 内环（洞）
    complex_poly.inners().resize(1);
    bg::append(complex_poly.inners()[0], point_t(1.0, 1.0));
    bg::append(complex_poly.inners()[0], point_t(4.0, 1.0));
    bg::append(complex_poly.inners()[0], point_t(4.0, 4.0));
    bg::append(complex_poly.inners()[0], point_t(1.0, 4.0));
    bg::append(complex_poly.inners()[0], point_t(1.0, 1.0));

    double complex_area = bg::area(complex_poly);
    cout << "复杂多边形面积（外环-内环）: " << complex_area << endl;

    // 示例6: 线段操作
    cout << "\n6. 线段操作:" << endl;

    linestring_t line;
    bg::append(line, point_t(0.0, 0.0));
    bg::append(line, point_t(3.0, 4.0));
    bg::append(line, point_t(6.0, 2.0));

    double line_length = bg::length(line);
    cout << "线段长度: " << line_length << endl;

    // 示例7: 矩形（包围盒）
    cout << "\n7. 矩形（包围盒）:" << endl;

    point_t min_corner(1.0, 1.0);
    point_t max_corner(4.0, 4.0);
    box_t box(min_corner, max_corner);

    double box_area = bg::area(box);
    double box_perimeter = bg::perimeter(box);

    cout << "矩形最小角: (" << bg::get<0>(min_corner) << ", "
         << bg::get<1>(min_corner) << ")" << endl;
    cout << "矩形最大角: (" << bg::get<0>(max_corner) << ", "
         << bg::get<1>(max_corner) << ")" << endl;
    cout << "矩形面积: " << box_area << endl;
    cout << "矩形周长: " << box_perimeter << endl;

    // 示例8: 空间关系判断
    cout << "\n8. 空间关系判断:" << endl;

    polygon_t poly2;
    bg::append(poly2, point_t(2.0, 2.0));
    bg::append(poly2, point_t(6.0, 2.0));
    bg::append(poly2, point_t(6.0, 6.0));
    bg::append(poly2, point_t(2.0, 6.0));
    bg::append(poly2, point_t(2.0, 2.0));

    // 判断两个多边形是否相交
    bool intersects = bg::intersects(poly, poly2);
    cout << "多边形1和多边形2是否相交: " << (intersects ? "是" : "否") << endl;

    // 判断一个多边形是否包含另一个
    bool contains = bg::within(poly, poly2);
    cout << "多边形1是否在多边形2内: " << (contains ? "是" : "否") << endl;

    // 示例9: 计算两个多边形的交集
    cout << "\n9. 计算交集:" << endl;

    vector<polygon_t> output;
    bg::intersection(poly, poly2, output);

    if (!output.empty()) {
        cout << "交集多边形数量: " << output.size() << endl;
        for (size_t i = 0; i < output.size(); ++i) {
            double intersection_area = bg::area(output[i]);
            cout << "交集" << (i+1) << "面积: " << intersection_area << endl;
        }
    }

    // 示例10: 简化多边形
    cout << "\n10. 简化多边形:" << endl;

    polygon_t simplified;
    bg::simplify(poly, simplified, 0.5);  // 简化阈值0.5

    cout << "原始多边形顶点数: " << poly.outer().size() << endl;
    cout << "简化后顶点数: " << simplified.outer().size() << endl;

    cin.get();
    return 0;
}
