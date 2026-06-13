
#include "../includes/threads.h"


int	main(int argc, char **argv)
{
	int	i;
	int	*data;

	i = 0;
	if (argc != 9)
	{
		printf("Error: Expected 8 arguments, but received %d.\n", argc - 1);
		return (1);
	}
	data = parser(argc - 1, argv);
	if (data == NULL)
		return (1);
	ft_threads(data);
	free(data);
	return (0);
}
