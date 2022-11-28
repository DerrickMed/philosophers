SRCS	=	main.c threads.c ft_atoi.c ft_itoa.c

NAME	=	philosophers

OBJS	   = $(patsubst %.c,%.o,$(SRCS))

D_FILES     = $(patsubst %.c,%.d,$(SRCS))

CC		=	gcc

FLAGS	=	-Wall -Wextra -Werror -g -MMD

LIB		=	-lpthread

all:		$(NAME)

$(NAME):	$(OBJS)
			$(CC) $(FLAGS) -o $(NAME) $(OBJS) $(LIB)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

-include $(wildcard $(D_FILES))

clean:
			rm -rf $(OBJS)
			rm -f ${D_FILES}

fclean:		clean
			rm -rf $(NAME)


re:			fclean all

.PHONY: 	all clean fclean re

