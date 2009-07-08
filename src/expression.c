#include "expression.h"

#include <stdlib.h>

#include "assert.h"
#include "object.h"

static int
expression_real_evaluate (void *);

static void
expression_real_finalize (void *);


static ExpressionClass *klass = NULL;

ExpressionClass *
expression_class_allocate (size_t size, void *parent_class, char *name)
{
	assert (name);
	assert (size >= sizeof (ExpressionClass));

	ExpressionClass *expression_class = EXPRESSION_CLASS (object_class_allocate (size, parent_class, name));
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
		klass = expression_class_allocate (sizeof (ExpressionClass), (void *) object_class_get (), "Expression");
		return klass;
	}

	return object_class_ref (klass);
}

Expression *
expression_construct (size_t size, void *klass)
{
	assert (size >= sizeof (Expression));

	Expression *self =  EXPRESSION (object_construct (size, klass));

	return self;
}

int
expression_evaluate (void *self)
{
	assert (self);

	return EXPRESSION_GET_CLASS(self)->evaluate (self);
}

static int
expression_real_evaluate (void *self)
{
	return 0;
}

static void
expression_real_finalize (void *self)
{
	// Finalizes if necessary.

	assert (klass);
	OBJECT_CLASS_GET_PARENT (klass)->finalize (self);
}

