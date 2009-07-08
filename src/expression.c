#include "expression.h"

#include <stdlib.h>

#include "assert.h"
#include "object.h"

static int
expression_real_evaluate (Expression *);

static void
expression_real_finalize (void *object);


static ExpressionClass *klass = NULL;

static const ObjectClass *parent_class = NULL;

ExpressionClass *
expression_class_allocate (size_t size)
{
	assert (size >= sizeof (ExpressionClass));

	ExpressionClass *expression_class = EXPRESSION_CLASS (object_class_allocate (size));
	if (!expression_class) // Allocation failed
	{
		return NULL;
	}

	OBJECT_CLASS (expression_class)->finalize = expression_real_finalize;
	expression_class->evaluate = expression_real_evaluate;

	return expression_class;
}

const ExpressionClass *
expression_class_get ()
{
	if (!klass) // The Object class is not yet initalized.
	{
		expression_class_initialize ();
	}

	return klass;
}

void
expression_class_initialize ()
{
	if (klass) // Already initialized
	{
		return;
	}

	klass = expression_class_allocate (sizeof (ExpressionClass));
	if (!klass) //Allocation failed
	{
		return;
	}

	parent_class = object_class_get ();
	if (!parent_class) // Failed to get the parent class.
	{
		return;
	}

	OBJECT_CLASS (klass)->name = "Expression";
}

Expression *
expression_allocate (size_t size)
{
	assert (size >= sizeof (Expression));

	return EXPRESSION (object_construct (size));
}

void
expression_initialize (Expression *expression)
{
	assert (expression);
}

Expression *
expression_new () {
	Expression *expression = expression_allocate (sizeof (Expression));
	expression_initialize (expression);
	OBJECT (expression)->klass = OBJECT_CLASS (klass);
	return expression;
}

int
expression_evaluate (Expression *expression)
{
	assert (expression);

	return EXPRESSION_GET_CLASS(expression)->evaluate (expression);
}

static int
expression_real_evaluate (Expression *expression)
{
	return 0;
}

static void
expression_real_finalize (void *object)
{
	// Finalizes if necessary.

	parent_class->finalize (object);
}

