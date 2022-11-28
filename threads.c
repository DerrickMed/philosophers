#include "phils.h"

void	*mondeath(void *philo)
{
	t_phs	*ph;

	ph = (t_phs *)philo;
	while (1 && !ph->data->stop)
	{
		usleep(100);
		if (ph->data->eat_num != -1 && !ph->data->stop)
			moneat(ph->data);
		pthread_mutex_lock(&ph->omnomnom);
		if (ft_time() > ph->time_to_die && ph->eat_num != ph->data->eat_num
			&& !ph->data->stop)
		{
			ph->dead = 1;
			ph->data->stop = 1;
			printer("dead", ph, 3);
			usleep(10000);
			pthread_mutex_unlock(&ph->omnomnom);
			return (NULL);
		}
		pthread_mutex_unlock(&ph->omnomnom);
	}
	return (NULL);
}

void	printer(char *msg, t_phs *philo, int state)
{
	if (philo->data->stop == 1 && state != 3)
		return ;
	pthread_mutex_lock(&philo->data->print);
	if (philo->data->stop == 1 && state != 3)
	{
		pthread_mutex_unlock(&philo->data->print);
		return ;
	}
	if (state == 0)
		printf("%ld %d %s\n", ft_time() - philo->thread_start,
			philo->num + 1, msg);
	else if (state == 1)
		printf("%ld %d %s %dth time\n", ft_time() - philo->thread_start,
			philo->num + 1, msg, philo->eat_num + 1);
	else if (state == 3)
	{
		printf("%ld %d died\n", ft_time() - philo->thread_start, philo->num + 1);
		pthread_mutex_unlock(&philo->data->exit);
	}
	pthread_mutex_unlock(&philo->data->print);
}

void	moneat(t_data *data)
{
	int	i;
	int	allfed;

	allfed = 1;
	i = -1;
	while (allfed && ++i < data->total_num && !data->stop)
		if (data->phs[i].eat_num < data->eat_num)
			allfed = 0;
	if (allfed == 1 && !data->stop)
	{
		pthread_mutex_lock(&data->print);
		if (data->stop == 1)
		{
			pthread_mutex_unlock(&data->print);
			return ;
		}
		data->stop = 1;
		printf("all fed\n");
		pthread_mutex_unlock(&data->print);
		usleep(10000);
		pthread_mutex_unlock(&data->exit);
	}
}

void	forks_lock_unlock(t_phs *ph, t_bool y)
{
	if (y && !ph->data->stop)
	{
		pthread_mutex_lock(&ph->data->forks[ph->right_fork]);
		if (ph->data->stop == 1)
		{
			pthread_mutex_unlock(&ph->data->forks[ph->right_fork]);
			return ;
		}
		pthread_mutex_lock(&ph->data->forks[ph->left_fork]);
		if (ph->data->stop == 1)
		{
			pthread_mutex_unlock(&ph->data->forks[ph->left_fork]);
			return ;
		}
		printer("has taken a fork", ph, 0);
		printer("has taken a fork", ph, 0);
	}
	else
	{
		pthread_mutex_unlock(&ph->data->forks[ph->left_fork]);
		pthread_mutex_unlock(&ph->data->forks[ph->right_fork]);
		printer("is sleeping", ph, 0);
	}
}

void	*philosophers(void *philosopher)
{
	t_phs		*ph;
	pthread_t	thr;

	ph = (t_phs *)philosopher;
	pthread_create(&thr, NULL, &mondeath, philosopher);
	pthread_detach(thr);
	while (1 && ph->eat_num != ph->data->eat_num && !ph->data->stop)
	{
		forks_lock_unlock(ph, 1);
		ph->time_to_die = ft_time() + ph->data->die;
		pthread_mutex_lock(&ph->omnomnom);
		if (ph->data->stop)
			break ;
		printer("is eating", ph, 1);
		usleep(ph->data->eat * 1000);
		pthread_mutex_unlock(&ph->omnomnom);
		ph->eat_num++;
		forks_lock_unlock(ph, 0);
		usleep(ph->data->sleep * 1000);
		if (ph->data->stop == 1)
			return ((void *)0);
		printer("is thinking", ph, 0);
	}
	pthread_mutex_unlock(&ph->omnomnom);
	return (NULL);
}
