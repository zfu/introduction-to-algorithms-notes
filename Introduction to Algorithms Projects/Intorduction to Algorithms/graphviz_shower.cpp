//////////////////////////////////////////////////////////////////////////
/// @file		graphviz_shower.cpp
/// @brief		调用Graphviz显示dot语言所描述的图
/// @details	COPYRIGHT NOTICE
///			    Copyright (c) 2011
///			    All rights reserved.\n
///
///
/// @author		谭川奇	chuanqi.tan(at)gmail.com
/// @date		2011/06/17
/// @version	1.0
//////////////////////////////////////////////////////////////////////////
/// 修改记录：
/// 2011/06/17	1.0	谭川奇	创建

#include <string>
#include <fstream>
using namespace std;

namespace qi
{
    void ShowGraphvizViaDot( std::string const &dot )
    {
        ofstream dot_file( "c:\\graphiv_show.dot" );
        dot_file << dot;
        dot_file.close();
        system( "dot -Tpng c:\\graphiv_show.dot -o c:\\graphiv_show.png" );
        system( "c:\\graphiv_show.png" );
    }
}