Tarea corta 2
===
**Estudiante**

- Esteban Mena - 2018026623

Manual de ejecución
---
### Requisitos
- Se debe utilizar cualquier distribución de Linux como sistema operativo, se recomienda Ubuntu 20.04.
- Es necesario tener instalado Docker y Docker Compose, a continuación se describen los pasos a ejecutar en consola para su instalación:
     1. En caso de tener versiones anteriores, procedemos a eliminarlas para evitar conflictos.
         ```console
         $ sudo apt-get remove docker docker-engine docker.io containerd runc
         ```
    2. Actualizamos la lista de paquetes del sistema.
       ```console
        $ sudo apt-get update
        ```
    3. Se instalan las dependencias necesarias.
        ```console   
        $ sudo apt-get install ca-certificates curl gnupg lsb-release \
        apt-transport-https software-properties-common
         ```
    4. Se agrega la clave GPG, esto para garantizar que los archivos de instalación sean auténticos.
        ```console
        $  curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg
        ```
    5. Agregamos el repositorio de Docker.

        ```console
        $ echo \ 
        "deb [arch=$(dpkg --print-architecture) signed by=/usr/share/keyrings/docker-archive-keyring.gpg] https://download.docker.com/linux/ubuntu \ 
        $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
        ```
    6. Actualizamos la información de los paquetes con la modificación.
        ```console
        $ sudo apt-get update
        ```
    7. Y procedemos a realizar la instalación de Docker.
        ```console
        $ sudo apt install docker-ce
        ```
    8. Con el siguiente comando descargamos y guardamos el ejecutable de Docker Compose en la versión 2.4.1.
        ```console
        $ curl -SL https://github.com/docker/compose/releases/download/v2.4.1/docker-compose-linux-x86_64 -o /usr/local/bin/docker-compose
        ```
    9. Otorgamos los permisos adecuados para que sea ejecutable.
        ```console
        $ sudo chmod +x /usr/local/bin/docker-compose
        ```
    10. Por último, si deseamos comprobar la instalación correcta de Docker podemos ejecutar el siguiente comando y se espera un resultado similar al mostrado.
        ```console
        $ sudo systemctl status docker
        ● docker.service - Docker Application Container Engine
        Loaded: loaded (/lib/systemd/system/docker.service; enabled; vendor preset>
        Active: active (running) since Sun 2022-04-24 21:36:33 CST; 4 days ago
        ```
        
### Ejecución

Primero es necesario tener clonado o descargar el contenido del [repositorio](https://github.com/estebanmr99/redesDocker). Luego, para la ejecución es necesario tener dos comandos en cuenta, es importante aclarar que los comandos de Docker Compose se deben ejecutar en el directorio donde se encuentra el archivo 'docker-compose.yml', y uno adicional para el manejo de los contenedores:
1. Se construyen los contenedores.

```console
$ sudo docker-compose build
```
2. Inicia los servicios de los contenedores en segundo plano y los deja en ejecución.

```console
$ sudo docker-compose up -d
```

3. El siguiente comando es de gran utilidad, este permite ingresar a un contenedor en ejecución e interactuar con el mismo mediante el uso de la terminal. Se reemplaza "CONTENEDOR" por el nombre del contenedor objetivo.
```console
$ docker exec -it CONTENEDOR /bin/bash
```

Consideraciones
----
* El contenedor va correr siempre y cuando no se caiga, esto significa que puede manejar N cantidad de clientes simultáneamente pero ante un error, todas las conexiones se perderán.
* Ante una IP inválida o una máscara de bit que no esté dentro del rango de 0-32 se mostrará un mensaje indficando el error en cuestión.

Comandos
---

Con el servidor telnet ejecutado gracias a los pasos anteriores, se mostrarán los comandos implementados y un par de errores.

Como nota, la IP 192.168.100.81 solo se usa de referencia, se debe usar la IP del host donde se ejecutan las pruebas, esto se puede verificar con:

```console
ip route
```

Para ejecutar los comandos hacia el servidor, se debe de establecer primero una conexión telnet con el servidor con la IP del host y el puerto 9666.
```console
telnet 192.168.100.81 9666
```
![](https://i.imgur.com/dv5Rvle.jpg)


- **GET BROADCAST IP {dirección IP} MASK {máscara en formato /bits o notación X.X.X.X}**
    Se puede ejecutar el comando de la siguientes maneras:
    1. Máscara en formato /bits
    ![](https://i.imgur.com/yhpKbij.jpg)
    2. Máscara en notación X.X.X.X
    ![](https://i.imgur.com/DSfV5iZ.jpg)
  
- **GET NETWORK NUMBER IP {dirección IP} MASK {máscara en formato /bits o notación X.X.X.X}**
    Se puede ejecutar el comando de la siguientes maneras:
    1. Máscara en formato /bits
    ![](https://i.imgur.com/RLtggmb.jpg)
    2. Máscara en notación X.X.X.X
    ![](https://i.imgur.com/pPbE8Gn.jpg)

- **GET HOSTS RANGE IP {dirección IP} MASK {máscara en formato /bits o notación X.X.X.X}**
    Se puede ejecutar el comando de la siguientes maneras:
    1. Máscara en formato /bits
    ![](https://i.imgur.com/JZNMtSi.jpg)
    2. Máscara en notación X.X.X.X
    ![](https://i.imgur.com/cY8wnBg.jpg)

- **GET RANDOM SUBNETS NETWORK NUMBER {Y.Y.Y.Y} MASK {mascara en formato /bits o notación X.X.X.X} NUMBER {número de redes} SIZE {mascara en formato /bits o notación X.X.X.X}**
    No implementado, muestra un mensaje de comando inválido ante cualquier comando no identificado:
    ![](https://i.imgur.com/OhCyLEE.jpg)

- **exit**
    Se puede ejecutar el comando ```exit``` para cerrar la conexión con el servidor telnet sin apagarlo.
    ![](https://i.imgur.com/NIX7zrS.jpg)
