#include "array.h"

#include <stdlib.h>

#include "assert.h"
#include "debug.h"
#include "object.h"

#define INITIAL_CAPACITY 16

static void
array_real_finalize (void *);

static ArrayClass *klass = NULL;

ArrayClass *
array_class_allocate (size_t size, void *parent_class, char *name)
{
	assert (name);
	assert (size >= sizeof (ArrayClass));

	ArrayClass *array_class = ARRAY_CLASS (object_class_allocate (size, parent_class, name));
	if (!array_class) // Allocation failed
	{
		return NULL;
	}

	OBJECT_CLASS (array_class)->finalize = array_real_finalize;

	return array_class;
}

const ArrayClass *
array_class_get ()
{
	if (!klass) // The Object class is not yet initalized.
	{
		klass = array_class_allocate (sizeof (ArrayClass), (void *) object_class_get (), "Array");
		return klass;
	}

	return object_class_ref (klass);
}

Array *
array_construct (size_t size, void *klass, destroy_func_t destroy_func)
{
	assert (size >= sizeof (Array));

	Array *self =  ARRAY (object_construct (size, klass));

	self->capacity = 0;
	self->length = 0;
	self->array = NULL;
	self->destroy_func = destroy_func;

	return self;
}

void
array_add (void *self, void *item)
{
	size_t new_length = ARRAY (self)->length + 1;
	array_ensure_capacity (self, new_length);

	ARRAY (self)->array[ARRAY (self)->length] = item;
	ARRAY (self)->length = new_length;
}

void
array_set (void *self, size_t index, void *item)
{
	assert (index < ARRAY (self)->length);

	if (ARRAY (self)->destroy_func)
	{
		ARRAY (self)->destroy_func (ARRAY (self)->array[index]);
	}

	ARRAY (self)->array[index] = item;
}

void
array_clear (void *self)
{
	if (ARRAY (self)->destroy_func)
	{
		for (size_t i = 0; i < ARRAY (self)->length; ++i)
		{
			ARRAY (self)->destroy_func (ARRAY (self)->array[i]);
		}
	}

	ARRAY (self)->length = 0;
}

void
array_ensure_capacity (void *self, size_t capacity)
{
	if (ARRAY (self)->capacity >= capacity) // Current capacity is correct
	{
		return;
	}

	size_t new_capacity;
	if (ARRAY (self)->capacity == 0)
	{
		new_capacity = INITIAL_CAPACITY;
	}
	else
	{
		new_capacity = ARRAY (self)->capacity << 1;
	}

	while (new_capacity < capacity)
	{
		new_capacity <<= 1;
	}

	void **p = (void **) realloc (ARRAY (self)->array, sizeof (void *) * new_capacity);
	assert (p);

	ARRAY (self)->array = p;
	ARRAY (self)->capacity = new_capacity;

	debug ("New Array capacity: %u", new_capacity);
}

static void
array_real_finalize (void *self)
{
	array_clear (ARRAY (self));

	free (ARRAY(self)->array);

	assert (klass);
	OBJECT_CLASS_GET_PARENT (klass)->finalize (self);
}

