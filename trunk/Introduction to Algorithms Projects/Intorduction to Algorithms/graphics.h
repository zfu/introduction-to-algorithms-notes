/// 图的表示法
/// 
/// 图的两种表示方法：（1）邻接表法；（2）邻接矩阵法
/// 


#include <vector>
using std::vector;

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
			T				Item;		///< 顶点的标识
			AdjacencyList	*Next;		///< 指向下一个结点
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
		void Link2Vertex(size_t index1, size_t index2)
		{
			AdjacencyList *node = new AdjacencyList();
			node->Item = _v[index2];
			node->Next = _e[index1];
			_e[index1] = node;

			if (_type == Undigraph)
			{
				//无向图
				node = new AdjacencyList();
				node->Item = _v[index1];
				node->Next = _e[index2];
				_e[index2] = node;
			}
		}

		/// 查询两个顶点是否连接
		bool IsLinked(size_t index1, size_t index2) const
		{
			AdjacencyList *l = _e[index1];
			while(l)
			{
				if (l->Item == _v[index2])
				{
					return true;
				}
				l = l->Next;
			}
			return false;
		}

		inline vector<T> & GetVertex()				{return _v;}
		inline vector<AdjacencyList *> & GetEdge()	{return _e;}

	
	private:
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

		GrpahicsViaAdjacencyMatrix(vector<VT> const &v, vector<vector<ET>> const &e, GraphicsType type = Digraph) : _v(v), _e(e), _type(type)
		{
			assert(e.size() == v.size() && e[0].size() == v.size());
		}

		void Link2Vertex(size_t index1, size_t index2, ET weight)
		{
			_e[index1][index2] = weight;
			if (_type == Undigraph)
			{
				_e[index2][index1] = weight;
			}
		}

	
	private:
		vector<VT>				_v;		///< 图的顶点的集合
		vector<vector<ET>>		_e;		///< 图的边，即邻接矩阵
		GraphicsType			_type;	///< 图的类型
	};

}