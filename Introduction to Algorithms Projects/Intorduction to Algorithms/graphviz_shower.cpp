#include "stdafx.h"

#include <string>
#include <fstream>
using namespace std;

namespace qi
{
	void ShowGraphvizViaDot(string const &dot)
	{
		ofstream dot_file( "c:\\graphiv_show.dot" );
		dot_file << dot;
		dot_file.close();
		system( "dot -Tpng c:\\graphiv_show.dot -o c:\\graphiv_show.png" );
		system( "c:\\graphiv_show.png" );
	}
}