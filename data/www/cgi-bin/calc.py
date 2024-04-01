#!/usr/bin/env python3
import cgi

# Função para realizar operações matemáticas
def calcular(operando1, operando2, operador):
    if operador == "+":
        return operando1 + operando2
    elif operador == "-":
        return operando1 - operando2
    elif operador == "*":
        return operando1 * operando2
    elif operador == "/":
        if operando2 != 0:
            return operando1 / operando2
        else:
            return "Erro: Divisão por zero"

# Cabeçalho da resposta HTTP
print("Content-type: text/html\n")

# HTML para a página da calculadora
print("<!DOCTYPE html>")
print("<html lang='pt-BR'>")
print("<head>")
print("<meta charset='UTF-8'>")
print("<title>Calculadora CGI</title>")
print("</head>")
print("<body>")
print("<h1>Calculadora CGI</h1>")
print("<form method='get' action='cgi-bin/calculadora.py'>")
print("<p>Operando 1: <input type='text' name='operando1'></p>")
print("<p>Operando 2: <input type='text' name='operando2'></p>")
print("<p>Operador (+, -, *, /): <input type='text' name='operador'></p>")
print("<input type='submit' value='Calcular'>")
print("</form>")
print("<hr>")

# Obtém os valores dos operandos e do operador da URL
form = cgi.FieldStorage()
operando1 = float(form.getvalue("operando1"))
operando2 = float(form.getvalue("operando2"))
operador = form.getvalue("operador")

# Calcula o resultado e exibe na página
if operando1 is not None and operando2 is not None and operador is not None:
    resultado = calcular(operando1, operando2, operador)
    print("<h2>Resultado: {}</h2>".format(resultado))

print("</body>")
print("</html>")