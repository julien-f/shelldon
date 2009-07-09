#include "object.h"

#include <stdlib.h>
#include <string.h>

#include "assert.h"
//#define DISABLE_DEBUG
#include "debug.h"

static void
object_real_finalize (void *self);

static void
object_class_real_finalize (void *klass);

static ObjectClass *klass = NULL;

ObjectClass *
object_class_allocate (size_t size, void *parent_class, char *name)
{
	assert (name);
	assert_cmpuint (size, >=, sizeof (ObjectClass));

	debug ("Class creation: %s", name);

	ObjectClass *object_class = (ObjectClass *) malloc (size);
	if (!object_class) // Allocation failed
	{
		return NULL;
	}

	object_class->parent_class = parent_class;
	object_class->name = name;
	object_class->ref_count = 1;
	object_class->finalize = object_real_finalize;
	object_class->finalize_class = NULL;

	return object_class;
}

ObjectClass *
object_class_get ()
{
	if (!klass) // The Object class is not yet initalized.
	{
		klass = object_class_allocate (sizeof (ObjectClass), NULL, "Object");
		klass->finalize_class = object_class_real_finalize;
		return klass;
	}
	return object_class_ref (klass);
}

bool
object_class_is_a (const void *klass, const void *name)
{
	assert (klass);
	assert (name);

	if (strcmp (OBJECT_CLASS (klass)->name, name) == 0)
	{
		return true;
	}
	if (OBJECT_CLASS_GET_PARENT (klass))
	{
		return object_class_is_a (OBJECT_CLASS_GET_PARENT (klass), name);
	}
	return false;
}

void *
object_class_ref (void *klass)
{
	assert (klass);
	assert (OBJECT_CLASS (klass)->ref_count > 0);

	++(OBJECT_CLASS (klass)->ref_count);

	return klass;
}

void
object_class_unref (void *klass)
{
	assert (klass);
	assert (OBJECT_CLASS (klass)->ref_count > 0);

	--(OBJECT_CLASS (klass)->ref_count);

	if (OBJECT_CLASS (klass)->ref_count == 0)
	{
		debug ("Class deletion: %s", OBJECT_CLASS (klass)->name);

		if (OBJECT_CLASS (klass)->finalize_class)
		{
			OBJECT_CLASS (klass)->finalize_class (klass);
		}

		if (OBJECT_CLASS_GET_PARENT (klass))
		{
			object_class_unref (OBJECT_CLASS_GET_PARENT (klass));
		}
		free (klass);
	}
}

Object *
object_construct (size_t size, void *klass)
{
	assert (klass);
	assert_cmpuint (size, >=, sizeof (Object));

	debug ("Instance creation: %s", OBJECT_CLASS (klass)->name);

	Object *self = (Object *) malloc (size);
	if (!self) // Allocation failed.
	{
		return NULL;
	}

	self->ref_count = 1;
	self->klass = klass;

	return self;
}

const char *
object_get_class_name (const void *self)
{
	assert (self);

	return OBJECT_GET_CLASS (self)->name;
}

void *
object_ref (void *self)
{
	assert (self);
	assert (OBJECT (self)->ref_count > 0);

	++(OBJECT (self)->ref_count);

	return self;
}

void
object_unref (void *self)
{
	assert (self);
	assert (OBJECT (self)->ref_count > 0);

	--(OBJECT (self)->ref_count);

	if (OBJECT (self)->ref_count == 0)
	{
		OBJECT_GET_CLASS (self)->finalize (self);

		free (self);
	}
}

static void
object_real_finalize (void *self)
{
	debug ("Instance deletion: %s", object_get_class_name (self));
	object_class_unref (OBJECT_GET_CLASS (self));
}

static void
object_class_real_finalize (void *_klass)
{
	assert (_klass == klass);
	klass = NULL;
}

