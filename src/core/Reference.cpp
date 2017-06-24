#include "Reference.h"

#include "Memory.h"

Referenced::Referenced()
{
	ref_count = 0;
}

void Referenced::increase_ref_count()
{
	ref_count++;
}

void Referenced::decrease_ref_count()
{
	ref_count--;

	if (ref_count == 0)
		GC->queue_clean(this);
}
