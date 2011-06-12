/// 图的表示法
/// 
/// 图的两种表示方法：（1）邻接表法；（2）邻接矩阵法
/// 


#include <vector>
#include <utility>
using namespace std;

namespace chapter22
{
	/// 图的类型：有向图还是无向图
	enum GraphicsType
	{
		Digraph,			///< 有向图
		Undigraph			///< 无向图
	};

	/// 使用邻接表法表示一个图
	template<typename T>
	class GraphicsViaAdjacencyList
	{
	public:
		/// 邻接表上的一个结点
		struct AdjacencyList
		{
			size_t			AimNodeIndex;	///< 目标顶点的编号
			double			Weight;			///< 该条连接边的权值
			AdjacencyList	*Next;			///< 指向下一个结点
		};

	public:
		/// 通过结点来初始化一个图
		GraphicsViaAdjacencyList(vector<T> const &v, GraphicsType type) : _v(v), _e(v.size(), NULL), _type(type)
		{

		}

		~GraphicsViaAdjacencyList()
		{
			for_each(_e.begin(), _e.end(), [](AdjacencyList *e){
				while (e)
				{
					AdjacencyList *temp = e;
					e = e->Next;
					delete temp;
				}
			});
		}


		/// 标识图中两个标点之间有一条边
		void Link2Vertex(size_t index1, size_t index2, double weight = 1)
		{
			AdjacencyList *node = new AdjacencyList();
			node->AimNodeIndex = index2;
			node->Next = _e[index1];
			node->Weight = weight;
			_e[index1] = node;

			if (_type == Undigraph)
			{
				//无向图
				node = new AdjacencyList();
				node->AimNodeIndex = index1;
				node->Next = _e[index2];
				node->Weight = weight;
				_e[index2] = node;
			}
		}

		/// 查询两个顶点是否连接
		/// 
		/// 返回两个顶点是否相连，同时还返回该条边的权值。如果不相连，就返回<false, 0>，这里的权值无意义
		pair<bool, double> IsLinked(size_t index1, size_t index2) const
		{
			AdjacencyList *l = _e[index1];
			while(l)
			{
				if (l->AimNodeIndex == index2)
				{
					return make_pair(true, l->Weight);
				}
				l = l->Next;
			}
			return make_pair(false, 0);
		}

		inline vector<T> & GetVertex()				{return _v;}
		inline vector<AdjacencyList *> & GetEdge()	{return _e;}
		vector<pair<size_t, size_t>> GetAllEdges()
		{
			vector<pair<size_t, size_t>> edges;
			for (size_t i = 0; i < _e.size(); ++i)
			{
				AdjacencyList *l = _e[i];
				while(l)
				{
					edges.push_back(make_pair(i, l->AimNodeIndex));
					l = l->Next;
				}
			}
			return edges;
		}

	
	private:
		//GraphicsViaAdjacencyList(GraphicsViaAdjacencyList<T> const &);
		//GraphicsViaAdjacencyList<T> & operator=(GraphicsViaAdjacencyList<T> &);

		vector<T>				_v;		///< 图的顶点的集合
		vector<AdjacencyList *>	_e;		///< 图的边，即邻接表
		GraphicsType			_type;	///< 图的类型
	};


	/// 使用邻接矩阵来表示一个图
	template<typename VT, typename ET>
	class GrpahicsViaAdjacencyMatrix
	{
	public:
		GrpahicsViaAdjacencyMatrix(vector<VT> const &v, GraphicsType type = Digraph) : _v(v), _e(v.size(), vector<ET>(v.size())), _type(type)
		{

		}

		void Link2Vertex(size_t index1, size_t index2, ET weight)
		{
			_e[index1][index2] = weight;
			if (_type == Undigraph)
			{
				_e[index2][index1] = weight;
			}
		}

		/// 查询两个顶点是否连接
		bool IsLinked(size_t index1, size_t index2) const
		{
			return _e[index1][index2] != ET();
		}

		inline vector<VT> & GetVertex()			{return _v;}
		inline vector<vector<ET>> & GetEdge()	{return _e;}


	
	private:
		vector<VT>				_v;		///< 图的顶点的集合
		vector<vector<ET>>		_e;		///< 图的边，即邻接矩阵
		GraphicsType			_type;	///< 图的类型
	};

}