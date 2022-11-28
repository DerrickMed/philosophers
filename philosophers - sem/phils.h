#ifndef PHILS_H
# define PHILS_H

# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>
# include <signal.h>
# include <semaphore.h>
# include <fcntl.h>

typedef enum e_bool {
	false,
	true
}				t_bool;

typedef struct s_phils {
	int				num;
	t_bool			dead;
	long			thread_start;
	long			hungry;
	long			time_to_die;
	struct s_data	*data;
	pid_t			pid;
	t_bool			iseating;
	int				eat_num;
}				t_phs;

typedef struct s_data {
	int				total_num;
	sem_t			*eat_num_sem;
	int				eat_num;
	long			eat;
	long			start;
	long			sleep;
	long			die;
	t_bool			stop;
	sem_t			*forks;
	sem_t			*print;
	sem_t			*exit;
	sem_t			*odd_lock;
	sem_t			*allfed;
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
int		ft_free(t_data *data);
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
void	*philosophers(t_phs *philosopher);
#endif
