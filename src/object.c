#include "object.h"

#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "debug.h"

static void
object_real_finalize (void *self);

static void
object_class_real_finalize (void *klass);

static ObjectClass *klass = NULL;

ObjectClass *
object_class_allocate (size_t size, void *parent, char *name)
{
	assert (name);
	assert_cmpuint (size, >=, sizeof (ObjectClass));

	debug ("Class creation: %s", name);

	ObjectClass *object_class = malloc (size);
	if (!object_class) // Allocation failed
	{
		return NULL;
	}

	object_class->parent = parent;
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

	void *parent = object_class_get_parent (klass);
	if (parent)
	{
		return object_class_is_a (parent, name);
	}
	return false;
}

void
object_class_unref (void *klass)
{
	assert (klass);
	assert (OBJECT_CLASS (klass)->ref_count);

	--(OBJECT_CLASS (klass)->ref_count);

	if (OBJECT_CLASS (klass)->ref_count == 0)
	{
		debug ("Class deletion: %s", OBJECT_CLASS (klass)->name);

		if (OBJECT_CLASS (klass)->finalize_class)
		{
			OBJECT_CLASS (klass)->finalize_class (klass);
		}

		void *parent = object_class_get_parent (klass);
		if (parent)
		{
			object_class_unref (parent);
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

void
object_unref (void *self)
{
	assert (self);
	assert (OBJECT (self)->ref_count);

	--(OBJECT (self)->ref_count);

	if (OBJECT (self)->ref_count == 0)
	{
		OBJECT_CLASS (object_get_class (self))->finalize (self);

		free (self);
	}
}

static void
object_real_finalize (void *self)
{
	debug ("Instance deletion: %s", object_get_class_name (self));
	object_class_unref (object_get_class (self));
}

static void
object_class_real_finalize (void *_klass)
{
	assert (_klass == klass);
	klass = NULL;
}

