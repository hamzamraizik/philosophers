NAME = philo_bonus
C_SCR = main.c utils.c simulation_func.c init.c utils2.c
O_SRC = $(C_SCR:.c=.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror

%.o : %.c philo_bonus.h
	$(CC) $(CFLAGS) -c $<

all: $(NAME)

$(NAME): $(O_SRC)
	$(CC) $(CFLAGS) $(O_SRC) -o $(NAME)

clean:
	rm -f $(O_SRC)

fclean: clean
	rm -f $(NAME)

re: fclean all
