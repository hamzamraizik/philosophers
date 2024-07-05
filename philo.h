#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <stdbool.h>
# include <sys/time.h>

# define RED  "\x1B[31m"

typedef pthread_mutex_t	t_mutex;
typedef struct s_data	t_data;
typedef struct s_philo	t_philo;

// utils
void	ft_exit(const char *s);
void	ft_error(t_data *data, const char *s);
int		ft_atoi(char *s);
long	current_time(void);
void	print_status(t_data *data, int id, const char *status);
void	ft_usleep(long time, t_data *data);
void	ft_thinking(t_data *data, long philo_id);
void	ft_eating(t_data *data, int i);
void	ft_sleeping(t_data *data, int i);
void	init_philos(t_data *data);
void	init_data(t_data *data);
void	*philos_routine(void *void_data);
void	*routine_monitor(void *void_data);
void	parsing(char **av, t_data *philos);
void	ft_end(t_data *philos, int j);
void	ft_unlock(t_data *data);
int		check(t_data *data);
int		check_time_dead(t_data *data, int i);
int		check_meals_full_dead(t_data *data, int count);
void	ft_unlock(t_data *data);
void	update_infos_of_philo(t_data *data, t_philo *philo);

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
	long			must_eat;
	long			is_full_counter;
	bool			end_simulation;
	long			start_time;
	t_fork			*forks;
	t_philo			*philos;
	t_mutex			print_lock;
	t_mutex			simulation_lock;
	pthread_t		monitor_thread;
}			t_data;

#endif //PHILO_H