#ifndef PHILO_H
#define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <stdbool.h>
# include <sys/time.h>

# define RED  "\x1B[31m"
typedef pthread_mutex_t	t_mutex;
typedef struct s_data t_data;

// utils
void	ft_exit(const char *s);
int		ft_atoi(char *s);
long	current_time(void);
void	print_status(t_data *data, int id, const char *status);
void	parsing(char **av, t_data *philos);

typedef struct s_fork
{
	t_mutex	fork;
	int		fork_id;
}		t_fork;

typedef struct s_philo
{
	long		id;
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
	long			number_of_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			how_much_time_must_eat;
	bool			end_simulation;
	t_fork			*forks;
	t_philo			*philos;
	t_mutex			print_lock;
    t_mutex			simulation_lock;
}			t_data;

#endif //PHILO_H