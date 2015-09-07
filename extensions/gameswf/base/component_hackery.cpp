// Just some messing around.
#if 0

#include <vector>


class Component {
//	typedef vector<Component>::iterator;

	template< class interface >
	class iterator {


		operator++()
		{
		}
	};
};


class SpecialInterface : public Component {
	void	DoStuff()
	{
		printf( "doing stuff.\n" );
	}
};


class Actor {
	
};


#define foreach( _decl, _p, _actor )	\
	for ( _decl _p, Component::iterator _i( _p ) = _actor.components.get_first( _p ); _i != _actor.components.end(); ++ _i, _p = *_i )



foreach(SpecialInterface*, p, actors)
{
	p->DoStuff();
	some_list.push_back( p );
}


#endif // 0
