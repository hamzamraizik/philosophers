#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <stdbool.h>
# include <sys/time.h>
# include <semaphore.h>
# include <sys/wait.h> 
# include <fcntl.h> // For O_CREAT, O_EXCL

typedef struct s_data	t_data;
typedef struct s_philo	t_philo;

// Utils
int		ft_atoi(char *s);
long	current_time(void);
void	print_status(t_data *data, int id, const char *status);
void	ft_usleep(long time, t_data *data);
void	ft_thinking(t_data *data, long philo_id);
void	ft_eating(t_data *data, int i);
void	ft_sleeping(t_data *data, int i);
int		init_philos(t_data *data);
int		init_data(t_data *data);
void	*philos_routine(void *void_data);
void	*routine_monitor(void *void_data);
int		parsing(char **av, t_data *philos);
void	update_infos_of_philo(t_data *data, t_philo *philo);
int		check(t_data *data);
int		check_time_dead(t_data *data, int i);
int		check_meals_full_dead(t_data *data, int count);
void	cleanup_semaphores(t_data *data);
void	philosopher_process(t_philo *philo);

typedef struct s_philo
{
	long		id;
	long		meals_counter;
	long		last_meal_time;
	pid_t		process_id;
	t_data		*data;
}		t_philo;

typedef struct s_data
{
	long		number_of_philos;
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	long		must_eat;
	long		is_full_counter;
	bool		end_simulation;
	long		start_time;
	t_philo		*philos;
	sem_t		*forks;       // Semaphore for forks
	sem_t		*print_lock;  // Semaphore for printing
	sem_t		*sim_lock;    // Semaphore for simulation lock
}			t_data;

#endif //PHILO_BONUS_H
