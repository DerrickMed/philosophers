#include "phils.h"

void	ft_free(t_data *data)
{
	int	i;

	pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->exit);
	i = -1;
	while (++i < data->total_num)
	{
		pthread_mutex_destroy(&data->phs[i].omnomnom);
		pthread_mutex_destroy(&data->forks[i]);
	}
	free(data->forks);
	free(data->phs);
}

void	start_threads(t_data *data)
{
	int			i;
	pthread_t	thread;

	i = 0;
	data->start = ft_time();
	while (i < data->total_num)
	{
		data->phs[i].thread_start = data->start;
		data->phs[i].hungry = data->start;
		data->phs[i].time_to_die = data->phs[i].hungry + data->die;
		pthread_create(&thread, NULL, &philosophers, &data->phs[i]);
		pthread_detach(thread);
		usleep(50);
		i += 2;
	}
	usleep(50);
	i = 1;
	while (i < data->total_num)
	{
		data->phs[i].thread_start = data->start;
		data->phs[i].hungry = data->start;
		data->phs[i].time_to_die = data->phs[i].hungry + data->die;
		pthread_create(&thread, NULL, &philosophers, &data->phs[i]);
		pthread_detach(thread);
		usleep(50);
		i += 2;
	}
}

void	init(t_data *data, int i)
{
	while (++i < data->total_num)
	{
		data->phs[i].left_fork = i % data->total_num;
		data->phs[i].right_fork = (i + 1) % data->total_num;
		data->phs[i].data = data;
		data->phs[i].num = i;
		data->phs[i].dead = 0;
		data->phs[i].eat_num = 0;
		data->phs[i].hungry = 0;
		pthread_mutex_init(&data->forks[i], NULL);
		pthread_mutex_init(&data->phs[i].omnomnom, NULL);
	}
	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->exit, NULL);
	pthread_mutex_lock(&data->exit);
}

int	parser(int ac, char **av, t_data *data)
{
	int		i;

	i = -1;
	data->eat_num = -1;
	data->total_num = ft_atoi(av[1]);
	data->die = ft_atoi(av[2]);
	data->eat = ft_atoi(av[3]);
	data->sleep = ft_atoi(av[4]);
	if (ac == 6)
		data->eat_num = ft_atoi(av[5]);
	if (data->die < 60 || data->eat < 60 || data->sleep < 60
		|| data->total_num > 200 || data->total_num <= 0 || data->eat_num < -1)
		return (1);
	data->stop = 0;
	data->phs = malloc(sizeof(t_phs) * data->total_num);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->total_num);
	if (data->phs == NULL || data->forks == NULL)
		return (1);
	init(data, i);
	return (0);
}

int	main(int ac, char **av)
{
	t_data		data;

	if (ac < 5 || ac > 6)
		return (printf("Arguments are incorrect\n"));
	if (parser(ac, av, &data))
		return (printf("Arguments are incorrect\n"));
	start_threads(&data);
	pthread_mutex_lock(&data.exit);
	pthread_mutex_unlock(&data.exit);
	data.stop = 1;
	usleep((data.sleep + data.eat) * 1100);
	ft_free(&data);
	return (0);
}
