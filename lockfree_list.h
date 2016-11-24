#ifndef _LOCKFREE_LIST_H_
#define _LOCKFREE_LIST_H_

#include <iostream>
#include <functional>
#include <vector>
#include <atomic>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

namespace kimgbo
{
	template<typename T>
	class LockFreeQueue
	{
	public:
		LockFreeQueue(int size):head(new Connection()),tail(NULL),task_count(0),task_size(size)
		{
			tail = head;
		}
	
		void EnQueue(const T& val)
		{
			Connection* newVal = new Connection(val);
			Connection* val_new = NULL;
			Connection* old = NULL;
			bool stop = false;
			//int retry = 0;
			
			do
			{
				if(task_count < task_size)
				{
					val_new = tail;
					old = val_new;
				
					if(task_count == 0)
					{
						tail = head;
						val_new = tail;
						old = val_new;
					}
					
					if(val_new->next != NULL)
					{
						while(!__sync_bool_compare_and_swap(&tail, val_new, val_new->next));
						continue;
					}
					
					while(__sync_bool_compare_and_swap(&(val_new->next), NULL, newVal))
					{
						while(!__sync_bool_compare_and_swap(&tail, old, newVal));
						stop = true;
					}
				}
			}while(!stop);
			FetchAndAdd(task_count, 1);
		}
	
		T DeQueue()
		{
			Connection* ret_ptr;
			T ret_val = -1;
			bool stop = false;
			int retry = 0;
			
			do
			{
				ret_ptr = head->next;
				if(task_count > 0 && ret_ptr != NULL)
				{
					while(__sync_bool_compare_and_swap(&(head->next), ret_ptr, ret_ptr->next))
					{
						FetchAndAdd(task_count, -1);
						if(task_count == 0)
						{
							tail = head;
						}
						
						ret_val = ret_ptr->m_val;
						delete ret_ptr;
						stop = true;
					}
				}
				else
				{
					if(retry > 10)
					{
						break;
					}
					
					usleep(5000);
					retry++;
				}
			}while(!stop);
			
			return ret_val;
		}
		
		~LockFreeQueue()
		{
			Connection* temp_delete = NULL;
			while(head->next != tail && head != tail)
			{
				temp_delete = head->next;
				head->next = head->next->next;
				delete temp_delete;
				temp_delete = NULL;
			}
			delete head;
		}
	
	private:
		class Connection
		{
		public:
			Connection(): next(NULL), m_val(0) { }
		
			explicit Connection(const T& num): next(NULL), m_val(num) { }
			
			~Connection() { }
		
			Connection* next;
			T m_val;
		};
		
		int FetchAndAdd(int& val, int x)
		{
			return __sync_fetch_and_add(&val, x);
		}
	
		LockFreeQueue(const LockFreeQueue&) { }
	
		LockFreeQueue& operator=(const LockFreeQueue&) { }
	
	private:
		Connection* head;
		Connection* tail;
		int task_count;
		int task_size;
	};
}

#endif //_LOCKFREE_LIST_H_
