#ifndef PHILS_H
# define PHILS_H

# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>

typedef enum e_bool {
	false,
	true
}				t_bool;

typedef struct s_phils {
	int				num;
	t_bool			dead;
	int				left_fork;
	int				right_fork;
	int				eat_num;
	long			thread_start;
	long			hungry;
	long			time_to_die;
	pthread_mutex_t	omnomnom;
	struct s_data	*data;
}				t_phs;

typedef struct s_data {
	int				total_num;
	int				eat_num;
	long			eat;
	long			sleep;
	long			die;
	t_bool			stop;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print;
	pthread_mutex_t	exit;

	long			start;
	struct s_phils	*phs;
}				t_data;

/*
kinda libft
*/

int		ft_atoi(const char *str);
char	*ft_itoa(long long n);
long	ft_time(void);

/*
main.c
*/
void	ft_free(t_data *data);
void	start_threads(t_data *data);
int		parser(int ac, char **av, t_data *data);
void	init(t_data *data, int i);
long	ft_time(void);

/*
threads.
*/
void	*mondeath(void *philo);
void	printer(char *msg, t_phs *philo, int state);
void	moneat(t_data *data);
void	forks_lock_unlock(t_phs *ph, t_bool y);
void	*philosophers(void *philosopher);
#endif
