#!/usr/bin/env python3
import cgi
import os

# Obter o nome do arquivo a ser excluído a partir dos dados do formulário
form = cgi.FieldStorage()
filename = form.getvalue('filename')
print(filename)
# Caminho completo para o arquivo
filepath = os.path.join("data/www/cgi-bin/tmp/", filename)

# Verificar se o arquivo existe e é uma imagem antes de excluí-lo
if os.path.exists(filepath):
    # Excluir o arquivo
    os.remove(filepath)
    message = f'The file "{filename}" has been deleted successfully'
else:
    message = f'The file "{filename}" does not exist or cannot be deleted'

# HTML de resposta para indicar o resultado da exclusão
print("Content-type: text/html\n")
print('<html lang="pt-br">')
print("<head>")
print('<meta charset="UTF-8">')
print('<meta name="viewport" content="width=device-width, initial-scale=1.0">')
print("<title>Delete Result</title>")
print("</head>")
print("<body>")
print("<h1>Delete Result</h1>")
print(f"<p>{message}</p>")
print('<a href="index.html">Voltar</a>')
print("</body>")
print("</html>")