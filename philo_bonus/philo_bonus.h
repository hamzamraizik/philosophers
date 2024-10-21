/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmraizik <hmraizik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 16:14:23 by hmraizik          #+#    #+#             */
/*   Updated: 2024/09/08 18:11:32 by hmraizik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <pthread.h>
# include <signal.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <stdbool.h>
# include <sys/time.h>
# include <semaphore.h>
# include <sys/wait.h> 
# include <fcntl.h> 

typedef struct s_data	t_data;
typedef struct s_philo	t_philo;

int		ft_atoi(char *s);
long	current_time(void);
void	print_status(t_data *data, int id, const char *status);
void	ft_usleep(long time);
void	ft_thinking(t_data *data, long philo_id);
void	ft_eating(t_data *data, int i);
void	ft_sleeping(t_data *data, int i);
int		init_philos(t_data *data);
int		init_data(t_data *data);
void	*routine_monitor(void *void_data);
int		parsing(char **av, t_data *philos);
void	update_infos_of_philo(t_data *data, t_philo *philo);
void	cleanup_semaphores(t_data *data);
void	philosopher_process(t_philo *philo);
void	ft_putstr_fd(char *s, int fd);

typedef struct s_philo
{
	pthread_t	thread;
	int			id;
	int			meals_counter;
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
	long		start_time;
	t_philo		*philos;
	sem_t		*forks;
	sem_t		*print_lock;
	sem_t		*sim_lock;
}			t_data;

#endif //PHILO_BONUS_H