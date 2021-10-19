#include "stack.h"


/**
 * @brief:		将数据入栈
 * @param:		none
 * @retval:		none
 * @author:		Brimon
 */
int stack_push(STACK *stack, unsigned char data)
{
	if(stack->stack_top >= 99) return 0;
	else {
		stack->stack[stack->stack_top] = data;
		stack->stack_top++;
		return 1;
	}
}


/**
 * @brief:		将数据弹出栈
 * @param:		none
 * @retval:		none
 * @author:		Brimon
 */
int stack_pop(STACK *stack)
{
	if(stack->stack_top == 0) return 0;
	else {
		stack->stack[stack->stack_top] = 0;
		stack->stack_top--;
		return 1;
	}
}

/**
 * @brief:		获取栈顶的数据
 * @param:		none
 * @retval:		none
 * @author:		Brimon
 */
int stack_read_top(STACK *stack)
{
	return stack->stack[stack->stack_top];
}
