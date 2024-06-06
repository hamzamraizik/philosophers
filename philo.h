#ifndef PHILO_H
#define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <stdbool.h>

# define RED  "\x1B[31m"
typedef pthread_mutex_t	t_mutex;
typedef struct s_data t_data;

// utils
void	ft_exit(const char *s);
int		ft_atoi(char *s);

typedef struct s_fork
{
	t_mutex	fork;
	int		fork_id;
}		t_fork;

typedef struct s_philo
{
	long		id;
	bool		full;
	long		meals_counter;
	long		last_meal_time;
	pthread_t	thread_id;
	t_fork		*first_fork;
	t_fork		*second_fork;
	t_mutex		philo_mutex;
	t_data		*data;
}		t_philo;

typedef struct s_data
{
	long		number_of_philos;
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	long		how_much_time_must_eat;
	bool		end_simulation;
	bool		all_threads_ready;
	t_fork		*forks;
	long		threads_running_nbr;
	pthread_t	monitor;
	t_philo		*philos;
	t_mutex		mutex_table;
	t_mutex		write_mutex;
}			t_data;

#endif //PHILO_H