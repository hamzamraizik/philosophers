/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmraizik <hmraizik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:04:51 by hmraizik          #+#    #+#             */
/*   Updated: 2024/08/27 14:16:16 by hmraizik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <semaphore.h> // For semaphores
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <stdbool.h>
# include <sys/time.h>
# include <fcntl.h>   // For O_CREAT
# include <sys/stat.h> // For mode constants


typedef sem_t	t_sem;
typedef struct s_data	t_data;
typedef struct s_philo	t_philo;

// utils
int		ft_atoi(char *s);
char	*ft_itoa(int n);
long	current_time(void);
void	print_status(t_data *data, int id, const char *status);
void	ft_usleep(long time, t_data *data);
void	ft_thinking(t_data *data, long philo_id);
void	ft_eating(t_data *data, int i);
void	ft_sleeping(t_data *data, int i);
int		init_philos(t_data *data);
int		init_data(t_data *data);
void	philos_routine(void *void_data);
void	monitor_routine(t_data *data);
int		parsing(char **av, t_data *data);
void	ft_unlock(t_data *data);
int		check(t_data *data);
int		check_time_dead(t_data *data, int i);
int		check_meals_full_dead(t_data *data, int count);
void	update_infos_of_philo(t_data *data, t_philo *philo);

typedef struct s_fork
{
	t_sem	*fork;
	int		fork_id;
}		t_fork;

typedef struct s_philo
{
	long		id;
	char		*name;
	long		meals_counter;
	long		last_meal_time;
	pid_t		pid; // Process ID for each philosopher
	t_fork		*first_fork;
	t_fork		*second_fork;
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
	t_sem			*print_lock;
	t_sem			*simulation_lock;
}			t_data;

#endif // PHILO_H
