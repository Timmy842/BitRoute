# 🧠 BitRoute

**BitRoute** es una colección de servidores y clientes de red programados en C desde cero, diseñados para aprender y dominar protocolos, estructuras de bajo nivel y comunicación entre sistemas.

---

## 🚀 Objetivos del proyecto

- Comprender el funcionamiento interno de los protocolos TCP y UDP.
- Aprender a diseñar e implementar protocolos personalizados.
- Crear un servidor HTTP simple desde cero.
- Desarrollar servicios como transferencia de archivos y chat en tiempo real.
- Ganar experiencia real programando en C y usando sockets.

---

## 📦 Estructura del proyecto

<pre lang="markdown"> ``` BitRoute/ ├── tcp_echo/ # Servidor y cliente TCP (echo server) ├── udp_echo/ # Servidor y cliente UDP ├── custom_protocol/ # Protocolo binario personalizado ├── http_server/ # Mini servidor HTTP ├── file_server/ # Servidor de archivos con autenticación ├── chat_server/ # Chat multiusuario en tiempo real └── README.md ``` </pre>

---

## 🛠️ Tecnologías utilizadas

- **Lenguaje**: C
- **Sistema operativo**: Linux (WSL en Windows)
- **Herramientas**: GCC, Make, GDB, VSCode

---

## 📚 Módulos

| Módulo | Descripción |
|--------|-------------|
| 1️⃣ TCP Echo         | Cliente y servidor que envían y reciben datos usando TCP |
| 2️⃣ UDP Echo         | Comunicación sin conexión con datagramas UDP |
| 3️⃣ Protocolo personalizado | Estructura binaria tipo `[LENGTH][TYPE][PAYLOAD]` sobre TCP |
| 4️⃣ Mini servidor HTTP | Maneja peticiones `GET`, entrega archivos HTML estáticos |
| 5️⃣ Servidor de archivos | Transferencia segura con autenticación básica |
| 6️⃣ Chat multiusuario | Servidor concurrente con múltiples clientes y canales |

---

## 📦 Compilación

Desde el directorio raíz:

```bash
make              # Compila todos los módulos disponibles
make tcp_echo     # Compila solo el módulo de echo TCP

---

📌 Requisitos
GCC (gcc)

Make (make)

VSCode (opcional pero recomendado)

Linux o WSL (Windows Subsystem for Linux)

---

👨‍💻 Autor
Proyecto desarrollado con fines educativos por Carlos Garcia.

---

⚠️ Licencia
Este proyecto se distribuye bajo la licencia MIT. Consulta el archivo LICENSE para más detalles.
