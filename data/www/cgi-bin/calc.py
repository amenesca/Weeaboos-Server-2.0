#!/usr/bin/env python3
import cgi

form = cgi.FieldStorage()

if "num1" in form and "num2" in form and "operacao" in form:
    try:
        num1 = float(form["num1"].value)
        num2 = float(form["num2"].value)
        operacao = form["operacao"].value

        resultado = None

        if operacao == "+":
            resultado = num1 + num2
        elif operacao == "-":
            resultado = num1 - num2
        elif operacao == "*":
            resultado = num1 * num2
        elif operacao == "/":
            if num2 != 0:
                resultado = num1 / num2
            else:
                resultado = "Erro: Divisão por zero não é permitida!"

        if resultado is not None:
            print(f"<p>O resultado de {num1} {operacao} {num2} = {resultado}</p>")
    except ValueError:
        print("<p>Erro: Por favor, insira números válidos.</p>")
else:
    print("Os campos 'num1' ou 'num2' estão vazios.")