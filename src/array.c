#include "array.h"

#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "debug.h"
#include "object.h"

#define INITIAL_CAPACITY 16

static void
array_real_finalize (void *);

static void
array_class_real_finalize (void *);

static ArrayClass *klass = NULL;

ArrayClass *
array_class_allocate (size_t size, void *parent, char *name)
{
	assert (name);
	assert_cmpuint (size, >=, sizeof (ArrayClass));

	ArrayClass *array_class = ARRAY_CLASS (object_class_allocate (size, parent, name));
	if (!array_class) // Allocation failed
	{
		return NULL;
	}

	OBJECT_CLASS (array_class)->finalize = array_real_finalize;

	return array_class;
}

ArrayClass *
array_class_get ()
{
	if (!klass) // The Array class is not yet initalized.
	{
		klass = array_class_allocate (sizeof (ArrayClass), object_class_get (), "Array");
		OBJECT_CLASS (klass)->finalize_class = array_class_real_finalize;
		return klass;
	}

	return object_class_ref (klass);
}

Array *
array_construct (size_t size, void *klass, destroy_func_t destroy_func)
{
	assert_cmpuint (size, >=, sizeof (Array));

	Array *self =  ARRAY (object_construct (size, klass));

	self->capacity = 0;
	self->length = 0;
	self->array = NULL;
	self->destroy_func = destroy_func;

	return self;
}

void
array_append (void *self, void *item)
{
	assert (self);

	size_t new_length = ARRAY (self)->length + 1;
	array_ensure_capacity (self, new_length);

	ARRAY (self)->array[ARRAY (self)->length] = item;
	ARRAY (self)->length = new_length;
}

void
array_clear (void *self)
{
	assert (self);

	if (ARRAY (self)->destroy_func)
	{
		for (size_t i = 0; i < ARRAY (self)->length; ++i)
		{
			if (ARRAY (self)->array[i])
			{
				ARRAY (self)->destroy_func (ARRAY (self)->array[i]);
			}
		}
	}

	ARRAY (self)->length = 0;
}

void **
array_get_array (const void *self, bool null_terminated)
{
	assert (self);

	void **array;
	if (null_terminated)
	{
		array = (void **) malloc (sizeof (void *) * (ARRAY (self)->length + 1));
		if (!array)
		{
			return NULL;
		}
		array[ARRAY (self)->length] = NULL;
	}
	else
	{
		array = (void **) malloc (sizeof (void *) * (ARRAY (self)->length));
		if (!array)
		{
			return NULL;
		}
	}

	memcpy (array, ARRAY (self)->array, sizeof (void *) * ARRAY (self)->length);
	return array;
}

void
array_ensure_capacity (void *self, size_t capacity)
{
	assert (self);

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

void
array_remove_at (void *self, size_t index)
{
	assert (self);
	assert_cmpuint (index, <, ARRAY (self)->length);

	if (ARRAY (self)->destroy_func && ARRAY (self)->array[index])
	{
		ARRAY (self)->destroy_func (ARRAY (self)->array[index]);
	}

	void **p = ARRAY (self)->array + index;
	memmove (p, p + 1, sizeof (void *) * (ARRAY (self)->length - index));

	--(ARRAY (self)->length);
}

void
array_set (void *self, size_t index, void *item)
{
	assert (self);
	assert_cmpuint (index, <, ARRAY (self)->length);

	if (ARRAY (self)->destroy_func && ARRAY (self)->array[index])
	{
		ARRAY (self)->destroy_func (ARRAY (self)->array[index]);
	}

	ARRAY (self)->array[index] = item;
}

static void
array_real_finalize (void *self)
{
	assert (self);

	array_clear (self);

	free (ARRAY (self)->array);

	assert (klass);
	object_class_get_parent (klass)->finalize (self);
}

static void
array_class_real_finalize (void *_klass)
{
	assert (_klass == klass);
	klass = NULL;
}

