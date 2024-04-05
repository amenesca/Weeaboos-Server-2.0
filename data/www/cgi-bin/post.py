#!/usr/bin/env python3

import cgi
import os

form = cgi.FieldStorage()
nome = form.getvalue("nome")
email = form.getvalue("email")

print('<html lang="pt-br">')
print("<head>")
print('<meta charset="UTF-8">')
print('<meta name="viewport" content="width=device-width, initial-scale=1.0">')
print("<title>Formulário POST</title>")
print("</head>")
print("<body>")
print("<h1>Formulário POST</h1>")
print(f"<p>Nome: {nome}</p>")
print(f"<p>Email: {email}</p>")
print("</body>")
print("</html>")
