//////////////////////////////////////////////////////////////////////////
/// @file		computational_geometry.cpp
/// @brief		计算几何学
/// @details	COPYRIGHT NOTICE
///			    Copyright (c) 2011
///			    All rights reserved.\n
///
///
/// @author		谭川奇	chuanqi.tan(at)gmail.com
/// @date		2012/02/22
/// @version	1.0
//////////////////////////////////////////////////////////////////////////
/// 修改记录：
/// 2012/02/22		1.0	谭川奇	创建

#include <vector>
#include <iostream>
#include <queue>
#include <set>
#include <map>
#include <iterator>
#include <bitset>
#include <queue>
#include <algorithm>
#include <numeric>
#include <limits>
#include <utility>
#include <stack>
#include <cmath>

using namespace std;


namespace ita
{
	double const k_PI=3.14159265;

	/// 点
	struct PointD{
		PointD() : X(0), Y(0){}
		PointD(double x, double y) : X(x), Y(y){}

		double X;
		double Y;
	};

	/// 线段
	class Segments{
	public:
		Segments(PointD start, PointD end) : Start(start), End(end){}

		Segments(double x1, double y1, double x2, double y2) : Start(x1, y1), End(x2, y2){}

		/// @brief 判断是否与另一条线段相交
		/// 
		/// 先求两条线段所在直线的方程,求出对应的a和b(y=ax+b). <br/>
		/// 关键在于相交的两条线段一定满足这个性质:任意一条线段的2个端点一定分布在另外一条线段所对应的直线的两侧(或者之上)
		/// @note 使用了除法,精度不如<算法导论>上介绍的方法好,但是简单易懂
		bool IsIntersect(Segments const &other) const{
			double a1 = (this->Start.Y - this->End.Y) / (this->Start.X - this->End.X);
			double b1 = this->Start.Y - this->Start.X * a1;
			double a2 = (other.Start.Y - other.End.Y) / (other.Start.X - other.End.X);
			double b2 = other.Start.Y - other.Start.X * a2;		

			if ( (other.Start.Y - (other.Start.X * a1 + b1)) * (other.End.Y - (other.End.X * a1 + b1)) <= numeric_limits<double>::epsilon()
				&& (this->Start.Y - (this->Start.X * a2 + b2)) * (this->End.Y - (this->End.X * a2 + b2))  <= numeric_limits<double>::epsilon() ){
				return true;
			}
			else{
				return false;
			}
		}

		/// 与另一条线段的叉积
		double CrossProduct(Segments const &other) const{
			PointD p1(0, 0), p2(0, 0);
			p1.X = this->End.X - this->Start.X;
			p1.Y = this->End.Y - this->Start.Y;
			p2.X = other.End.X - other.Start.X;
			p2.Y = other.End.Y - other.Start.Y;

			return p1.X * p2.Y - p2.X * p1.Y;
		}


	public:
		PointD Start;
		PointD End;
	};



	/// 确定任意一对线段是否相交
	bool IsAnySegmentsIntersect(vector<Segments> const &segs){
		/// 线段的端点
		struct EndPoint{
			EndPoint(PointD point, bool isLeft, Segments const *releationSegment) 
				: Point(Point), IsLeft(isLeft), ReleationSegment(releationSegment){}

			PointD	Point;
			bool	IsLeft;
			Segments const *ReleationSegment;
		};

		vector<EndPoint> endpoints;
		for (size_t i = 0; i < segs.size(); ++i){
			Segments const &seg = segs[i];
			endpoints.push_back(EndPoint(seg.Start, true, &seg));
			endpoints.push_back(EndPoint(seg.End, false, &seg));
		}

		sort(endpoints.begin(), endpoints.end(), [](EndPoint const &lhs, EndPoint const &rhs) -> bool{
			if (lhs.Point.X != rhs.Point.X){
				return lhs.Point.X < rhs.Point.X;
			}
			else{
				if (lhs.IsLeft != rhs.IsLeft){
					return lhs.IsLeft == true;
				}
				else{
					return lhs.Point.Y < rhs.Point.Y;
				}
			}
		});


		struct _Segment_Pred_Fun{
			bool operator()(Segments const *lhs, Segments const *rhs){
				return lhs->CrossProduct(*rhs) > 0;
			}
		};

		set<Segments const *, _Segment_Pred_Fun> t;
		for (size_t i = 0; i < endpoints.size(); ++i){
			if (endpoints[i].IsLeft){
				auto insert_iter = t.insert(endpoints[i].ReleationSegment).first;

				auto above_iter = insert_iter;
				above_iter--;
				auto below_iter = insert_iter;
				below_iter++;
				if ((above_iter != t.end() && (*above_iter)->IsIntersect(*endpoints[i].ReleationSegment))
					|| (below_iter != t.end() && (*below_iter)->IsIntersect(*endpoints[i].ReleationSegment)))
				{
					return true;
				}
			}
			else{
				auto find_iter = t.find(endpoints[i].ReleationSegment);
				auto above_iter = find_iter;
				above_iter--;
				auto below_iter = find_iter;
				below_iter++;
				if (above_iter != t.end() && below_iter != t.end() && (*above_iter)->IsIntersect(**below_iter))
				{
					return true;
				}

				t.erase(find_iter);
			}
		}

		return false;
	}

	double DistanceOfTwoPoints(PointD pp1, PointD pp2){
		return sqrt((pp1.X - pp2.X)*(pp1.X - pp2.X) + (pp1.Y - pp2.Y)*(pp1.Y - pp2.Y));
	}

	/// 计算p1相对于p0的方向角度
	double CalcAngleOfTwoPoints(PointD p0, PointD p1){
		if (p1.X == p0.X){		//x坐标相同时的情况
			if (p1.Y == p0.Y){
				return 0;
			}
			else if (p1.Y > p0.Y){
				return k_PI / 2;
			}
			else{
				return 1.5 * k_PI;
			}
		}
		else{					//x坐标不同时的情况
			double angle = atan(abs((p1.Y - p0.Y) / (p1.X - p0.X)));		//普通情况为在一象限时的情况

			if (p1.Y >= p0.Y){			//在一二象限				
				if (p1.X < p0.X){	//二象限
					angle = k_PI - angle;
				}
			}
			else{						//在三四象限
				if (p1.X < p0.X){	//三象限
					angle = k_PI + angle;
				}
				else{				//四象限
					angle = 2 * k_PI - angle;
				}
			}

			return angle;
		}
	}

	///判断向量p2,p3相对于p2,p1的转向是不是向左转
	bool IsLeftTurn(PointD p1, PointD p2, PointD p3){
		//使用叉积来判断向左转还是向右转 p576页
		return Segments(p2, p1).CrossProduct(Segments(p2, p3)) > 0;
	}

	/// 求凸包的Graham算法
	stack<PointD> GrahamConvexHull(vector<PointD> const &points){
		struct PointDWapper{
			PointDWapper(PointD p, double angle, double distance) : Point(p), Angle(angle), Distance(distance) {}
			PointD Point;
			double Angle;
			double Distance;
		};

		vector<PointDWapper> ps;
		auto min_iter = min_element(points.begin(), points.end(), [](PointD p1, PointD p2){
			return p1.Y < p1.Y;
		});
		PointD min_point = *min_iter;
		
		for(size_t i = 0; i < points.size(); ++i){
			PointD const &p = points[i];
			if (p.X != min_point.X || p.Y != min_point.Y){
				PointDWapper pw(p, CalcAngleOfTwoPoints(min_point, p), DistanceOfTwoPoints(min_point, p));
				ps.push_back(pw);
			}
		};

		sort(ps.begin(), ps.end(), [&](PointDWapper p1, PointDWapper p2) -> bool{
			//以 角度的顺序+距离的逆序 进行排序
			if (p1.Angle != p2.Angle){
				return p1.Angle < p2.Angle;
			}
			else{
				return p1.Distance > p2.Distance;
			}
		});

		//清除角度相同的点 ,只保留举例最远的点
		ps.erase(unique(ps.begin(), ps.end(), [](PointDWapper p1, PointDWapper p2){
			return p1.Angle == p2.Angle;
		}), ps.end());;
		
		stack<PointD> s;
		s.push(min_point);
		s.push(ps[0].Point);
		s.push(ps[1].Point);

		for (size_t i = 2; i < ps.size(); ++i){
			do{
				PointD top = s.top();
				s.pop();
				PointD next_top = s.top();
				s.push(top);

				if (!IsLeftTurn(ps[i].Point, top, next_top)){
					s.pop();
				}
				else{
					break;
				}
			} while (true);
			s.push(ps[i].Point);
		}

		return s;
	}

	/// 寻找最近点对
	pair<PointD, PointD> NearestPointPair(vector<PointD> const &X, vector<PointD> const &Y){
		pair<PointD, PointD> min_pair;

		if (X.size() <= 3){			//临界条件,少于或等于3个点时直接求出最近距离点对
			double min_distance = numeric_limits<double>::max();			
			for (size_t i = 0; i < X.size() - 1; ++i){
				for (size_t j = i + 1; j < X.size(); ++j){
					double dis = DistanceOfTwoPoints(X[i], X[j]);
					if (dis < min_distance){
						min_distance = dis;
						min_pair = make_pair(X[i], X[j]);
					}
				}
			}
			return min_pair;
		}
		else{						//不满足临界条件,大于3小个点,需要进行分治
			auto pred_by_Y = [](PointD p1, PointD p2){
				return p1.Y < p2.Y;
			};

			vector<PointD> left_side_X(X.begin(), X.begin() + X.size() / 2);
			vector<PointD> right_side_X(X.begin() + X.size() / 2, X.end());
			vector<PointD> left_side_Y(left_side_X);
			vector<PointD> right_side_Y(right_side_X);
			sort(left_side_Y.begin(), left_side_Y.end(), pred_by_Y);
			sort(right_side_Y.begin(), right_side_Y.end(), pred_by_Y);

			auto left_nearest = NearestPointPair(left_side_X, left_side_Y);
			auto right_nearest = NearestPointPair(right_side_X, right_side_Y);

			double nearest_distance = 0;
			if (DistanceOfTwoPoints(left_nearest.first, left_nearest.second) < DistanceOfTwoPoints(right_nearest.first, right_nearest.second)){
				nearest_distance = DistanceOfTwoPoints(left_nearest.first, left_nearest.second);
				min_pair = left_nearest;
			}
			else{
				nearest_distance = DistanceOfTwoPoints(right_nearest.first, right_nearest.second);
				min_pair = right_nearest;
			}

			double split_x = (*(X.begin() + X.size() / 2)).X;
			vector<PointD> y_plus;
			copy_if(X.begin(), X.end(), back_inserter(y_plus), [&](PointD p){
				return abs(p.X - split_x) <= nearest_distance;
			});
			sort(y_plus.begin(), y_plus.end(), pred_by_Y);
			for (size_t i = 0; i < y_plus.size() - 1; ++i){
				int count = 0;
				for (size_t j = i + 1; j < y_plus.size() && count < 8; ++j, ++count){
					if (DistanceOfTwoPoints(y_plus[i], y_plus[j]) < nearest_distance){
						nearest_distance = DistanceOfTwoPoints(y_plus[i], y_plus[j]);
						min_pair = make_pair(y_plus[i], y_plus[j]);
					}
				}
			}

			return min_pair;
		}
	}

	pair<PointD, PointD> GetNearestPointPair(vector<PointD> const &points){
		if (points.size() < 2){
			throw exception("Error number of points");
		}

		vector<PointD> X(points);
		vector<PointD> Y(points);

		//数组X按X坐标顺序排列
		sort(X.begin(), X.end(), [](PointD p1, PointD p2){
			return p1.X < p2.X;
		});
		//数组Y按Y坐标顺序排列
		sort(Y.begin(), Y.end(), [](PointD p1, PointD p2){
			return p1.Y < p2.Y;
		});

		return NearestPointPair(X, Y);
	}

	void testComputionalGeometry(){
		// 测试IsIntersect算法
		cout << "============测试IsIntersect算法============" << endl;
		Segments s1(0, 0, 5, 5);
		Segments s2(0, 5, 5, 0);
		Segments s3(1, 0, 6, 5);
		cout << boolalpha << s1.IsIntersect(s2) << endl;
		cout << boolalpha << s1.IsIntersect(s3) << endl;
		

		cout << "===============Graham凸包算法==============" << endl;
		vector<PointD> points;
		points.push_back(PointD(1, 1));
		points.push_back(PointD(5, 1));
		points.push_back(PointD(1, 5));
		points.push_back(PointD(5, 5));
		points.push_back(PointD(3, 2));
		stack<PointD> ps = GrahamConvexHull(points);
		while(!ps.empty()){
			PointD p = ps.top();
			cout << "(" << p.X << ", " << p.Y << ")" << endl;
			ps.pop();
		}

		cout << "============最近点对的分治算法=============" << endl;		
		pair<PointD, PointD> pp = GetNearestPointPair(points);
		cout << pp.first.X << ", " << pp.first.Y << " ====== " << pp.second.X << ", " << pp.second.Y << endl;
	}
}