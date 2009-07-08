#include "array.h"

#include <stdlib.h>

#include "assert.h"
#include "object.h"

#define INITIAL_CAPACITY 16

static void
array_real_finalize (Object *self);

static void
array_ensure_capacity (Array *self, size_t capacity);

static ArrayClass *klass = NULL;

static const ObjectClass *parent_class = NULL;

ArrayClass *
array_class_allocate (size_t size)
{
	assert (size >= sizeof (ArrayClass));

	ArrayClass *array_class = ARRAY_CLASS (object_class_allocate (size));
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
		array_class_initialize ();
	}

	return klass;
}

void
array_class_initialize ()
{
	if (klass) // Already initialized
	{
		return;
	}

	klass = array_class_allocate (sizeof (ArrayClass));
	if (!klass) //Allocation failed
	{
		return;
	}

	parent_class = object_class_get ();
	if (!parent_class) // Failed to get the parent class.
	{
		return;
	}

	OBJECT_CLASS (klass)->name = "Array";
}

Array *
array_allocate (size_t size)
{
	assert (size >= sizeof (Array));

	return ARRAY (object_allocate (size));
}

void
array_initialize (Array *self, destroy_func_t destroy_func)
{
	assert (self);

	object_initialize (OBJECT (self));

	self->capacity = 0;
	self->length = 0;
	self->array = NULL;
	self->destroy_func = destroy_func;
}

Array *
array_new (destroy_func_t destroy_func) {
	Array *array = array_allocate (sizeof (Array));
	array_initialize (array, destroy_func);
	OBJECT (array)->klass = OBJECT_CLASS (klass);
	return array;
}

size_t
array_get_length (Array *self)
{
	return self->length;
}

void *
array_get (Array *self, size_t index)
{
	assert (index < self->length);

	return self->array[index];
}

void
array_add (Array *self, void *item)
{
	size_t new_length = self->length + 1;
	array_ensure_capacity (self, new_length);

	self->array[self->length] = item;
	self->length = new_length;
}

void
array_set (Array *self, size_t index, void *item)
{
	assert (index < self->length);

	if (self->destroy_func)
	{
		self->destroy_func (self->array[index]);
	}

	self->array[index] = item;
}

void
array_clear (Array *self)
{
	if (self->destroy_func)
	{
		for (size_t i = 0; i < self->length; ++i)
		{
			self->destroy_func (self->array[i]);
		}
	}

	self->length = 0;
}

static void
array_real_finalize (Object *self)
{
	array_clear (ARRAY (self));

	free (ARRAY(self)->array);

	parent_class->finalize (self);
}

static void
array_ensure_capacity (Array *self, size_t capacity)
{
	if (self->capacity >= capacity) // Current capacity is correct
	{
		return;
	}

	size_t new_capacity;
	if (self->capacity == 0)
	{
		new_capacity = INITIAL_CAPACITY;
	}
	else
	{
		new_capacity = self->capacity << 1;
	}

	while (new_capacity < capacity)
	{
		new_capacity <<= 1;
	}

	void **p = (void **) realloc (self->array, sizeof (void *) * new_capacity);
	assert (p);

	self->array = p;
	self->capacity = new_capacity;
}

