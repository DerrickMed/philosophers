#include "phils.h"

int	ft_free(t_data *data)
{
	free(data->phs);
	sem_close(data->exit);
	sem_close(data->print);
	sem_close(data->odd_lock);
	sem_close(data->forks);
	sem_close(data->eat_num_sem);
	sem_close(data->allfed);
	return (0);
}

int	start_forks(t_data *data)
{
	static int		i;

	data->start = ft_time();
	while (i < data->total_num)
	{
		data->phs[i].pid = fork();
		if (data->phs[i].pid < 0)
			return (1);
		else if (data->phs[i].pid == 0)
		{
			data->phs[i].thread_start = data->start;
			data->phs[i].time_to_die = data->start + data->phs[i].data->die;
			philosophers(&data->phs[i]);
			usleep((data->sleep + data->eat) * 1000);
			ft_free(data);
			exit(0);
		}
		i++;
	}
	return (0);
}

void	init(t_data *data, int i)
{
	while (++i < data->total_num)
	{
		data->phs[i].data = data;
		data->phs[i].num = i;
		data->phs[i].dead = 0;
		data->phs[i].eat_num = 0;
		data->phs[i].hungry = 0;
		data->phs[i].iseating = 0;
	}
	data->print = sem_open("print", O_CREAT | O_EXCL, 0644, 1);
	data->allfed = sem_open("allfed", O_CREAT | O_EXCL, 0644, 1);
	data->exit = sem_open("exit", O_CREAT | O_EXCL, 0644, 0);
	data->forks = sem_open("forks", O_CREAT | O_EXCL, 0644, data->total_num);
	data->eat_num_sem = sem_open("eat_num_sem", O_CREAT | O_EXCL, 0644, 0);
	data->odd_lock = sem_open("odd_lock", O_CREAT | O_EXCL, 0644, 1);
	sem_unlink("forks");
	sem_unlink("eat_num_sem");
	sem_unlink("exit");
	sem_unlink("print");
	sem_unlink("allfed");
	sem_unlink("odd_lock");
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
	if (data->phs == NULL)
		return (1);
	init(data, i);
	return (0);
}

int	main(int ac, char **av)
{
	t_data		data;
	int			i;

	if (ac < 5 || ac > 6)
		return (printf("Arguments are incorrect\n"));
	if (parser(ac, av, &data))
		return (printf("Arguments are incorrect\n"));
	if (start_forks(&data))
		return (ft_free(&data) && printf("Process spawn went wrong"));
	sem_wait(data.exit);
	data.stop = 1;
	i = -1;
	usleep((data.sleep + data.eat) * 1000);
	while (++i < data.total_num)
		kill(data.phs[i].pid, SIGKILL);
	ft_free(&data);
	exit(0);
}
