Para compilar el codigo en C++ se debe compilar el archivo XYZ.cpp con el codigo del programa:

        g++ XYZ.cpp -O2 -o XYZ

Para compilar el codigo en Java se debe compilar con:

        javac XYZ.java

Luego para testear utilizando, por ejemplo, el caso de prueba número T se debe correr el siguiente comando:

        java -jar XYZTester.jar -exec XYZ -test T

Si se usa el código en C++ XYZ es XYZ.exe, y si se usa el código en Java XYZ es "java XYZ"

El argumento de -test es el número de test que se correra, siendo todos los tests generados al azar.
