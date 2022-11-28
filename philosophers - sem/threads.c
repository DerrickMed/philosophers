#include "phils.h"

void	*mondeath(void *philo)
{
	t_phs	*ph;

	ph = (t_phs *)philo;
	while (1 && !ph->data->stop)
	{
		if (ft_time() > ph->time_to_die && ph->eat_num
			 != ph->data->eat_num && !ph->data->stop && !ph->iseating)
		{
			ph->dead = 1;
			ph->data->stop = 1;
			printer("dead", ph, 3);
			usleep(1000);
			sem_post(ph->data->exit);
			return (NULL);
		}
		if (ph->data->eat_num != -1 && ph->eat_num
			 == ph->data->eat_num && !ph->data->stop)
			moneat(ph->data);
		usleep(25);
	}
	return (NULL);
}

void	printer(char *msg, t_phs *philo, int state)
{
	if (philo->data->stop == 1 && state != 3)
		return ;
	sem_wait(philo->data->print);
	if (philo->data->stop == 1 && state != 3)
	{
		sem_post(philo->data->print);
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
		sem_post(philo->data->exit);
		return ;
	}
	sem_post(philo->data->print);
}

void	moneat(t_data *data)
{
	if (data->eat_num_sem->__align == data->total_num && !data->stop)
	{
		sem_wait(data->print);
		if (data->stop == 1)
		{
			sem_post(data->print);
			return ;
		}
		data->stop = 1;
		sem_post(data->print);
		sem_wait(data->allfed);
		usleep(1000);
		printf("all fed\n");
		sem_post(data->exit);
		return ;
	}
	return ;
}

void	forks_lock_unlock(t_phs *ph, t_bool y)
{
	if (y && !ph->data->stop)
	{
		sem_wait(ph->data->forks);
		sem_wait(ph->data->forks);
		ph->iseating = 1;
		if (ph->data->stop == 1)
		{
			sem_post(ph->data->forks);
			sem_post(ph->data->forks);
			return ;
		}
		printer("has taken a fork", ph, 0);
		printer("has taken a fork", ph, 0);
	}
	else if (!y)
	{
		ph->iseating = 0;
		sem_post(ph->data->forks);
		sem_post(ph->data->forks);
		printer("is sleeping", ph, 0);
	}
}

void	*philosophers(t_phs *ph)
{
	pthread_t	thr;

	pthread_create(&thr, NULL, &mondeath, ph);
	pthread_detach(thr);
	while (1 && ph->eat_num != ph->data->eat_num && !ph->data->stop)
	{
		sem_wait(ph->data->odd_lock);
		forks_lock_unlock(ph, 1);
		printer("is eating", ph, 1);
		sem_post(ph->data->odd_lock);
		if (ph->data->stop)
			break ;
		ph->time_to_die = ft_time() + ph->data->die;
		usleep(ph->data->eat * 1000);
		ph->eat_num++;
		forks_lock_unlock(ph, 0);
		usleep(ph->data->sleep * 1000);
		if (ph->data->stop == 1)
			return ((void *)0);
		printer("is thinking", ph, 0);
	}
	sem_post(ph->data->eat_num_sem);
	sem_wait(ph->data->exit);
	sem_post(ph->data->exit);
	return (NULL);
}
