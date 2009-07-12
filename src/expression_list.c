#include "expression_list.h"

#include <stdlib.h>

#include "assert.h"
#include "expression.h"

static int
expression_list_real_evaluate (void *);

static void
expression_list_real_finalize (void *);

static void
expression_list_class_real_finalize (void *);


static ExpressionListClass *klass = NULL;

ExpressionListClass *
expression_list_class_allocate (size_t size, void *parent_class, char *name)
{
	assert (name);
	assert_cmpuint (size, >=, sizeof (ExpressionListClass));

	ExpressionListClass *klass = EXPRESSION_LIST_CLASS (expression_class_allocate (size, parent_class, name));
	if (!klass) // Allocation failed
	{
		return NULL;
	}

	OBJECT_CLASS (klass)->finalize = expression_list_real_finalize;
	EXPRESSION_CLASS (klass)->evaluate = expression_list_real_evaluate;

	return klass;
}

ExpressionListClass *
expression_list_class_get ()
{
	if (!klass) // The ExpressionList class is not yet initalized.
	{
		klass = expression_list_class_allocate (sizeof (ExpressionListClass), (void *) expression_class_get (), "ExpressionList");
		OBJECT_CLASS (klass)->finalize_class = expression_list_class_real_finalize;
		return klass;
	}

	return object_class_ref (klass);
}

ExpressionList *
expression_list_construct (size_t size, void *klass)
{
	assert_cmpuint (size, >=, sizeof (ExpressionList));

	ExpressionList *self =  EXPRESSION_LIST (expression_construct (size, klass));

	return self;
}

static int
expression_list_real_evaluate (void *self)
{
	return ((int) self) % 100;
}

static void
expression_list_real_finalize (void *self)
{
	// Finalizes if necessary.

	assert (klass);
	object_class_get_parent (klass)->finalize (self);
}

static void
expression_list_class_real_finalize (void *_klass)
{
	assert (_klass == klass);
	klass = NULL;
}

