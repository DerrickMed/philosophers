#include "phils.h"

char	*ft_itoa(long long n)
{
	long long	nb;
	int			len;
	static char	ret[20];

	len = 2;
	if (n < 0)
		len++;
	nb = n;
	while (!(nb < 10 && nb > -10) && len++)
		nb /= 10;
	ret[--len] = '\0';
	if (n < 0)
		nb = -1;
	else
		nb = 1;
	while (n || len)
	{
		ret[--len] = n % 10 * nb + '0';
		n /= 10;
	}
	if (nb < 0)
		ret[0] = '-';
	return (ret);
}
