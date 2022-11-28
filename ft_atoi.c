#include "phils.h"

static int	ft_isspace(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

long	ft_time(void)
{
	struct timeval	i;

	gettimeofday(&i, NULL);
	return (i.tv_sec * 1000 + i.tv_usec / 1000);
}

int	ft_atoi(const char *str)
{
	int				sign;
	long long int	res;
	long long int	res_tmp;

	res = 0;
	sign = 1;
	while (*str && ft_isspace(*str))
		str++;
	if (*str == '-')
		sign = -1;
	if (*str == '+' || *str == '-')
		str++;
	while (*str && (*str >= 48 && *str <= 57))
	{
		res_tmp = res;
		res = res * 10 + (*str++ - '0') * sign;
		if (res < res_tmp && sign == 1)
			return (-1);
		else if (res > res_tmp && sign == -1)
			return (0);
	}
	return ((int)res);
}
