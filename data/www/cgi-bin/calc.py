#!/usr/bin/env python3
import cgi

# Recebe os parâmetros da URL
form = cgi.FieldStorage()

# Obtém os valores dos parâmetros
num1 = float(form.getvalue('num1'))
num2 = float(form.getvalue('num2'))
operator = form.getvalue('operator')

# Verifica se os campos estão vazios
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

# Imprime o cabeçalho HTTP e o resultado
print("Content-type:text/html\r\n\r\n")
print("<html>")
print("<head>")
print("<title>Resultado</title>")
print("</head>")
print("<body>")
print(f"<h2>Resultado: {result}</h2>")
print("</body>")
print("</html>")

