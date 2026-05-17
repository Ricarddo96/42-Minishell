*This project has been created as part of the 42 cursus by ridoming, bsaldana.*

# Minishell

## Description

`minishell` is a minimalist Unix shell written in C as part of the 42 cursus. The
goal of the project is to build, from scratch, a simple but functional command
interpreter that mimics the core behavior of `bash`. It aims to give a deep
understanding of how a shell works internally: how processes are created and
synchronized, how file descriptors are duplicated, how signals are handled, and
how a line of text is turned into an executable pipeline of programs.

The shell reads input through GNU `readline`, tokenizes the line, expands
environment variables and quotes, validates the syntax, builds a list of
commands and finally executes them, optionally connected through pipes and
redirections.

### Implemented features

- Interactive prompt with command history (via `readline` / `add_history`).
- Lexer / tokenizer with support for single quotes (`'...'`) and double quotes
  (`"..."`), preserving their respective expansion rules.
- Environment variable expansion (`$VAR`) and exit status expansion (`$?`).
- Pipelines of arbitrary length (`cmd1 | cmd2 | cmd3 | ...`).
- Redirections:
  - Input: `<`
  - Output (truncate): `>`
  - Output (append): `>>`
  - Heredoc: `<<` (pre-read before execution).
- Resolution of executables through the `PATH` environment variable.
- Signal handling that reproduces `bash` behavior in the three relevant states:
  interactive prompt, command execution and heredoc input.
- Built-in commands implemented from scratch:
  - `echo` (with the `-n` flag)
  - `cd` (with relative and absolute paths)
  - `pwd`
  - `export` (with and without arguments)
  - `unset`
  - `env`
  - `exit` (with numeric argument and proper status)
- Correct propagation of the exit status (`$?`) of the last executed command.
- Memory management designed to free every allocation on exit, validated with
  `valgrind` and a `readline` suppression file.

## Instructions

### Requirements

- A Unix-like operating system (Linux or macOS).
- Compiler: `cc`.
- GNU `make`.
- The `readline` library and its development headers.
  - Debian / Ubuntu: `sudo apt install libreadline-dev`
  - Fedora: `sudo dnf install readline-devel`
  - macOS (Homebrew): `brew install readline`

### Build

The project is built through the provided `Makefile`, which also compiles the
in-house `libft` located in the `libft/` directory.

```bash
make            # builds the libft and the minishell binary
make clean      # removes object files
make fclean     # removes object files and the binary
make re         # fclean + all
```

The compilation flags `-Wall -Wextra -Werror` are enforced, and the final
binary is linked against `libft.a` and `libreadline`.

### Run

Once compiled, launch the shell from the project root:

```bash
./minishell
```

A prompt will appear, ready to accept commands as a regular shell would.
Use `exit` (or `Ctrl+D`) to leave the shell.

To run the shell under `valgrind` with a `readline`-aware suppression file, copy the next file in the folder /includes:

```bash
{
   ignore_readline_leaks
   Memcheck:Leak
   ...
   fun:readline
}
{
   ignore_add_history_leaks
   Memcheck:Leak
   ...
   fun:add_history
}
```

Then execute this prompt to run the program.

```bash
make valgrind
```

### Usage examples

```bash
$ ./minishell
minishell$ echo "Hello, $USER" | tr a-z A-Z
HELLO, RIDOMING
minishell$ ls -la > files.txt
minishell$ cat < files.txt | grep ".c" | wc -l
14
minishell$ cat << EOF > out.txt
> first line
> second line
> EOF
minishell$ export GREET="hi there"
minishell$ echo $GREET
hi there
minishell$ exit 0
exit
$
```

## Project structure

```
.
├── Makefile
├── includes/          # public headers (minishell.h, readline.supp)
├── libft/             # in-house C standard library
└── srcs/
    ├── main/          # entry point, struct initialization, prompt loop
    ├── parser/        # tokenizer, expansor, validator, command list builder
    ├── exec/          # executor, path resolution, redirections, pipes
    ├── builtins/      # echo, cd, pwd, export, unset, env, exit
    └── signals/       # signal handlers for the different shell states
```

The high-level pipeline of the shell is:

`readline` → `tokenize` → `validate` → `expand` → `build_cmd_list` →
`pre_read_heredocs` → `executor` (single command or pipeline) → free everything
→ next iteration.

## Resources

The following references were consulted while designing and implementing the
project.

### Official documentation and standards

- [Bash Reference Manual (GNU)](https://www.gnu.org/software/bash/manual/bash.html)
- [POSIX Shell Command Language (IEEE Std 1003.1)](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html)
- [GNU Readline Library Manual](https://tiswww.case.edu/php/chet/readline/readline.html)
- Manual pages: `man 2 fork`, `man 2 execve`, `man 2 pipe`, `man 2 dup2`,
  `man 2 wait`, `man 2 waitpid`, `man 2 sigaction`, `man 3 readline`,
  `man 3 termios`.

### Articles and tutorials

- Beej's Guide to Unix IPC — [https://beej.us/guide/bgipc/](https://beej.us/guide/bgipc/)
- *Write a Shell in C* by Stephen Brennan —
  [https://brennan.io/2015/01/16/write-a-shell-in-c/](https://brennan.io/2015/01/16/write-a-shell-in-c/)
- *Let's Build a Shell* — [https://github.com/kamalmarhubi/shell-workshop](https://github.com/kamalmarhubi/shell-workshop)
- *The Architecture of Open Source Applications: Bash* —
  [https://aosabook.org/en/bash.html](https://aosabook.org/en/bash.html)

### Use of AI

Artificial intelligence assistants (mainly large language models) were used
during the development of the project as a learning and review tool, never as
a substitute for understanding the code. Specifically, AI was used for:

- **Conceptual explanations**: clarifying how `fork`/`execve`/`dup2` interact,
  how heredocs are typically pre-read before execution, and how `bash` handles
  signals in its different states.
- **Reference comparison**: contrasting the observed behavior of `minishell`
  with the expected behavior of `bash` in edge cases (quoting, expansion,
  exit codes such as `127` and `126`, signal-derived statuses like `130`).
- **Code review**: spotting potential file-descriptor or memory leaks in the
  executor and pipe handling, and suggesting cleaner ways to free the token
  and command lists.

All AI suggestions were manually reviewed, adapted to the project's coding
style (Norminette-compliant), and tested before being integrated. No code was
copy-pasted blindly: every function in the repository was written, understood
and debugged by the authors.

## Authors

- **Ricardo Domínguez Fernández** — `ridoming` — 42 Madrid
- **Blukker Saldaña Manosalva** — `bsaldana` — 42 Madrid
