
# Sql Parser

https://github.com/baidu/BaikalDB/tree/master/include/sqlparser

# Bison

```
%code requires {} // file.h -> top, above YYSTYPE、YYLTYPE 
%code provides {} // file.h -> bottom, under YYSTYPE、YYLTYPE 
%code top {} // file.cc -> top
%code {} // file.cc -> %{···%}
```