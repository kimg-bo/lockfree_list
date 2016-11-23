#ifndef _CONNOBJ_H_
#define _CONNOBJ_H_

#include <iostream>

namespace kimgbo
{
	class Connection
	{
	public:
		Connection(): next(NULL), m_val(0)
		{
		}
		
		explicit Connection(const int num): next(NULL), m_val(num)
		{
		}
		
		~Connection()
		{
		}
		
		void execute()
		{
			std::cout<<"execute "<<m_val<<"...\n";
		}
		
		Connection* next;
	private:
		int m_val;
	};
}

#endif //_CONNOBJ_H_