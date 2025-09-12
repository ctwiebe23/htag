# NAME

htag — Create html in the shell

# SYNOPSIS

**htag** *tag-name* [-\-*attr-name* *attr-val*].\.. [*text-content*].\..

# DESCRIPTION

Create HTML tags and attributes in the shell.

The first argument must be the name of the tag.  All other non-flag arguments
are inserted into the tag as text content.

If the tag name is the only non-flag argument, then text is read from the
STDIN.

Long flags are automatically converted into attributes, and all consume one
argument as the attribute value.

Prints the created tag to the STDOUT.

# OPTIONS

-h, \--help
:   Prints a help message.

-V, \--version
:   Prints the program's version.

\--
:   Parse flags like text after this is read.

# RETURN CODES

0
:   Returned on success.

1
:   Returned when an attribute is not given a value.

# EXAMPLES

Printing an empty tag:

```
$  htag p ""
>  !((htag p ""))!
```

Printing some text:

```
$  htag p Some text
>  !((htag p Some text))!

$  htag p "Some text"
>  !((htag p "Some text"))!

$  echo "Some text" | htag p
>  !((echo "Some text" | htag p))!
```

Adding attributes:

```
$  htag a "An anchor tag" --href "https://foobar.baz"
>  !((htag a "An anchor tag" --href "https://foobar.baz"))!
```

A more complicated example:

```
$  for FILE in $(ls)
   do htag li "$FILE"
   done | htag ul
>  !((
for FILE in $(ls)
do htag li "$FILE"
done | htag ul
))!
```

# SEE ALSO

Source code can be found at <https://github.com/ctwiebe23/htag>.
