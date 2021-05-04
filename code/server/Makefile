CC				:= gcc
CFLAGS			:= -Wall -Wextra -g -pthread
SRC				:= src
OBJ				:= bin/extra
BINDIR			:= bin
SRCS			:= $(wildcard $(SRC)/*.c) $(wildcard $(SRC)/**/*.c)
OBJS			:= $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
BIN				:= $(BINDIR)/server
SUBMITFILE		:= server.zip

all:			$(BIN)

DEPS			:= $(patsubst $(OBJ)/%.o, $(OBJ)/%.d, $(OBJS))
-include $(DEPS)
DEPFLAGS 		= -MMD -MF $(@:.o=.d)

$(BIN):			$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJ)/%.o:		$(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(DEPFLAGS)

clean:
	$(RM) -r $(BIN) $(OBJS) $(DEPS)

submit:
	$(RM) $(SUBMITFILE)
	zip -r $(SUBMITFILE) .