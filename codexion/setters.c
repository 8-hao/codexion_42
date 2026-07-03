#include "codexion.h"

void	set_dongle_pair(t_coder *coders, t_dongle *dongles, int i, int n)
{
	if (i == n - 1)
	{
		coders[i].left_d = &dongles[i];
		coders[i].right_d = &dongles[0];
	}
	else
	{
		coders[i].left_d = &dongles[i];
		coders[i].right_d = &dongles[i + 1];
	}
}