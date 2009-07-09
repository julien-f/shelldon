#include "string.h"

#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "debug.h"
#include "object.h"

#define INITIAL_CAPACITY 16

static void
string_real_finalize (void *);

static void
string_class_real_finalize (void *);

static StringClass *klass = NULL;

StringClass *
string_class_allocate (size_t size, void *parent_class, char *name)
{
	assert (name);
	assert (size >= sizeof (StringClass));

	StringClass *string_class = STRING_CLASS (object_class_allocate (size, parent_class, name));
	if (!string_class) // Allocation failed
	{
		return NULL;
	}

	OBJECT_CLASS (string_class)->finalize = string_real_finalize;

	return string_class;
}

StringClass *
string_class_get ()
{
	if (!klass) // The Object class is not yet initalized.
	{
		klass = string_class_allocate (sizeof (StringClass), object_class_get (), "String");
		OBJECT_CLASS (klass)->finalize_class = string_class_real_finalize;
		return klass;
	}

	return object_class_ref (klass);
}

String *
string_construct (size_t size, void *klass)
{
	assert (size >= sizeof (String));

	String *self =  STRING (object_construct (size, klass));

	self->capacity = INITIAL_CAPACITY;
	self->length = 0;
	self->string = (char *) malloc (sizeof (char) * INITIAL_CAPACITY);
	assert (self->string);
	self->string[0] = 0;

	return self;
}

void
string_append_n (void *self, const char *str, size_t n)
{
	assert (self);
	assert (str);

	if (!*str) // The string is empty.
	{
		return;
	}


	size_t new_length = STRING (self)->length + n;
	string_ensure_capacity (self, new_length + 1);

	memcpy (STRING (self)->string + STRING (self)->length, str, n);

	STRING (self)->length = new_length;

	STRING (self)->string[STRING (self)->length] = 0;
}

void
string_append_char (void *self, char c)
{
	assert (self);

	if (!c) // Null character.
	{
		return;
	}

	size_t new_length = STRING (self)->length + 1;
	string_ensure_capacity (self, new_length + 1);

	STRING (self)->string[STRING (self)->length] = c;
	STRING (self)->length = new_length;
	STRING (self)->string[STRING (self)->length] = 0;
}

void
string_ensure_capacity (void *self, size_t capacity)
{
	if (STRING (self)->capacity >= capacity) // Current capacity is correct
	{
		return;
	}

	assert (STRING (self)->capacity);
	size_t new_capacity = STRING (self)->capacity << 1;

	while (new_capacity < capacity)
	{
		new_capacity <<= 1;
	}

	char *p = (char *) realloc (STRING (self)->string, sizeof (char) * new_capacity);
	assert (p);

	STRING (self)->string = p;
	STRING (self)->capacity = new_capacity;

	debug ("New String capacity: %u", new_capacity);
}

static void
string_real_finalize (void *self)
{
	free (STRING (self)->string);

	assert (klass);
	OBJECT_CLASS_GET_PARENT (klass)->finalize (self);
}

static void
string_class_real_finalize (void *_klass)
{
	assert (_klass == klass);
	klass = NULL;
}
