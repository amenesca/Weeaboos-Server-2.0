#!/usr/bin/env python3
import cgi

# Função para calcular o resultado com base na operação selecionada
def calculate(num1, num2, operation):
    if operation == "add":
        return num1 + num2
    elif operation == "subtract":
        return num1 - num2
    elif operation == "multiply":
        return num1 * num2
    elif operation == "divide":
        if num2 != 0:
            return num1 / num2
        else:
            return "Erro: Divisão por zero"

# Pegar os dados do formulário
form = cgi.FieldStorage()
num1 = float(form.getvalue("num1"))
num2 = float(form.getvalue("num2"))
operation = form.getvalue("operation")

# Calcular o resultado
result = calculate(num1, num2, operation)

# Imprimir o resultado como uma resposta HTTP
print("Content-type:text/html\r\n\r\n")
print("<html>")
print("<head>")
print("<title>Resultado</title>")
print("</head>")
print("<body>")
print("<h2>Resultado:</h2>")
print("<p>{}</p>".format(result))
print("</body>")
print("</html>")