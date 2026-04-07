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


**PARA BLUKKER: HACER LA FUNCIÓN DEL TOKENIZER "tokenizer_quotes"**:

La función recibe line e i apuntando a la comilla de apertura (" o '). Tiene que devolver la nueva posición de i tras haber creado el token.

Paso 1 — Guardar qué tipo de comilla abrió
line[i] es " o '. Guárdalo en una variable char quote antes de avanzar. Lo necesitarás para saber cuándo cierra.

Paso 2 — Avanzar pasando la comilla de apertura
i++ para no incluirla en el token.

Paso 3 — Guardar la posición de inicio del contenido
start = i. El contenido del token empieza aquí.

Paso 4 — Avanzar hasta encontrar la comilla de cierre
while que avance mientras line[i] no sea quote y no sea \0. Al salir del while tienes dos casos:

line[i] == quote → comilla cerrada correctamente
line[i] == '\0' → comilla sin cerrar → error, devuelve -1
Paso 5 — Extraer el substring entre comillas
Con ft_substr(line, start, i - start) obtienes el contenido sin las comillas.

Paso 6 — Crear el token y añadirlo a la lista
Igual que en tokenize_word: new_token(word, WORD), free(word), comprobar NULL, add_back.

Paso 7 — Avanzar pasando la comilla de cierre
i++ para no procesar la comilla de cierre como parte del siguiente token.

Paso 8 — Devolver i
La nueva posición para que tokenize continúe desde ahí.

Caso especial importante
¿Qué pasa con "hola"mundo? Después de cerrar las comillas, si el siguiente carácter no es espacio ni operador, sigue siendo el mismo token. Por ahora puedes ignorarlo e implementarlo después, pero tenlo en mente.