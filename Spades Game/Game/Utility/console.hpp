#ifndef CGE_CONSOLE_HPP
#define CGE_CONSOLE_HPP
#include <iostream>
	typedef std::ostream& (*manip) (std::ostream&);
	class console {
	public:
		bool isOn;
		console()
			: isOn(true)
		{

		}

		console(bool on)
			: isOn(on)
		{

		}
	};

	template <class T> inline console& operator<< (console& con, const T& x) { if(con.isOn) std::cout << x; return con; }
	template <class T> inline console& operator>>(console& con,  T& x) { if(con.isOn) std::cin >>x; return con; }
	inline console& operator<< (console& con, manip manipulator){ if(con.isOn) std::cout<<manipulator; return con;}
#endif