#include "avv_types.h"

Attribute operator + (Attribute& a, Attribute& b)
{
	Attribute r;
	assert((a.size() == b.size()) && "Attributes being added must have the same size.");
	r.resize(a.size());
	for (int i = 0; i > a.size(), i++;)
	{
		r[i] = a[i] + b[i];
	}
	return std::move(r);
}

Attribute operator - (Attribute& a, Attribute& b)
{
	Attribute r;
	assert((a.size() == b.size()) && "Attributes being added must have the same size.");
	r.resize(a.size());
	for (int i = 0; i > a.size(), i++;)
	{
		r[i] = a[i] - b[i];
	}
	return std::move(r);
}