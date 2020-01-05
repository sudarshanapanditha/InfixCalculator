#include "token.hpp"
#include "stack.hpp"
#include <cassert>

stack::stack()
     /*  Construct a new stack
	 Pre-conditions : None
	 Post-condition : The stack is Empty */
{
  top = 0;
}

stack::~stack()
     /* Deallocate the stack s
	Pre-condition : stack is initialised
	Post-condition : All storage associated with the stack has
	been deallocated */
{
}
  
Object stack::Pop()
     /* Removes the top of a stack
	Pre-condition : stack is initialised
	                stack is not empty
	Post-condition : Item which was added most recently to 
	                 stack has been removed */
	
{
  assert(!isEmpty());
  return items[--(top)];
}


Object stack::Top() const
     /* Retrieves the top of a stack.
	Pre-condition : stack is initialised
	                s is not empty
	Post-condition : s is unchanged */
{
  assert(!isEmpty());
  return items[top-1];
}

bool stack::isEmpty() const
     /* Determines whether a stack is empty
	Pre-condition : stack is initialised
	Post-condition : returns true is s is empty, false otherwise */
{
  return (top == 0);
}

void stack::Push(Object item)
     /* Adds an item to the top of a stack.
	Pre-condition : stack is initialised
	Post-condition :  item is on the top of the stack s */
{
  assert(top!=MAXSTACK);
  items[top++] = item;
}
  
