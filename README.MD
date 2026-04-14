Fase 1 Tokenizado y Parseo:

Hay que transformar una línea de texto bruta en una **estructura de datos** que tu ejecutor pueda entender.

---

## 1. Fase de Tokenización (Lexer)

El objetivo es dividir la línea en "piezas" (tokens) ignorando los espacios, pero respetando las reglas de la shell. Debes identificar:

* **Palabras/Comandos:** El comando principal y sus argumentos (ej: `ls`, `-la`).
* **Operadores de Redirección:** * `<` (Entrada)
* `>` (Salida truncando)
* `<<` (Here-doc)
* `>>` (Salida append/añadiendo)


* **Pipes:** El carácter `|`.

---

## 2. Gestión de Comillas (Quotes)

Este es el punto donde la mayoría de la gente falla. Debes implementar:

* **Comillas Simples (`'`):** Todo lo que hay dentro es literal. No se expanden variables.
* **Comillas Dobles (`"`):** Se permite la expansión de variables (`$VAR`), pero el contenido se mantiene como un solo token aunque tenga espacios.
* **Comillas vacías:** Manejar casos como `""` o `''` (deben ser un token vacío, no "nada").

---

## 3. Expansión de Variables de Entorno

Antes de ejecutar, debes buscar el símbolo `$` y sustituirlo por su valor:

* **Variables estándar:** `$USER`, `$PATH`, `$HOME`.
* **El estado de salida (`$?`):** Debes sustituir `$?` por el número que devolvió el último comando ejecutado.
* **Variables inexistentes:** Si pones `$ESTO_NO_EXISTE`, debe expandirse a un string vacío (a menos que esté entre comillas simples).

---

## 4. Limpieza (Syntactic Analysis)

Una vez tienes los tokens, debes validar que la frase "tenga sentido" antes de intentar ejecutarla:

* **Pipes mal puestos:** `| ls` o `ls | | wc` o `ls |`.
* **Redirecciones huérfanas:** `ls >` (sin archivo después).
* **Comillas abiertas:** ¿Qué pasa si el usuario escribe `ls "archivo` y nunca cierra la comilla? (Normalmente se considera un error de sintaxis en Minishell).

---

## 5. Estructura de Datos Final

No devuelvas solo un `char **`. Para las siguientes fases, lo ideal es que el parser rellene una **lista enlazada** donde cada nodo sea un comando separado por un pipe. Algo así:

```c
typedef struct s_cmd {
    char    **args;      // ["ls", "-la", NULL]
    char    *infile;    // nombre del archivo de <
    char    *outfile;   // nombre del archivo de > o >>
    int     append;     // booleano para saber si es > o >>
    struct s_cmd *next; // Siguiente comando tras un pipe
} t_cmd;

```

---


Parseo: 

3 fases separadas


input string
     │
     ▼
 [LEXER]          → lista enlazada de tokens con tipo
     │               TOKEN_WORD, TOKEN_PIPE, TOKEN_REDIR_IN,
     │               TOKEN_REDIR_OUT, TOKEN_APPEND, TOKEN_HEREDOC
     ▼
 [VALIDATOR]      → comprueba que la secuencia de tokens es válida
     │               (no pipe al inicio/fin, no dos pipes seguidos,
     │                no redireccion sin archivo después, etc.)
     ▼
 [PARSER]         → convierte los tokens en array de t_cmd
¿Por qué separar lexer de validator de parser?


El lexer solo clasifica, no juzga. Convierte ls -la | grep foo > out en una lista de tokens tipados. También es donde manejas las comillas (simples y dobles), que es lo más delicado.
El validator trabaja sobre tokens, no sobre strings crudos. Es muchísimo más fácil comprobar PIPE → PIPE o REDIR → EOF sobre una lista de tipos que sobre caracteres.
El parser construye las t_cmd ya con la garantía de que la entrada es válida.
Lo más importante que debes resolver primero
Antes de cualquier estrategia, el manejo de comillas es el punto más crítico del lexer:

echo "hello world"   → 1 token WORD: hello world  (sin comillas)
echo 'it'"'"'s'      → 1 token WORD: it's
echo "$HOME"         → expand variable
echo '$HOME'         → literal, NO expandir
Si resuelves bien las comillas en el lexer, el resto fluye. Si las ignoras y las tratas después, todo se complica.



---
---
---

## Reestructuracion del parser - 14 abril 2026

### Que se ha hecho

Se ha reestructurado todo el parser en un pipeline de 5 fases limpias y separadas. Antes estaba todo mezclado (el tokenizer expandia variables y quitaba comillas a la vez). Ahora cada fase hace una sola cosa.

### El pipeline actual

```
Input string
     |
     v
[1. validate_quotes]  -->  Comprueba comillas sin cerrar ANTES de tokenizar
     |
     v
[2. tokenize]         -->  Corta en tokens respetando comillas como delimitadores
     |                      Los tokens salen CRUDOS (con comillas incluidas)
     v
[3. validate]         -->  Comprueba sintaxis: pipes, redirecciones
     |                      Si hay error: exit_status = 2
     v
[4. expand]           -->  Expande $VAR y $? + elimina comillas (todo en un paso)
     |                      Respeta comillas simples (no expande dentro)
     v
[5. build_cmd_list]   -->  Transforma tokens en lista enlazada de t_cmd
     |                      Cada t_cmd tiene args[], redirs y next
     v
  LISTO PARA EJECUCION
```

### Cambios por archivo

**minishell.h**
- `t_sh` se pasa por puntero (`t_sh *mini`) en todas partes
- Nueva struct `t_redir` (lista enlazada de redirecciones por comando)
- `t_cmd` actualizada: usa `t_redir *redirs` en vez de `infile/outfile/append`
- `t_sh` tiene `cmd_list` para guardar el resultado del parser
- Eliminado `single_quoted` de `t_tkn` (ya no se necesita)

**tokenizer.c**
- Eliminado TODO el codigo de expansion (append_char, check_in_envp, process_token, vars_in_str)
- Nuevo: `is_word_end` y `update_quote_state` para trackear estado de comillas
- `tokenize_complex_word` reescrita: solo corta, no expande ni quita comillas
- Salta tabs ademas de espacios

**validator.c**
- Nueva funcion `validate_quotes`: detecta comillas sin cerrar antes de tokenizar
- `validate_redir_sintax` mejorada: muestra el token real en el error (no siempre "newline")
- `validate` recibe `t_sh *mini` y pone `exit_status = 2` en errores

**expansor.c**
- Reescrito para funcionar como paso separado del pipeline
- `expand_token` tiene state machine de comillas: expande $ en NONE y DOUBLE, no en SINGLE
- Tambien hace quote removal (las comillas se eliminan al procesar)
- Soporta `$?` (exit status) via `get_var_value`
- Maneja `$` solo como literal
- Todos los valores se retornan con ft_strdup/ft_itoa (memoria propia)

**parser.c**
- Orquesta las 5 fases en orden
- Contiene `new_cmd`, `add_arg`, `add_redir`, `build_cmd_list`
- Contiene `free_tokens`, `free_redirs`, `free_cmd_list`
- Libera tokens despues de construir la cmd_list
- Libera tokens en caso de error de validacion

**main.c**
- Pasa `&mini` a parser (puntero)
- Libera `cmd_list` despues de cada iteracion del bucle
- Inicializa `cmd_list = NULL`

### Structs actuales

```c
typedef struct s_redir
{
    t_tkn_type       type;     // REDIR_IN, REDIR_OUT, REDIR_APPEND, REDIR_HEREDOC
    char             *file;    // nombre archivo o delimitador heredoc
    struct s_redir   *next;
}   t_redir;

typedef struct s_cmd
{
    char            **args;    // ["grep", "-i", "hello", NULL] (como argv)
    t_redir         *redirs;   // lista enlazada de redirecciones
    struct s_cmd    *next;     // siguiente comando tras un pipe
}   t_cmd;
```

### Lo que falta

- Ejecucion de comandos (fork + execve)
- Pipes entre comandos
- Aplicar redirecciones (open + dup2 recorriendo cmd->redirs)
- Heredoc (leer lineas hasta delimitador)
- Builtins (echo, cd, pwd, export, unset, env, exit)
- Signals (Ctrl+C, Ctrl+D, Ctrl+\)
- Manejar readline NULL (Ctrl+D)
