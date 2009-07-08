#include "expression_list.h"

#include <stdlib.h>

#include "assert.h"
#include "expression.h"

static int
expression_list_real_evaluate (Expression *);

static void
expression_list_real_finalize (void *object);

static ExpressionListClass *klass = NULL;

static const ExpressionClass *parent_class = NULL;

ExpressionListClass *expression_list_class_allocate (size_t size)
{
	assert (size >= sizeof (ExpressionListClass));

	ExpressionListClass *expression_list_class = EXPRESSION_LIST_CLASS (expression_class_allocate (size));
	return expression_list_class;
}

void
expression_list_class_initialize ()
{
	if (klass) // Already initialized
	{
		return;
	}

	klass = expression_list_class_allocate (sizeof (ExpressionListClass));
	if (!klass) //Allocation failed
	{
		return;
	}

	parent_class = expression_class_get ();
	if (!parent_class) // Failed to get the parent class.
	{
		return;
	}

	OBJECT_CLASS (klass)->name = "ExpressionList";
	OBJECT_CLASS (klass)->finalize = expression_list_real_finalize;

	EXPRESSION_CLASS (klass)->evaluate = expression_list_real_evaluate;
}

ExpressionList *expression_list_allocate (size_t size)
{
	assert (size >= sizeof (ExpressionList));

	return EXPRESSION_LIST (expression_allocate (size));
}

void expression_list_initialize (ExpressionList *expression_list, int value)
{
	assert (expression_list);

	expression_initialize (EXPRESSION (expression_list));

	expression_list->value = value;
}

ExpressionList *expression_list_new (int value) {
	ExpressionList *expression_list = expression_list_allocate (sizeof (ExpressionList));
	expression_list_initialize (expression_list, value);
	OBJECT (expression_list)->klass = OBJECT_CLASS (klass);
	return expression_list;
}

static int expression_list_real_evaluate (Expression *expression)
{
	return EXPRESSION_LIST (expression)->value;
}

static void
expression_list_real_finalize (void *object)
{
	// Finalizes if necessary.

	OBJECT_CLASS (parent_class)->finalize (object);
}

