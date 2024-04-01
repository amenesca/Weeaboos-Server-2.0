#!/usr/bin/env python3
import cgi
import cgitb
cgitb.enable()

print("Content-type:text/html\r\n\r\n")
print("<html>")
print("<head>")
print("<title>Resultado</title>")
print("</head>")
print("<body>")

form = cgi.FieldStorage()
expression = form.getvalue('expression')

try:
    result = eval(expression)
    print("<h2>O resultado é: {}</h2>".format(result))
except Exception as e:
    print("<h2>Erro ao calcular: {}</h2>".format(str(e)))

print("</body>")
print("</html>")