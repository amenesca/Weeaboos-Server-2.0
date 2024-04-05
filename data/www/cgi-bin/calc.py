#!/usr/bin/env python3
import cgi

# Recebe os parâmetros da URL
form = cgi.FieldStorage()

if form.getvalue('num1') is None or form.getvalue('num2') is None or form.getvalue('operator') is None:
    print('<html lang="pt-br">')
    print("<head>")
    print('<meta charset="UTF-8">')
    print('<meta name="viewport" content="width=device-width, initial-scale=1.0">')
    print("<title>Erro</title>")
    print("</head>")
    print("<body>")
    print("<h2>Erro: Parametros não informados</h2>")
    print("</body>")
    print("</html>")
    exit()

num1 = float(form.getvalue('num1'))
num2 = float(form.getvalue('num2'))
operator = form.getvalue('operator')

if operator == 'add':
    result = num1 + num2
elif operator == 'subtract':
    result = num1 - num2
elif operator == 'multiply':
     result = num1 * num2
elif operator == 'divide':
    if num2 != 0:
        result = num1 / num2
    else:
        result = "Erro: Divisao por zero"

print('<html lang="pt-br">')
print("<head>")
print('<meta charset="UTF-8">')
print('<meta name="viewport" content="width=device-width, initial-scale=1.0">')
print("<title>Resultado</title>")
print("</head>")
print("<body>")
print(f"<h2>Resultado: {result}</h2>")
print("</body>")
print("</html>")

